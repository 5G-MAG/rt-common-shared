#ifndef _LIBRTSDP_PUB_TIME_ZONE_ADJUSTMENT_HH_
#define _LIBRTSDP_PUB_TIME_ZONE_ADJUSTMENT_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: TimeZoneAdjustment class
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

#include <chrono>
#include <format>
#include <iostream>
#include <string>

#include <rtsdp/common.hh>

LIBRTSDP_NAMESPACE_START

class TimeZoneAdjustment {
public:
    using datetime_type = std::chrono::system_clock::time_point;
    using duration_type = std::chrono::seconds;

    TimeZoneAdjustment();
    TimeZoneAdjustment(const datetime_type &start_time, const duration_type &offset);
    TimeZoneAdjustment(const TimeZoneAdjustment &other);
    TimeZoneAdjustment(TimeZoneAdjustment &&other);

    virtual ~TimeZoneAdjustment() {};

    TimeZoneAdjustment &operator=(const TimeZoneAdjustment &other);
    TimeZoneAdjustment &operator=(TimeZoneAdjustment &&other);

    bool operator==(const TimeZoneAdjustment &other) const;

    std::size_t hash() const;
    operator std::string() const;
    operator bool() const;

private:
    datetime_type m_startTime;
    duration_type m_offset;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(TimeZoneAdjustment), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(TimeZoneAdjustment) &time_zone_adj, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(time_zone_adj));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(TimeZoneAdjustment)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(TimeZoneAdjustment) &time_zone_adj) const { return time_zone_adj.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(TimeZoneAdjustment) &time_zone_adj);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_TIME_ZONE_ADJUSTMENT_HH_ */
