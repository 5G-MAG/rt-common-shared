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
#include <string>
#include <version>

#include <rtsdp/common.hh>

#include <rtsdp/RepeatTime.hh>

#if __cpp_lib_chrono < 202306L
namespace std {
template<class Rep, class Period>
struct hash<std::chrono::duration<Rep, Period>> {
    std::size_t operator()(const std::chrono::duration<Rep, Period> &durn) const {
        std::hash<Rep> hr;
        std::hash<std::intmax_t> hp;
        return hr(durn.count()) + hp(Period::num) * 2 + hp(Period::den) * 3;
    };
};
}
#endif

LIBRTSDP_NAMESPACE_START

RepeatTime::RepeatTime()
    :m_repeatInterval(0)
    ,m_activeDuration(0)
    ,m_startOffsets()
{
}

RepeatTime::RepeatTime(const RepeatTime::duration_type &repeat_interval, const RepeatTime::duration_type &active_duration,
                       std::initializer_list<RepeatTime::duration_type> start_offsets)
    :m_repeatInterval(repeat_interval)
    ,m_activeDuration(active_duration)
    ,m_startOffsets(start_offsets)
{
}

RepeatTime::RepeatTime(const RepeatTime &other)
    :m_repeatInterval(other.m_repeatInterval)
    ,m_activeDuration(other.m_activeDuration)
    ,m_startOffsets(other.m_startOffsets)
{
}

RepeatTime::RepeatTime(RepeatTime &&other)
    :m_repeatInterval(std::move(other.m_repeatInterval))
    ,m_activeDuration(std::move(other.m_activeDuration))
    ,m_startOffsets(std::move(other.m_startOffsets))
{
}

RepeatTime &RepeatTime::operator=(const RepeatTime &other)
{
    m_repeatInterval = other.m_repeatInterval;
    m_activeDuration = other.m_activeDuration;
    m_startOffsets = other.m_startOffsets;
    return *this;
}

RepeatTime &RepeatTime::operator=(RepeatTime &&other)
{
    m_repeatInterval = std::move(other.m_repeatInterval);
    m_activeDuration = std::move(other.m_activeDuration);
    m_startOffsets = std::move(other.m_startOffsets);
    return *this;
}

bool RepeatTime::operator==(const RepeatTime &other) const
{
    return m_repeatInterval == other.m_repeatInterval && m_activeDuration == other.m_activeDuration &&
           m_startOffsets == other.m_startOffsets;
}

std::size_t RepeatTime::hash() const
{
    std::hash<duration_type> hd;
    std::hash<size_t> hui;
    std::size_t result = hd(m_repeatInterval) + hd(m_activeDuration) * 2;
    size_t idx = 0;
    for (auto &so : m_startOffsets) {
        result += hui(idx++) ^ hd(so);
    }
    return result;
}

RepeatTime::operator std::string() const
{
    if (!m_repeatInterval.count()) return std::string();

    std::string result = std::format("r={} {}", m_repeatInterval.count(), m_activeDuration.count());
    for (auto &so : m_startOffsets) {
        result += std::format(" {}", so.count());
    }
    result += "\r\n";
    return result;
}

RepeatTime::operator bool() const
{
    return m_repeatInterval.count() != 0;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(RepeatTime) &repeat_times)
{
    os << static_cast<std::string>(repeat_times);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
