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

#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <format>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "common.hh"
#include "DocrootFile.hh"
#include "HTTPRequest.hh"
#include "HTTPRequestHandler.hh"
#include "HTTPResponse.hh"
#include "HTTPServer.hh"
#include "MimeTypeMap.hh"

#include "DocrootHTTPRequestHandler.hh"

using namespace std::literals::string_view_literals;

HTTPXPP_NAMESPACE_START

static std::string resolve_path(const std::string &path);

DocrootHTTPRequestHandler::DocrootHTTPRequestHandler(const std::string &docroot_dir, const std::shared_ptr<IndexHandler> &index_handler)
    :m_docroot(resolve_path(docroot_dir))
    ,m_indexHandler(index_handler)
    ,m_mimeTypeMap(new MimeTypeMap(*MimeTypeMap::defaultMap))
{
}

DocrootHTTPRequestHandler &DocrootHTTPRequestHandler::docroot(const std::string &docroot)
{
    m_docroot = resolve_path(docroot);
    return *this;
}

DocrootHTTPRequestHandler &DocrootHTTPRequestHandler::indexHandler(const std::shared_ptr<IndexHandler> &index_handler)
{
    m_indexHandler = index_handler;
    return *this;
}

DocrootHTTPRequestHandler &DocrootHTTPRequestHandler::addMimeType(const std::string &ext, const std::string &mime_type)
{
    m_mimeTypeMap->setMimeType(ext, mime_type);
    return *this;
}

void DocrootHTTPRequestHandler::serverShutdown(const HTTPServer &server)
{
}

HTTPResponse DocrootHTTPRequestHandler::doRequest(const HTTPRequest &request, const HTTPServer &server)
{
    if (!request.url().starts_with('/')) {
        return server.makeResponse(std::vector<char>{'B', 'a', 'd', ' ', 'r', 'e', 'q', 'u', 'e', 's', 't', '\n'}).statusCode(400);
    }
    std::string file_path;
    try {
        file_path = resolve_path(m_docroot + request.url());
    } catch (std::error_condition &ex) {
        return server.makeResponse(std::vector<char>{'N', 'o', 't', ' ', 'f', 'o', 'u', 'n', 'd', '\n'}).statusCode(404);
    }
    if (!file_path.starts_with(m_docroot)) {
        return server.makeResponse(std::vector<char>{'N', 'o', 't', ' ', 'f', 'o', 'u', 'n', 'd', '\n'}).statusCode(404);
    }
    struct stat s;
    if (lstat(file_path.c_str(), &s) == -1) {
        return server.makeResponse(std::vector<char>{'F', 'o', 'r', 'b', 'i', 'd', 'd', 'e', 'n', '\n'}).statusCode(403);
    }
    if (s.st_mode & S_IFDIR) {
        if (!request.url().ends_with('/')) return server.makeResponse().statusCode(308).setHeader("Location", request.url() + "/");
        if (m_indexHandler) return m_indexHandler->makeResponseForDir(file_path, request.url(), server);
        return server.makeResponse(std::vector<char>{'F', 'o', 'r', 'b', 'i', 'd', 'd', 'e', 'n', '\n'}).statusCode(403);
    }

    // serve file
    auto if_none_match = request.getHeaderFirst("If-None-Match");
    auto if_modified_since = request.getHeaderFirst("If-Modified-Since");
    DocrootFile infile(file_path, std::const_pointer_cast<const MimeTypeMap>(m_mimeTypeMap));
    HTTPResponse resp = server.makeResponse();
    if (infile.isModified(if_none_match, if_modified_since)) {
        resp.body(infile.body());
        resp.statusCode(200);
    } else {
        resp.statusCode(304);
    }
    for (const auto &[field, value] : infile.headers()) {
        resp.setHeader(field, value);
    }

    return resp;
}

static std::string resolve_path(const std::string &path)
{
    char res_path[PATH_MAX];
    if (!realpath(path.c_str(), res_path)) {
        throw std::generic_category().default_error_condition(errno);
    }
    return res_path;
}

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
