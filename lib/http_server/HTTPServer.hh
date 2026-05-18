#ifndef _HTTPXPP_HTTP_SERVER_HH_
#define _HTTPXPP_HTTP_SERVER_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: HTTPServer class
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
#include "SockAddr.hh"
#include "HTTPResponse.hh"

HTTPXPP_NAMESPACE_START

class HTTPRequestHandler;

class HTTPServer {
public:
    HTTPServer();
    HTTPServer(const SockAddr &listen_address, const std::shared_ptr<HTTPRequestHandler> &request_handler);
    HTTPServer(HTTPServer &&other);
    HTTPServer(const HTTPServer &other) = delete;

    virtual ~HTTPServer();

    HTTPServer &operator=(HTTPServer &&other);
    HTTPServer &operator=(const HTTPServer &other) = delete;

    void run();
    void close();

    const SockAddr &listenAddress() const { return m_listenAddress; };

    const std::string &serverName() const { return m_serverName; };
    HTTPServer &serverName(const std::string &name) { m_serverName = name; return *this; };

    template <class... Args>
    HTTPResponse makeResponse(Args&&... args) const { 
        HTTPResponse resp{std::forward<Args>(args)...};
        if (!m_serverName.empty()) resp.setHeader("Server", m_serverName);
        return resp;
    };

    static const std::string &httpLibraryVersion();
    static const std::string &httpLibraryVersionComment();

private:
    static MHD_Result __accessHandlerCallback(void *cls, struct MHD_Connection *connection, const char *url, const char *method,
                                       const char *version, const char *upload_data, size_t *upload_data_size, void **req_cls);

    SockAddr m_listenAddress;
    std::shared_ptr<HTTPRequestHandler> m_requestHandler;
    std::shared_ptr<std::recursive_mutex> m_mutex;
    MHD_Daemon *m_mhd;
    std::string m_serverName;
};

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _HTTPXPP_HTTP_SERVER_HH_ */
