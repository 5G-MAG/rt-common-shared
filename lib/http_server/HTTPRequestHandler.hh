#ifndef _HTTPXPP_HTTP_REQUEST_HANDLER_HH_
#define _HTTPXPP_HTTP_REQUEST_HANDLER_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: HTTPRequestHandler class
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

#include <string>
#include <vector>

#include "common.hh"

HTTPXPP_NAMESPACE_START

class HTTPServer;
class HTTPRequest;
class HTTPResponse;

class HTTPRequestHandler {
public:
    HTTPRequestHandler() {};
    HTTPRequestHandler(const HTTPRequestHandler &other) = delete;
    HTTPRequestHandler(HTTPRequestHandler &&other) = delete;

    virtual ~HTTPRequestHandler() {};

    HTTPRequestHandler &operator=(const HTTPRequestHandler &other) = delete;
    HTTPRequestHandler &operator=(HTTPRequestHandler &&other) = delete;

    virtual void serverShutdown(const HTTPServer &server) {};
    virtual HTTPResponse doRequest(const HTTPRequest &request, const HTTPServer &server) = 0;

private:
};

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _HTTPXPP_HTTP_REQUEST_HANDLER_HH_ */
