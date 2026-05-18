#ifndef _HTTPXPP_HTTP_RESPONSE_HH_
#define _HTTPXPP_HTTP_RESPONSE_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: HTTPResponse class
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
#include <microhttpd.h>

#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "common.hh"

HTTPXPP_NAMESPACE_START

class HTTPServer;

class HTTPResponse {
public:
    HTTPResponse();

    /* In memory response constructors */
    HTTPResponse(const std::vector<char> &body);
    HTTPResponse(std::vector<char> &&body);
    HTTPResponse(std::istringstream &&stream);

    /* On disk response constructors */
    HTTPResponse(const std::string &filepath);
    HTTPResponse(int fd);
    HTTPResponse(std::ifstream &&strm);

    HTTPResponse(const HTTPResponse &other);
    HTTPResponse(HTTPResponse &&other);

    virtual ~HTTPResponse() {};

    HTTPResponse &operator=(const HTTPResponse &other);
    HTTPResponse &operator=(HTTPResponse &&other);

    unsigned int statusCode() const { return m_statusCode; };
    HTTPResponse &statusCode(unsigned int status_code) { m_statusCode = status_code; return *this; };

    HTTPResponse &body(const std::vector<char> &body);
    HTTPResponse &body(std::vector<char> &&body);
    HTTPResponse &body(std::istringstream &&stream);
    HTTPResponse &body(const std::string &filepath);
    HTTPResponse &body(std::ifstream &&file_stream);
    HTTPResponse &body(int fd);

    HTTPResponse &setHeader(const std::string &field, const std::string &value);
    HTTPResponse &addHeader(const std::string &field, const std::string &value);
    HTTPResponse &removeHeader(const std::string &field);
    const std::optional<std::string> getHeader(const std::string &field) const;

protected:
    friend class HTTPServer;
    MHD_Response *makeMHDResponse() const;

private:
    static ssize_t __contentReaderCallback(void *cls, uint64_t pos, char *buf, size_t max);
    void __addDefaultHeaders();

    unsigned int m_statusCode;
    std::vector<char> m_body;
    int m_bodyFD;
    std::shared_ptr<std::ifstream> m_bodyFileStream;
    std::shared_ptr<std::istringstream> m_bodyStringStream;
    std::map<std::string, std::string> m_headers;
};

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _HTTPXPP_HTTP_RESPONSE_HH_ */
