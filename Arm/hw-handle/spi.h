#pragma once

#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "common.h"

#include "sio_socket.h"

#define SPI_DEV_NAME    "/dev/spidev3.0"
#define SPI_MODE        0

#define SPI_SR_READY_MASK   (1 << 0)
#define SPI_SR_LED_DV_MASK  (1 << 1)

#define SPI_CR_RESET_MASK       (1 << 0)
#define SPI_CR_LED_START_MASK   (1 << 1)

extern int spi_fd;

int spi_init();
void led_acquire_handle(sio::socket::ptr socket, sio::message::ptr msg);