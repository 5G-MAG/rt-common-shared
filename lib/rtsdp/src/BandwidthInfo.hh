#ifndef _LIBRTSDP_BANDWIDTH_INFO_HH_
#define _LIBRTSDP_BANDWIDTH_INFO_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: BandwidthInfo class
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

class BandwidthInfo {
public:
    using value_type = unsigned long int;

    BandwidthInfo();
    BandwidthInfo(const std::string &bandwidth_type, value_type value);
    BandwidthInfo(const BandwidthInfo &other);
    BandwidthInfo(BandwidthInfo &&other);

    virtual ~BandwidthInfo() {};

    BandwidthInfo &operator=(const BandwidthInfo &other);
    BandwidthInfo &operator=(BandwidthInfo &&other);

    bool operator==(const BandwidthInfo &other) const;

    std::size_t hash() const;
    operator std::string() const;

    const std::string &type() const { return m_type; };
    BandwidthInfo &type(const std::string &val) { m_type = val; return *this; };
    BandwidthInfo &type(std::string &&val) { m_type = std::move(val); return *this; };
    BandwidthInfo &typeClear() { m_type.clear(); return *this; };

    value_type value() const { return m_value; };
    BandwidthInfo &value(value_type val) { m_value = val; return *this; };

private:
    std::string m_type;
    value_type m_value;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(BandwidthInfo), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(BandwidthInfo) &bandwidth_info, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(bandwidth_info));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(BandwidthInfo)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(BandwidthInfo) &bandwidth_info) const { return bandwidth_info.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(BandwidthInfo) &bandwidth_info);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_BANDWIDTH_INFO_HH_ */
