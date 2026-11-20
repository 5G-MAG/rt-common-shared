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

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE     /* See feature_test_macros(7) */
#endif
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include <format>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "common.hh"
#include "CaseInsensitiveTraits.hh"
#include "HTTPServer.hh"
#include "version.h"

#include "HTTPResponse.hh"

HTTPXPP_NAMESPACE_START

HTTPResponse::HTTPResponse()
    :m_statusCode(200)
    ,m_body()
    ,m_bodyFD(-1)
    ,m_bodyFileStream()
    ,m_bodyStringStream()
    ,m_headers()
{
    __addDefaultHeaders();
}


HTTPResponse::HTTPResponse(const std::vector<char> &body)
    :m_statusCode(200)
    ,m_body(body)
    ,m_bodyFD(-1)
    ,m_bodyFileStream()
    ,m_bodyStringStream()
    ,m_headers()
{
    __addDefaultHeaders();
}

HTTPResponse::HTTPResponse(std::vector<char> &&body)
    :m_statusCode(200)
    ,m_body(std::move(body))
    ,m_bodyFD(-1)
    ,m_bodyFileStream()
    ,m_bodyStringStream()
    ,m_headers()
{
    __addDefaultHeaders();
}

HTTPResponse::HTTPResponse(std::istringstream &&stream)
    :m_statusCode(200)
    ,m_body()
    ,m_bodyFD(-1)
    ,m_bodyFileStream()
    ,m_bodyStringStream(new std::istringstream(std::move(stream)))
    ,m_headers()
{
    __addDefaultHeaders();
}

HTTPResponse::HTTPResponse(const std::string &filepath)
    :m_statusCode(200)
    ,m_body()
    ,m_bodyFD(open(filepath.c_str(), O_RDONLY))
    ,m_bodyFileStream()
    ,m_bodyStringStream()
    ,m_headers()
{
    __addDefaultHeaders();
}

HTTPResponse::HTTPResponse(int fd)
    :m_statusCode(200)
    ,m_body()
    ,m_bodyFD(fd)
    ,m_bodyFileStream()
    ,m_bodyStringStream()
    ,m_headers()
{
    __addDefaultHeaders();
}

HTTPResponse::HTTPResponse(std::ifstream &&strm)
    :m_statusCode(200)
    ,m_body()
    ,m_bodyFD()
    ,m_bodyFileStream(new std::ifstream(std::move(strm)))
    ,m_bodyStringStream()
    ,m_headers()
{
    __addDefaultHeaders();
}

HTTPResponse::HTTPResponse(const HTTPResponse &other)
    :m_statusCode(other.m_statusCode)
    ,m_body(other.m_body)
    ,m_bodyFD(-1)
    ,m_bodyFileStream(other.m_bodyFileStream)
    ,m_bodyStringStream(other.m_bodyStringStream)
    ,m_headers(other.m_headers)
{
    if (other.m_bodyFD >= 0) {
        m_bodyFD = dup(other.m_bodyFD);
    }
}

HTTPResponse::HTTPResponse(HTTPResponse &&other)
    :m_statusCode(other.m_statusCode)
    ,m_body(std::move(other.m_body))
    ,m_bodyFD(other.m_bodyFD)
    ,m_bodyFileStream(std::move(other.m_bodyFileStream))
    ,m_bodyStringStream(std::move(other.m_bodyStringStream))
    ,m_headers(std::move(other.m_headers))
{
    other.m_bodyFD = -1;
    other.m_statusCode = 200;
}

HTTPResponse &HTTPResponse::operator=(const HTTPResponse &other)
{
    m_statusCode = other.m_statusCode;
    m_body = other.m_body;
    if (m_bodyFD >= 0) close(m_bodyFD);
    if (other.m_bodyFD >= 0) {
        m_bodyFD = dup(other.m_bodyFD);
    } else {
        m_bodyFD = -1;
    }
    m_bodyFileStream = other.m_bodyFileStream;
    m_bodyStringStream = other.m_bodyStringStream;
    m_headers = other.m_headers;

    return *this;
}

HTTPResponse &HTTPResponse::operator=(HTTPResponse &&other)
{
    m_statusCode = other.m_statusCode;
    other.m_statusCode = 200;
    m_body = std::move(other.m_body);
    if (m_bodyFD >= 0) close(m_bodyFD);
    m_bodyFD = other.m_bodyFD;
    other.m_bodyFD = -1;
    m_bodyFileStream = std::move(other.m_bodyFileStream);
    m_bodyStringStream = std::move(other.m_bodyStringStream);
    m_headers = std::move(other.m_headers);

    return *this;
}

HTTPResponse &HTTPResponse::body(const std::vector<char> &body)
{
    m_body = body;
    if (m_bodyFD >= 0) {
        close(m_bodyFD);
        m_bodyFD = -1;
    }
    m_bodyFileStream.reset();
    m_bodyStringStream.reset();
    return *this;
}

HTTPResponse &HTTPResponse::body(std::vector<char> &&body)
{
    m_body = std::move(body);
    if (m_bodyFD >= 0) {
        close(m_bodyFD);
        m_bodyFD = -1;
    }
    m_bodyFileStream.reset();
    m_bodyStringStream.reset();
    return *this;
}

HTTPResponse &HTTPResponse::body(std::istringstream &&stream)
{
    m_body.clear();
    if (m_bodyFD >= 0) {
        close(m_bodyFD);
        m_bodyFD = -1;
    }
    m_bodyFileStream.reset();
    m_bodyStringStream.reset(new std::istringstream(std::move(stream)));
    return *this;
}

HTTPResponse &HTTPResponse::body(const std::string &filepath)
{
    m_body.clear();
    if (m_bodyFD >= 0) close(m_bodyFD);
    m_bodyFD = open(filepath.c_str(), O_RDONLY);
    m_bodyFileStream.reset();
    m_bodyStringStream.reset();
    return *this;
}

HTTPResponse &HTTPResponse::body(std::ifstream &&file_stream)
{
    m_body.clear();
    if (m_bodyFD >= 0) {
        close(m_bodyFD);
        m_bodyFD = -1;
    }
    m_bodyFileStream.reset(new std::ifstream(std::move(file_stream)));
    m_bodyStringStream.reset();
    return *this;
}

HTTPResponse &HTTPResponse::body(int fd)
{
    m_body.clear();
    if (m_bodyFD >= 0) close(m_bodyFD);
    m_bodyFD = fd;
    m_bodyFileStream.reset();
    m_bodyStringStream.reset();
    return *this;
}

HTTPResponse &HTTPResponse::setHeader(const std::string &field, const std::string &value)
{
    m_headers[field] = value;
    return *this;
}

HTTPResponse &HTTPResponse::addHeader(const std::string &field, const std::string &value)
{
    auto it = m_headers.find(field);
    if (it == m_headers.end()) {
        m_headers.insert(std::make_pair(field,value));
    } else {
        it->second += ", ";
        it->second += value;
    }
    return *this;
}

HTTPResponse &HTTPResponse::removeHeader(const std::string &field)
{
    auto it = m_headers.find(field);
    if (it != m_headers.end()) m_headers.erase(it);
    return *this;
}

const std::optional<std::string> HTTPResponse::getHeader(const std::string &field) const
{
    std::basic_string_view<char, CaseInsensitiveTraits<char> > ci_field(field.data(), field.size());
    auto it = std::find_if(m_headers.begin(), m_headers.end(), [&ci_field](const decltype(m_headers)::value_type &kv) -> bool {
        std::basic_string_view<char, CaseInsensitiveTraits<char> > ci_key(kv.first.data(), kv.first.size());
        return ci_key == ci_field;
    });
    if (it != m_headers.end()) return it->second;
    static const std::optional<std::string> empty;
    return empty;
}

// protected:
MHD_Response *HTTPResponse::makeMHDResponse() const
{
    MHD_Response *resp = nullptr;

    if (m_bodyFD >= 0) {
        auto curr_offset = lseek64(m_bodyFD, 0, SEEK_CUR);
        uint64_t file_size = lseek64(m_bodyFD, 0, SEEK_END);
        lseek64(m_bodyFD, curr_offset, SEEK_SET);
        resp = MHD_create_response_from_fd(file_size - curr_offset, m_bodyFD);
    } else if (m_bodyFileStream) {
        // use callback to feed bits of the stream
        resp = MHD_create_response_from_callback(-1, 8192, HTTPResponse::__contentReaderCallback, m_bodyFileStream.get(), nullptr);
    } else if (m_bodyStringStream) {
        // use callback to feed bits of the stream
        resp = MHD_create_response_from_callback(-1, 8192, HTTPResponse::__contentReaderCallback, m_bodyStringStream.get(), nullptr);
    } else {
        resp = MHD_create_response_from_buffer(m_body.size(), reinterpret_cast<void*>(const_cast<char*>(m_body.data())), MHD_RESPMEM_MUST_COPY);
    }

    for (const auto &[field, value] : m_headers) {
        MHD_add_response_header(resp, field.c_str(), value.c_str());
    }

    return resp;
}

// private:

ssize_t HTTPResponse::__contentReaderCallback(void *cls, uint64_t pos, char *buf, size_t max)
{
    std::istream *strm = reinterpret_cast<std::istream*>(cls);
    ssize_t bytes_read = strm->readsome(buf, max);
    if (bytes_read == 0 && strm->eof()) return -1;
    return bytes_read;
}

void HTTPResponse::__addDefaultHeaders()
{
    auto server_version = std::format("{} ({})", HTTPServer::httpLibraryVersion(), HTTPServer::httpLibraryVersionComment());
    addHeader("Server", server_version);
}

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
