#pragma once
#include <string>
#include <memory>
#include "proxyConfig.h"
#include "streamingsocket.h"

namespace HttpSocket {
    //支持代理的Socket类封装
    class ProxySocket
    {
    public:
        ProxySocket();
        ProxySocket(const std::string& remote_host_name, int port_number);
        ~ProxySocket();

    public:
        void setHost(const std::string& remote_host_name, int port_number);
        std::string hostName() const { return _host_name; };
        int portNumber() const { return _port_number; };
        static void setProxy(const ProxyConfig& proxy_config);

        bool connect();
        bool isConnected() const { return _socket.isConnected(); }
        int read(char* dst_buffer, int max_read_length);
        int write(const char* source_buffer, int max_write_length);
        bool writeAll(const char* source_buffer, int max_write_length);    //确保所有数据都被发送
        void close();
    private:
        bool handShakeWithHttpProxy();
        bool handShakeWithSock4Proxy();
        bool handShakeWithSock5Proxy();
    private:
        std::string            _host_name;
        int                    _port_number;
        StreamingSocket        _socket;
        static ProxyConfig    _proxy_config;
    };
    typedef std::shared_ptr<ProxySocket>    ProxySocketPtr;
}