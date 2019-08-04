#include "spi.h"

int spi_fd;

int spi_init() {
    spi_fd = open(SPI_DEV_NAME, O_RDWR);
    if(spi_fd < 0) {
        MSG_ERR("open spi device %s error", SPI_DEV_NAME);
        return 0;
    }

    ioctl(spi_fd, SPI_IOC_WR_MODE, SPI_MODE);
    return 1;
}

uint8_t spi_xfer_buf(const char *txbuf, char *rxbuf, uint32_t len) {
    int ret;
    struct spi_ioc_transfer xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.tx_buf = (uint64_t)txbuf;
    xfer.rx_buf = (uint64_t)rxbuf;
    xfer.len = len;
    xfer.speed_hz = 500000;
    xfer.cs_change = 1;
    xfer.bits_per_word = 8;
    ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &xfer);

    if(ret < 0) {
        MSG_ERR("spi transfer error %d: %s", errno, strerror(errno));
        return 0;
    }
    return 1;
}

uint8_t spi_led_acquire(uint8_t *led_status) {
    char rxbuf[16] = {0x00};
    uint8_t retry = 0;

    spi_xfer_buf("\x21\x00", rxbuf, 2);
    if(!(rxbuf[1] & SPI_SR_READY_MASK)) {
        MSG_ERR("spi device not ready, read cr=0x%02x", rxbuf[1]);
        return 0;
    }

    spi_xfer_buf("\x22\x02", rxbuf, 2);

    do {
        spi_xfer_buf("\x21\x00", rxbuf, 2);
        retry++;
    } while(!(rxbuf[1] & SPI_SR_LED_DV_MASK) && retry < 4);

    if(retry == 4) {
        MSG_ERR("read led status timeout");
        return 0;
    }

    spi_xfer_buf("\x02\x00\x00\x00\x00\x00\x00\x00", rxbuf, 8);

    memcpy(led_status, rxbuf + 2, 7);
    return 1;
}

void led_acquire_handle(sio::socket::ptr socket, sio::message::ptr msg) {
    uint8_t leds[7] = {0x00};
    uint8_t is_success = spi_led_acquire(leds);

    sio::message::ptr p = sio::object_message::create();
    sio::message::ptr led_msg = sio::array_message::create();

    for(int i = 0; i < 56; i++) {
        led_msg->get_vector().push_back(sio::int_message::create(
            (leds[i / 8] & (1 << (i % 8))) > 0  // translate from bytes
        ));
    }

    p->get_map()["leds"] = led_msg;
    socket->emit("LED_SWITCH_STATUS", p);
}