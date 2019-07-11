#pragma once

#include "sio_socket.h"

#include <cstdint>
#include <memory>

#define FW_NAME          "/tmp/fpga_firmware.bin"
#define SYSFS_FPGA_PATH  "/sys/class/fpga_manager/fpga1"

#define MAX_WAIT_RETRIES 5

void prepare_download();

void bitstream_handle(sio::socket::ptr socket, sio::message::ptr msg);

uint8_t bitstream_download(std::shared_ptr<const std::string> bit, const int bit_length);

