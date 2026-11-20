#ifndef _RT_COMMON_HTTP_X_SERVER_COMMON_H_
#define _RT_COMMON_HTTP_X_SERVER_COMMON_H_
/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: HTTP Server common macros
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

#define HTTPXPP_NAMESPACE_NS reftools::common::httpxpp
#define HTTPXPP_NAMESPACE_START namespace HTTPXPP_NAMESPACE_NS {
#define HTTPXPP_NAMESPACE_STOP }
#define HTTPXPP_NAMESPACE_NAME(A) HTTPXPP_NAMESPACE_NS::A
#define HTTPXPP_NAMESPACE_USING(A) using HTTPXPP_NAMESPACE_NAME(A)
#define HTTPXPP_USING_NAMESPACE using namespace HTTPXPP_NAMESPACE_NS

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif // ifndef _RT_COMMON_HTTP_X_SERVER_COMMON_H_
