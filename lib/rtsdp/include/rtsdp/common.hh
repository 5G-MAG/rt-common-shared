#ifndef _RT_COMMON_SDP_LIBRARY_COMMON_H_
#define _RT_COMMON_SDP_LIBRARY_COMMON_H_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: Common macros
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

#define LIBRTSDP_NAMESPACE_NS reftools::common::rtsdp
#define LIBRTSDP_NAMESPACE_START namespace LIBRTSDP_NAMESPACE_NS {
#define LIBRTSDP_NAMESPACE_STOP }
#define LIBRTSDP_NAMESPACE_NAME(A) LIBRTSDP_NAMESPACE_NS::A
#define LIBRTSDP_NAMESPACE_USING(A) using LIBRTSDP_NAMESPACE_NAME(A)
#define LIBRTSDP_USING_NAMESPACE using namespace LIBRTSDP_NAMESPACE_NS

/* vim:ts=8:sts=4:sw=4:expandtab:
 */

#endif // ifndef _RT_COMMON_SDP_LIBRARY_COMMON_H_
