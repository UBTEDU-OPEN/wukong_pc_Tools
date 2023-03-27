#pragma once
#include <string>
#include "globalDefs.h"

namespace HttpSocket {

    class StreamingSocket
    {
    public:
        StreamingSocket();
        ~StreamingSocket();
    public:
        bool connect(const std::string& remote_host_name,
            int remote_port_number,
            int timeout = 30000);

        bool isConnected() const { return _connected; }

        void close();

        int read(char* dest_buffer, int max_read_length);

        int write(const char* source_buffer, int max_write_length);

        bool writeAll(const char* source_buffer, int max_write_length);
    private:
        bool resetSocketOptions();
    private:
        std::string _host_name;                 //目标地址
        int volatile _port_number, _handle;     //端口号，SOCKET
        bool _connected;                        //是否已连接
    };
}
