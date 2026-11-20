#ifndef _LIBRTSDP_PUB_REPEAT_TIME_HH_
#define _LIBRTSDP_PUB_REPEAT_TIME_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: RepeatTime class
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
#include <list>

#include <rtsdp/common.hh>

LIBRTSDP_NAMESPACE_START

class RepeatTime {
public:
    using duration_type = std::chrono::seconds;

    RepeatTime();
    RepeatTime(const duration_type &repeat_interval, const duration_type &active_duration, std::initializer_list<duration_type> start_offsets);
    RepeatTime(const RepeatTime &other);
    RepeatTime(RepeatTime &&other);

    virtual ~RepeatTime() {};

    RepeatTime &operator=(const RepeatTime &other);
    RepeatTime &operator=(RepeatTime &&other);

    bool operator==(const RepeatTime &other) const;

    std::size_t hash() const;
    operator std::string() const;
    operator bool() const;

private:
    duration_type m_repeatInterval;
    duration_type m_activeDuration;
    std::list<duration_type> m_startOffsets;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(RepeatTime), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(RepeatTime) &repeat_time, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(repeat_time));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(RepeatTime)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(RepeatTime) &repeat_time) const { return repeat_time.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(RepeatTime) &repeat_time);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_REPEAT_TIME_HH_ */
