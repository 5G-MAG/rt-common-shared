#ifndef _LIBSDP_TIME_ACTIVE_HH_
#define _LIBSDP_TIME_ACTIVE_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: SessionDescriptionProtocol class
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

class TimeActive {
public:
    using datetime_type = std::chrono::system_clock::time_point;

    static const datetime_type SDP_EPOCH;

    TimeActive(const datetime_type &start = SDP_EPOCH, const datetime_type &end = SDP_EPOCH);
    TimeActive(const TimeActive &other);
    TimeActive(TimeActive &&other);

    virtual ~TimeActive() {};

    TimeActive &operator=(const TimeActive &other);
    TimeActive &operator=(TimeActive &&other);

    bool operator==(const TimeActive &other) const;

    std::size_t hash() const;
    operator std::string() const;
    operator bool() const { return isValid(); };
    bool isValid() const { return true; };

    bool hasStart() const { return m_startTime != SDP_EPOCH; };
    bool hasEnd() const { return m_endTime != SDP_EPOCH; };

    const datetime_type &start() const { return m_startTime; };
    TimeActive &start(const datetime_type &val) { m_startTime = val; return *this; };
    TimeActive &start(datetime_type &&val) { m_startTime = std::move(val); return *this; };

    const datetime_type &end() const { return m_endTime; };
    TimeActive &end(const datetime_type &val) { m_endTime = val; return *this; };
    TimeActive &end(datetime_type &&val) { m_endTime = std::move(val); return *this; };

private:
    datetime_type m_startTime;
    datetime_type m_endTime;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(TimeActive), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(TimeActive) &time_active, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(time_active));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(TimeActive)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(TimeActive) &time_active) const { return time_active.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(TimeActive) &time_active);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBSDP_TIME_ACTIVE_HH_ */
