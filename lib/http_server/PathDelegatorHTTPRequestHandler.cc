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

#include <format>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common.hh"
#include "HTTPServer.hh"
#include "HTTPRequest.hh"
#include "HTTPRequestHandler.hh"
#include "HTTPResponse.hh"

#include "PathDelegatorHTTPRequestHandler.hh"

HTTPXPP_NAMESPACE_START

PathDelegatorHTTPRequestHandler::PathDelegatorHTTPRequestHandler()
    :m_delegatedPaths()
{
}

PathDelegatorHTTPRequestHandler::PathDelegatorHTTPRequestHandler(std::initializer_list<path_delegate_type> init_list)
    :m_delegatedPaths(init_list)
{
}

void PathDelegatorHTTPRequestHandler::serverShutdown(const HTTPServer &server)
{
    for (auto &[path, delegate] : m_delegatedPaths) {
        delegate->serverShutdown(server);
    }
}

HTTPResponse PathDelegatorHTTPRequestHandler::doRequest(const HTTPRequest &request, const HTTPServer &server)
{
    for (auto &[path, delegate] : m_delegatedPaths) {
        if (request.url().starts_with(path)) {
            auto subpath = request.url().substr(path.size());
            if (!subpath.starts_with('/')) subpath.insert(0, "/");
            HTTPRequest subpath_req{request};
            subpath_req.url(subpath);
            auto resp = delegate->doRequest(subpath_req, server);
            /* If redirection response, adjust Location header if needed */
            if (resp.statusCode() == 301 || resp.statusCode() == 302 || resp.statusCode() == 307 || resp.statusCode() == 308) {
                auto locn = resp.getHeader("Location");
                if (locn && locn.value().starts_with('/')) {
                    resp.setHeader("Location", std::format("{}{}{}", path, path.ends_with('/')?"":"/", locn.value().substr(1)));
                }
            }
            return resp;
        }
    }

    HTTPResponse resp = server.makeResponse().statusCode(404);
    return resp;
}

void PathDelegatorHTTPRequestHandler::setPathDelegate(const path_type &path, const delegate_type &delegate)
{
    m_delegatedPaths[path] = delegate;
}

void PathDelegatorHTTPRequestHandler::delPathDelegate(const path_type &path)
{
    auto it = m_delegatedPaths.find(path);
    if (it != m_delegatedPaths.end()) m_delegatedPaths.erase(it);
}

bool PathDelegatorHTTPRequestHandler::hasPathDelegate(const path_type &path) const
{
    auto it = m_delegatedPaths.find(path);
    return (it != m_delegatedPaths.end());
}

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
