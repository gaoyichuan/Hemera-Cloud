#include "bitstream.h"
#include "common.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <unistd.h>
#include <boost/filesystem.hpp>

using std::shared_ptr;
using std::string;
namespace fs = boost::filesystem;

uint8_t after_download() {
    int retry = 0;
    char buf[24];
    FILE *state = std::fopen(SYSFS_FPGA_PATH "/state", "r");

    while(retry < MAX_WAIT_RETRIES) {
        retry++;
        std::fread(buf, sizeof(char), 24, state);
        if(std::strcmp(buf, "operating\n") == 0) {
            return 1;
        } else {
            MSG_DBG("wait fpga status: %s, retry %d", buf, retry);
        }
    }

    return 0;
}

void prepare_download() {
    // TODO: prepare
}

void bitstream_handle(sio::socket::ptr socket, sio::message::ptr msg) {
    prepare_download();

    shared_ptr<const string> bit = msg->get_map().at("raw_data")->get_binary();
    int bit_length = msg->get_map().at("raw_data_length")->get_int();
    if(!bit || bit->size() != bit_length) {
        MSG_ERR("invalid bitstream length");
    }
    uint8_t is_success = bitstream_download(bit, bit_length);

    sio::message::ptr p = sio::object_message::create();

    p->get_map()["success"] = sio::bool_message::create(is_success);
    p->get_map()["message"] = sio::string_message::create("");

    socket->emit("BIT_STREAM_RESULT", p);
}

uint8_t bitstream_download(shared_ptr<const string> bit, const int bit_length) {
    uint8_t mode = 0;
    int len;
    const char *bitstream;
    uint8_t is_success = true;

    bitstream = (*bit).c_str();
    len = bit_length;
    MSG_INF("bit size: %d", len);

    std::FILE *fw;
    
    // write tmp bitstream file
    fw = std::fopen(FW_TMP_PATH, "wb");
    std::fwrite(bitstream, sizeof(char), len, fw);

    if(ferror(fw))
        MSG_ERR("Write bitstream file error");

    std::fclose(fw);

    // check if symbolic link to firmware file exists
    if(fs::symlink_status(FW_LINK_PATH).type() != fs::symlink_file) {
        symlink(FW_TMP_PATH, FW_LINK_PATH);
    }

    // write path to sysfs, trigger reconfigure
    fw = std::fopen(SYSFS_FPGA_PATH "/firmware", "w");
    std::fputs(FW_BASENAME, fw);
    std::fclose(fw);

    // MSG_DBG("transfer finished");
    // do something with io
    is_success = after_download();
    MSG_INF("is_success=%d", (int)is_success);
    return is_success;
}