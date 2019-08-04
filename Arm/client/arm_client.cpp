/*-----------------------------------------------------
 File Name : arm_client.cpp
 Purpose : main arm code
 Creation Date : 2019-07-11
 Created By : Yichuan Gao
-----------------------------------------------------*/
#include "common.h"

#include "sio_client.h"
#include "sio_socket.h"

#include "bitstream.h"
#include "spi.h"

#include <iostream>
#include <thread>
#include <string>
#include <cstdio>
#include <functional>

using namespace std;
using namespace sio;

int main(int argc, char* argv[]) {
    bitstream_download_ctrl_fpga();
    spi_init();

    sio::client h;
    setlinebuf(stdout);
    cout << "start client" << endl;
    h.set_close_listener([](client::close_reason const& reason) {
        cout << "sio closed" << endl;
    });
    h.set_open_listener([&]() {
        sio::socket::ptr s = h.socket();
        s->on("BIT_STREAM", sio::socket::event_listener_aux([s](string const& name, message::ptr const& data,
                        bool isAck,message::list &ack_resp)
        {
            std::thread t = std::thread(std::bind(bitstream_handle, s, data));
            t.detach();
        }));

        s->on("GET_STATUS", sio::socket::event_listener_aux([s](string const& name, message::ptr const& data,
                        bool isAck,message::list &ack_resp)
        {
            std::thread t = std::thread(std::bind(led_acquire_handle, s, data));
            t.detach();
        }));

    });

    h.connect("http://10.2.2.184:3412");
    return 0;
}
