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

#include <algorithm>
#include <list>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "common.hh"
#include "CaseInsensitiveTraits.hh"

#include "HTTPRequest.hh"

HTTPXPP_NAMESPACE_START

HTTPRequest::HTTPRequest(const std::string &url, const std::string &method, const std::string &version, std::map<std::string, std::list<std::string> > &&headers, std::vector<char> &&body)
    :m_url(url)
    ,m_method(method)
    ,m_version(version)
    ,m_headers(std::move(headers))
    ,m_body(std::move(body))
{
}

HTTPRequest::HTTPRequest(const HTTPRequest &other)
    :m_url(other.m_url)
    ,m_method(other.m_method)
    ,m_version(other.m_version)
    ,m_headers(other.m_headers)
    ,m_body(other.m_body)
{
}

HTTPRequest::HTTPRequest(HTTPRequest &&other)
    :m_url(std::move(other.m_url))
    ,m_method(std::move(other.m_method))
    ,m_version(std::move(other.m_version))
    ,m_headers(std::move(other.m_headers))
    ,m_body(std::move(other.m_body))
{
}

HTTPRequest &HTTPRequest::operator=(const HTTPRequest &other)
{
    m_url = other.m_url;
    m_method = other.m_method;
    m_version = other.m_version;
    m_headers = other.m_headers;
    m_body = other.m_body;
    return *this;
}

HTTPRequest &HTTPRequest::operator=(HTTPRequest &&other)
{
    m_url = std::move(other.m_url);
    m_method = std::move(other.m_method);
    m_version = std::move(other.m_version);
    m_headers = std::move(other.m_headers);
    m_body = std::move(other.m_body);
    return *this;
}

const std::list<std::string> &HTTPRequest::getHeader(const std::string &field) const
{
    std::basic_string_view<char, CaseInsensitiveTraits<char> > ci_field(field.data(), field.size());
    
    auto it = std::find_if(m_headers.begin(), m_headers.end(), [&ci_field](const decltype(m_headers)::value_type &kv) -> bool { std::basic_string_view<char, CaseInsensitiveTraits<char> > ci_key(kv.first.data(), kv.first.size()); return ci_key == ci_field; });
    if (it != m_headers.end()) return it->second;
    static const std::list<std::string> empty;
    return empty;
}

const std::optional<std::string> HTTPRequest::getHeaderFirst(const std::string &field) const
{
    auto values = getHeader(field);
    if (values.empty()) return std::nullopt;
    return values.front();
}

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
