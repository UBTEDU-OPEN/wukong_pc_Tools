#include "singleton.hpp"
#include "httpclient.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include "httprequest.h"
#include "httpresponse.h"
#include "log4z.h"
#include <functional>
#include <thread>
IMPL_SINGLETON_CLASS(HttpClient);

HttpClient::HttpClient()
{
}


HttpClient::~HttpClient()
{
}

void HttpClient::postRequest(const HttpRequest &request, _CALLBACK_CURLPP_POST _func)
{
    const string &url = request.get_url();
    if (url.empty()) {
        LOGFMT_INFO(0, "请求url为空");
        return;
    }
    curlpp::Cleanup cleaner;
    curlpp::Easy requester;
    if (request.get_method() == HttpBase::HttpMethod::post) {
        try {
            const map<string, string> &body = request.get_body_map();
            requester.setOpt(new curlpp::options::Url(url));
            requester.setOpt(new curlpp::options::Verbose(true));
            std::list<std::string> header;
            requester.setOpt(new curlpp::options::HttpHeader(header));
            curlpp::Forms formParts;
            for (auto it = body.begin(); it != body.end(); ++it) {
                string  key = it->first;
                string value = it->second;
                if (!key.empty() && !value.empty()) {
                    formParts.push_back(new curlpp::FormParts::Content(key.c_str(), value.c_str()));
                }
            }

            requester.setOpt(new curlpp::options::HttpPost(formParts));
            requester.setOpt(_func);
            requester.perform();
        }
        catch (curlpp::LogicError & e) {
            LOGFMT_INFO(0, e.what());
        }
        catch (curlpp::RuntimeError & e) {
            LOGFMT_INFO(0, e.what());
        }
    }
}

size_t HttpClient::writeCallback(char* ptr, size_t size, size_t nmemb, void *f)
{
    FILE *file = (FILE *)f;
    return fwrite(ptr, size, nmemb, file);
};

void HttpClient::downloadRequest(const HttpRequest &request, std::string fullPath, _CALLBACK_CURLPP_PROGRESS _func)
{
    try
    {
        curlpp::Cleanup cleaner;
        curlpp::Easy requester;
        /// Set the writer callback to enable cURL to write result in a memory area
        //curlpp::options::WriteFunctionCurlFunction writeFunction(std::bind(&HttpClient::writeCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        curlpp::options::WriteFunctionCurlFunction writeFunction(HttpClient::writeCallback);
        FILE *file = stdout;
        if (!fullPath.empty()) {
            file = fopen(fullPath.c_str(), "wb");
            if (file == NULL) {
                fprintf(stderr, "%s/n", strerror(errno));
                return;
            }
        }
        curlpp::OptionTrait<void *, CURLOPT_WRITEDATA>
            myData(file);
        requester.setOpt(writeFunction);
        requester.setOpt(myData);
        /// Setting the URL to retrive.
        requester.setOpt(new curlpp::options::Url(request.get_url()));
        requester.setOpt(new curlpp::options::Verbose(true));
        requester.setOpt(new curlpp::options::NoProgress(0));
        requester.setOpt(_func);
        requester.perform();
        fclose(file);
    }
    catch (curlpp::LogicError & e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (curlpp::RuntimeError & e)
    {
        std::cout << e.what() << std::endl;
    }
}

/**************************************************************************
* 函数名:   asyncDownloadRequest
* 功能:
* 参数:
*....@[in]  const HttpRequest & request
*....@[in]  std::string fullPath
*....@[in]  _CALLBACK_CURLPP_PROGRESS _func
*....@[out] void
* 返回值:
* 时间:     2017/11/08 19:58
* 作者:   Leo
*/
void HttpClient::asyncDownloadRequest(const HttpRequest &request, std::string fullPath, _CALLBACK_CURLPP_PROGRESS _func)
{
    std::thread t1(&HttpClient::downloadRequest, this, request, fullPath, _func);
    t1.detach();
}
