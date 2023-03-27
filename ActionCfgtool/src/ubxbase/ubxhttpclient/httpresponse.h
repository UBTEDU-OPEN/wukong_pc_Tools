#ifndef _HTTPRESPONSE_H
#define _HTTPRESPONSE_H

#include "ubxhttpclientdef.h"
#include "HttpBase.h"
#include <string>
#include <map>

using std::string;

class UBXHTTPCLIENT_EXPORTS HttpResponse : public HttpBase
{
    public:
        HttpResponse():
            _version(HttpVersion::v11),
            _status_code(HttpStatusCode::unknown),
            _headers(),
            _body()
        {
        }
        
        HttpResponse(int status_code, const string& body):
            _version(HttpVersion::v11),
            _status_code(static_cast<HttpStatusCode>(status_code)),
            _headers(),
            _body(body)
        {
        }
        
        void set_status_code(int code);
        string get_version();
        
        void append_to_body(const std::string& b);
        void append_to_body(const char* b, const char* e);

        void set_body(const std::string& body);
        const string& get_body();
        
        void add_header(const std::string& k, const std::string& v);
        
        // 支持HTTP的HEAD方法
        string to_string_without_body();
        string to_string();
        
        void print(std::ostream& os);
        
    private:
        HttpVersion _version;
        HttpStatusCode _status_code;
        std::map<std::string, std::string> _headers;
        std::string _body;
};

#endif
