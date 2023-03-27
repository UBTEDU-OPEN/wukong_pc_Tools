#pragma once
#include <string>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <locale>
#include <cctype>
#include <sys/stat.h>
#include "globaldefs.h"

#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#endif

namespace HttpSocket {

#define PTR_BOOL(p)        {assert(p); if(!p){return false;}}
#define PTR_VOID(p)        {assert(p); if(!p){return;}}
#define SAFE_DELETE(p)    {if(p) {delete p; p = 0;}}

    const int MAX_FILE_BUFFER_SIZE = 10240;    //上传下载文件的buffer的大小
    const int MAX_BUFFER_SIZE = 1024;    //普通传输的buffer大小
    const int MAX_HTTP_POOL_THREADS_NUM = 5;

    //代理类型
    enum  ProxyType
    {
        PROXY_NULL,
        PROXY_HTTP,
        PROXY_SOCK4,
        PROXY_SOCK5,
    };

    //代理配置项
    struct  ProxyConfig
    {
        ProxyConfig()
            :_proxy_type(PROXY_NULL)
            , _port_number(0)
        {
        }

        ProxyConfig(ProxyType proxy_type, const std::string& host_name, int port_number,
            const std::string& username = "", const std::string& password = "")
            :_proxy_type(proxy_type),
            _host_name(host_name),
            _port_number(port_number),
            _username(username),
            _password(password)
        {

        }

        void copy(const ProxyConfig& proxy_config)
        {
            _proxy_type = proxy_config._proxy_type;
            _host_name = proxy_config._host_name;
            _port_number = proxy_config._port_number;
            _username = proxy_config._username;
            _password = proxy_config._password;
        }

        ProxyType        _proxy_type;
        std::string        _host_name;
        int                _port_number;
        std::string        _username;
        std::string        _password;
    };
}