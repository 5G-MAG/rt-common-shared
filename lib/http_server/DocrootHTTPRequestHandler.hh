#ifndef _HTTPXPP_DOCROOT_HTTP_REQUEST_HANDLER_HH_
#define _HTTPXPP_DOCROOT_HTTP_REQUEST_HANDLER_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: DocrootHTTPRequestHandler class
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
class MimeTypeMap;

class DocrootHTTPRequestHandler : public HTTPRequestHandler {
public:
    class IndexHandler {
    public:
        IndexHandler() {};

        IndexHandler(const IndexHandler &) = delete;
        IndexHandler(IndexHandler &&) = delete;

        virtual ~IndexHandler() {};

        IndexHandler &operator=(const IndexHandler &) = delete;
        IndexHandler &operator=(IndexHandler &&) = delete;

        virtual HTTPResponse makeResponseForDir(const std::string &dir_path, const std::string &url_path, const HTTPServer &server) = 0;
    };

    DocrootHTTPRequestHandler() = delete;
    DocrootHTTPRequestHandler(const std::string &docroot_dir, const std::shared_ptr<IndexHandler> &index_handler = nullptr);
    DocrootHTTPRequestHandler(const DocrootHTTPRequestHandler &other) = delete;
    DocrootHTTPRequestHandler(DocrootHTTPRequestHandler &&other) = delete;

    virtual ~DocrootHTTPRequestHandler() {};

    DocrootHTTPRequestHandler &operator=(const DocrootHTTPRequestHandler &other) = delete;
    DocrootHTTPRequestHandler &operator=(DocrootHTTPRequestHandler &&other) = delete;

    DocrootHTTPRequestHandler &docroot(const std::string &docroot);
    const std::string &docroot() const { return m_docroot; };

    DocrootHTTPRequestHandler &indexHandler(const std::shared_ptr<IndexHandler> &index_handler);
    const std::shared_ptr<IndexHandler> &indexHandler() const { return m_indexHandler; };

    DocrootHTTPRequestHandler &addMimeType(const std::string &ext, const std::string &mime_type);

    virtual void serverShutdown(const HTTPServer &server);
    virtual HTTPResponse doRequest(const HTTPRequest &request, const HTTPServer &server);

private:
    std::string m_docroot;
    std::shared_ptr<IndexHandler> m_indexHandler;
    std::shared_ptr<MimeTypeMap> m_mimeTypeMap;
};

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _HTTPXPP_DOCROOT_HTTP_REQUEST_HANDLER_HH_ */
