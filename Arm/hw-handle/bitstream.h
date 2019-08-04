#pragma once

#include "sio_socket.h"

#include <cstdint>
#include <memory>

#define EXP_FW_BASENAME     "fpga_e_firmware.bin"
#define EXP_FW_TMP_PATH     ("/tmp/" EXP_FW_BASENAME)
#define EXP_FW_LINK_PATH    ("/lib/firmware/" EXP_FW_BASENAME)
#define EXP_FPGA_NAME       "fpga1"

#define CONTROL_FW_NAME     "fpga_c_firmware.bin"
#define CONTROL_FPGA_NAME   "fpga0"

#define SYSFS_FPGA_PATH  "/sys/class/fpga_manager/"

#define MAX_WAIT_RETRIES 5

void bitstream_handle(sio::socket::ptr socket, sio::message::ptr msg);

uint8_t bitstream_download_exp_fpga(std::shared_ptr<const std::string> bit, const int bit_length);
uint8_t bitstream_download_ctrl_fpga();