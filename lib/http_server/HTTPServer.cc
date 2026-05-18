/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: HTTP Server class
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

#include <memory>
#include <mutex>

#include "common.hh"
#include "HTTPRequestHandler.hh"
#include "HTTPRequest.hh"
#include "HTTPResponse.hh"
#include "SockAddr.hh"
#include "version.h"

#include "HTTPServer.hh"

HTTPXPP_NAMESPACE_START

static MHD_Result _get_headers(void *cls, MHD_ValueKind kind, const char *key, const char *value);

HTTPServer::HTTPServer()
    :m_listenAddress()
    ,m_requestHandler(nullptr)
    ,m_mutex(new decltype(m_mutex)::element_type)
    ,m_mhd(nullptr)
    ,m_serverName()
{
}

HTTPServer::HTTPServer(const SockAddr &listen_address, const std::shared_ptr<HTTPRequestHandler> &request_handler)
    :m_listenAddress(listen_address)
    ,m_requestHandler(request_handler)
    ,m_mutex(new decltype(m_mutex)::element_type)
    ,m_mhd(nullptr)
    ,m_serverName()
{
    run();
}

HTTPServer::HTTPServer(HTTPServer &&other)
    :m_listenAddress(std::move(other.m_listenAddress))
    ,m_requestHandler(std::move(other.m_requestHandler))
    ,m_mutex(new decltype(m_mutex)::element_type)
    ,m_mhd(other.m_mhd)
    ,m_serverName(std::move(other.m_serverName))
{
    other.m_mhd = nullptr;
}

HTTPServer::~HTTPServer()
{
    if (m_requestHandler) {
        m_requestHandler->serverShutdown(*this);
    }
    close();
}

HTTPServer &HTTPServer::operator=(HTTPServer &&other)
{
    m_listenAddress = std::move(other.m_listenAddress);
    m_requestHandler = std::move(other.m_requestHandler);
    m_mhd = other.m_mhd;
    other.m_mhd = nullptr;

    return *this;
}

void HTTPServer::run()
{
    std::lock_guard<decltype(m_mutex)::element_type> lock(*m_mutex);
    if (!m_mhd) {
        m_mhd = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG | (m_listenAddress.isIPv6()?MHD_USE_IPv6:0), 0, nullptr, nullptr, HTTPServer::__accessHandlerCallback, this, MHD_OPTION_SOCK_ADDR, static_cast<const struct sockaddr*>(m_listenAddress), MHD_OPTION_END);
        if (m_listenAddress.port() == 0) {
            /* TODO: Get ephemeral port from MHD */
        }
    }
}

void HTTPServer::close()
{
    std::lock_guard<decltype(m_mutex)::element_type> lock(*m_mutex);
    if (m_mhd) {
        MHD_stop_daemon(m_mhd);
        m_mhd = nullptr;
    }
}

const std::string &HTTPServer::httpLibraryVersion()
{
    static const std::string httpx_version_string{"5G-MAG-HTTP-Server/" HTTPXPP_SERVER_VERSION};
    return httpx_version_string;
}

const std::string &HTTPServer::httpLibraryVersionComment()
{
    static const std::string httpx_version_comment_string{std::format("microhttpd/{}", MHD_get_version())};
    return httpx_version_comment_string;
}

MHD_Result HTTPServer::__accessHandlerCallback(void *cls, struct MHD_Connection *connection, const char *url, const char *method,
                                        const char *version, const char *upload_data, size_t *upload_data_size, void **req_cls)
{
    HTTPServer *http_server = reinterpret_cast<HTTPServer*>(cls);

    std::map<std::string, std::list<std::string> > req_headers;
    MHD_get_connection_values(connection, MHD_HEADER_KIND, _get_headers, &req_headers);

    HTTPRequest req(url, method, version, std::move(req_headers), std::vector<char>(upload_data, upload_data + *upload_data_size));

    auto response = http_server->m_requestHandler->doRequest(req, *http_server);
    struct MHD_Response *mhd_response = response.makeMHDResponse();
    MHD_Result ret = MHD_queue_response(connection, response.statusCode(), mhd_response);

    MHD_destroy_response(mhd_response);

    return ret;
}

static MHD_Result _get_headers(void *cls, MHD_ValueKind kind, const char *key, const char *value)
{
    auto &hdrs = *reinterpret_cast<std::map<std::string, std::list<std::string> >*>(cls);
    hdrs[key].emplace_back(value);
    return MHD_YES;
}

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
