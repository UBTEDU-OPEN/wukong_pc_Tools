#ifndef _HTTPREQUEST_H
#define _HTTPREQUEST_H

#include "ubxhttpclientdef.h"
#include "HttpBase.h"
#include <string>
#include <map>
#include <functional>
#include "httpresponse.h"

class UBXHTTPCLIENT_EXPORTS HttpRequest : public HttpBase
{
    public:
        HttpRequest():
            _method(HttpMethod::unknown),
            _version(HttpVersion::unknown),
            _path(),
            _query(),
            _headers(),
            _body()
        {
        }
        
        HttpRequest(HttpMethod m, HttpVersion v):
            _method(m),
            _version(v),
            _path(),
            _query(),
            _headers()
        {
        }
        
        HttpRequest(HttpMethod m, HttpVersion v, 
            const string& p, 
            const string& q):
            _method(m),
            _version(v),
            _path(p),
            _query(q),
            _headers()
        {
        }
        
        void set_method(const char* start, const char* end);
        void set_method(const std::string & m);
        HttpMethod get_method() const;
        
        void set_url(const string &url);
        string get_url() const;

        void set_version(const char* start, const char* end);
        HttpVersion get_version() const;
        
        void set_path(const char* start, const char* end);
        string get_path() const;
        
        void set_query(const char* start, const char* end);
        string get_query() const;
        
        void add_header(const char* start, const char* end);
        string get_header(const string& k);
        
        void append_to_body(const string& b);
        void append_to_body(const char* b, const char* e);
        
        const string &body() const;

        void append_body(const string &name, const string &value);
        const std::map<string, string> &get_body_map() const;

        string str_method();
        string str_version();
        
        void print(std::ostream& os);
        
    private:
        HttpMethod _method;
        HttpVersion _version;
        std::string _url;
        std::string _path;
        std::string _query;
        std::map<string, string> _headers;
        std::map<string, string> _body_map;
        std::string _body;
};

#endif

