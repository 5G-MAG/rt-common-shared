#ifndef _LIBSDP_MEDIA_NAME_HH_
#define _LIBSDP_MEDIA_NAME_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: MediaName class
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
#include <iostream>
#include <list>
#include <string>

#include <rtsdp/common.hh>

LIBRTSDP_NAMESPACE_START

class MediaName {
public:
    MediaName();
    MediaName(const std::string &media_type, uint16_t port, const std::string &proto, const std::string &fmt, uint16_t number_of_ports = 1);
    MediaName(const MediaName &other);
    MediaName(MediaName &&other);

    virtual ~MediaName() {};

    MediaName &operator=(const MediaName &other);
    MediaName &operator=(MediaName &&other);

    bool operator==(const MediaName &other) const;

    std::size_t hash() const;
    operator std::string() const;
    operator bool() const;

private:
    std::string m_mediaType;
    uint16_t m_port;
    uint16_t m_numberOfPorts;
    std::string m_proto;
    std::string m_fmt;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(MediaName), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(MediaName) &media_name, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(media_name));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(MediaName)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(MediaName) &media_name) const { return media_name.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(MediaName) &media_name);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBSDP_MEDIA_NAME_HH_ */
