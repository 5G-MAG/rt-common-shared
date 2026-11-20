#ifndef _HTTPXPP_PATH_DELEGATER_HTTP_REQUEST_HANDLER_HH_
#define _HTTPXPP_PATH_DELEGATER_HTTP_REQUEST_HANDLER_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: PathDelegatorHTTPRequestHandler class
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

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common.hh"
#include "HTTPRequestHandler.hh"

HTTPXPP_NAMESPACE_START

class HTTPServer;
class HTTPResponse;
class HTTPRequest;
class MimeTypeMap;

class PathDelegatorHTTPRequestHandler : public HTTPRequestHandler {
public:
    using path_type = std::string;
    using delegate_type = std::shared_ptr<HTTPRequestHandler>;
    using path_delegate_map = std::map<path_type, delegate_type>;
    using path_delegate_type = path_delegate_map::value_type;

    PathDelegatorHTTPRequestHandler();
    PathDelegatorHTTPRequestHandler(std::initializer_list<path_delegate_type> init_list);
    PathDelegatorHTTPRequestHandler(const PathDelegatorHTTPRequestHandler &other) = delete;
    PathDelegatorHTTPRequestHandler(PathDelegatorHTTPRequestHandler &&other) = delete;

    virtual ~PathDelegatorHTTPRequestHandler() {};

    PathDelegatorHTTPRequestHandler &operator=(const PathDelegatorHTTPRequestHandler &other) = delete;
    PathDelegatorHTTPRequestHandler &operator=(PathDelegatorHTTPRequestHandler &&other) = delete;

    virtual void serverShutdown(const HTTPServer &server);
    virtual HTTPResponse doRequest(const HTTPRequest &request, const HTTPServer &server);

    void setPathDelegate(const path_type &path, const delegate_type &delegate);
    void delPathDelegate(const path_type &path);
    bool hasPathDelegate(const path_type &path) const;

private:
    path_delegate_map m_delegatedPaths;
};

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _HTTPXPP_PATH_DELEGATER_HTTP_REQUEST_HANDLER_HH_ */
