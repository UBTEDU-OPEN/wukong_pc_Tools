#ifndef _HTTP_CLIENT_
#define _HTTP_CLIENT_

#pragma once

#include "ubxhttpclientdef.h"
#include "singleton.h"
#include <string>
#include "ubxhttp"
#include <iostream>
#include <functional>
#include <cstdlib>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

//typedef curlpp::options::ReadFunction _CALLBACK_CURLPP_DEBUGFUNC;
//typedef curlpp::options::DebugFunction _CALLBACK_CURLPP_DEBUGFUNC;
typedef curlpp::options::WriteFunction _CALLBACK_CURLPP_POST;
typedef curlpp::options::ProgressFunction _CALLBACK_CURLPP_PROGRESS;
using namespace utils;
using namespace std;

class HttpResponse;
class HttpRequest;
class UBXHTTPCLIENT_EXPORTS HttpClient : public SingletonT<HttpClient>
{
public:
    HttpClient();
    virtual ~HttpClient();
public:
    static size_t writeCallback(char* ptr, size_t size, size_t nmemb, void *f);
    void postRequest( const HttpRequest &request, _CALLBACK_CURLPP_POST _func);
    void downloadRequest(const HttpRequest &request, std::string fullPath, _CALLBACK_CURLPP_PROGRESS _func);
    void asyncDownloadRequest(const HttpRequest &request, std::string fullPath, _CALLBACK_CURLPP_PROGRESS _func);
};

#endif

