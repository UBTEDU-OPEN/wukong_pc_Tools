#include "httprequest.h"
#include <algorithm>
#include <iostream>
#include "stlhelper.hpp"


void HttpRequest::set_method(const char* start, const char* end)
{
    string m(start, end);
    auto it = str_to_method_map.find(m);
    if (it != str_to_method_map.end())
        _method = it->second;
    else
        _method = HttpMethod::unknown;
}

void HttpRequest::set_method(const std::string & m)
{
    auto it = str_to_method_map.find(m);
    if (it != str_to_method_map.end())
        _method = it->second;
    else
        _method = HttpMethod::unknown;
}

HttpBase::HttpMethod HttpRequest::get_method()  const
{
    return _method;
}


void HttpRequest::set_url(const string &url)
{
    _url = url;
}


string HttpRequest::get_url()  const
{
    return _url;
}

void HttpRequest::set_version(const char* start, const char* end)
{
    string v(start, end);
    auto it = str_to_version_map.find(v);
    if (it != str_to_version_map.end())
        _version = it->second;
    else
        _version = HttpVersion::unknown;
}

HttpBase::HttpVersion HttpRequest::get_version()  const
{
    return _version;
}

void HttpRequest::set_path(const char* start, const char* end)
{
    _path.assign(start, end);
}

string HttpRequest::get_path()  const
{
    return _path;
}

void HttpRequest::set_query(const char* start, const char* end)
{
    _query.assign(start, end);
}

string HttpRequest::get_query() const
{
    return _query;
}

void HttpRequest::add_header(const char* start, const char* end)
{
    const char* p = std::find(start, end, ':');
    string key(start, p);
    
    p++;
    while (p < end && isspace(*p))
        p++;
    const char* e = end;
    while (e > p && isspace(*e))
        e--;
    
    string value(p, e+1);
    _headers[key] = value;
}

string HttpRequest::get_header(const string& k)
{
    string result;
    auto it = _headers.find(k);
    if (it != _headers.end())
        result = it->second;
    return result;
}

void HttpRequest::append_to_body(const string& b)
{
    _body.append(b);
}

void HttpRequest::append_to_body(const char* b, const char* e)
{
    _body.append(b, e);
}

const string& HttpRequest::body() const
{
    return _body;
}

/**************************************************************************
* 函数名:   append_body
* 功能:
* 参数:
*....@[in]  const string & name
*....@[in]  const string value
*....@[out] void
* 返回值:
* 时间:     2017/10/30 17:22
* 作者:   Leo
*/
void HttpRequest::append_body(const string &name, const string &value)
{
    if (!utils::contain(_body_map, name)) {
        _body_map.insert(std::make_pair(name, value));
    }
    else {

    }
}

/**************************************************************************
* 函数名:   body_map
* 功能:
* 参数:
*....@[out] const std::map<string, string> &
* 返回值:
* 时间:     2017/10/30 17:21
* 作者:   Leo
*/
const std::map<string, string> & HttpRequest::get_body_map() const
{
    return _body_map;
}

string HttpRequest::str_method()
{
    return method_to_str_map[_method];
}

string HttpRequest::str_version()
{
    return version_to_str_map[_version];
}

void HttpRequest::print(std::ostream& os)
{
    os << ">" << method_to_str_map[_method];
    os << " " << _path;
    if (!_query.empty())
        os << "?" << _query;
    os << " ";
    
    os << str_version();
    os << "\r\n";
    
    for (auto& header : _headers)
    {
        os << ">";
        os << header.first << ": " << header.second << "\r\n";
    }
    os << "\r\n";
    os << ">" << _body << std::endl;
}
