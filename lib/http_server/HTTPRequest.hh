#ifndef _HTTPXPP_HTTP_REQUEST_HH_
#define _HTTPXPP_HTTP_REQUEST_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: HTTPRequest class
 ******************************************************************************
 * Copyright: (C)2026 British Broadcasting Corporation
 * Author(s): David Waring <david.waring2@bbc.co.uk>
 * License: 5G-MAG Public License v1
 *
 * Licensed under the License terms and conditions for use, reproduction, and
 * distribution of 5G-MAG software (the “License”).  You may not use this file
 * except in compliance with the License.  You may obtain a copy of the License at
 * https://www.5g-mag.com/reference-tools.  Unless required by applicable law or
 * agreed to in writing, software distributed under the License is distributed on
 * an “AS IS” BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied.
 *
 * See the License for the specific language governing permissions and limitations
 * under the License.
 */

#include <list>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "common.hh"

HTTPXPP_NAMESPACE_START

class HTTPServer;
class HTTPResponse;

class HTTPRequest {
public:
    HTTPRequest() = delete;
    HTTPRequest(const std::string &url, const std::string &method, const std::string &version, std::map<std::string, std::list<std::string> > &&headers, std::vector<char> &&body);
    HTTPRequest(const HTTPRequest &other);
    HTTPRequest(HTTPRequest &&other);

    virtual ~HTTPRequest() {};

    HTTPRequest &operator=(const HTTPRequest &other);
    HTTPRequest &operator=(HTTPRequest &&other);

    const std::string &url() const { return m_url; };
    const std::string &method() const { return m_method; };
    const std::string &version() const { return m_version; };
    const std::map<std::string, std::list<std::string> > &headers() const { return m_headers; };
    const std::vector<char> &body() const { return m_body; };

    HTTPRequest &url(const std::string &new_url) { m_url = new_url; return *this; };

    const std::list<std::string> &getHeader(const std::string &field) const;
    const std::optional<std::string> getHeaderFirst(const std::string &field) const;

private:
    std::string m_url;
    std::string m_method;
    std::string m_version;
    std::map<std::string, std::list<std::string> > m_headers;
    std::vector<char> m_body;
};

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _HTTPXPP_HTTP_REQUEST_HH_ */
