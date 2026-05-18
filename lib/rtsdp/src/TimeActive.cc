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

#include "TimeActive.hh"

#define UNIX_TO_1900_SECONDS 2208988800
#define UNIX_TIME_TO_SDP_TIME(A) ((A) + UNIX_TO_1900_SECONDS)
#define SDP_TIME_TO_UNIX_TIME(A) ((A) - UNIX_TO_1900_SECONDS)
#define SYSTEM_CLOCK_TIME_POINT_TO_SDP_TIME(A) UNIX_TIME_TO_SDP_TIME(std::time_point_cast<std::chrono::seconds>(A).time_since_epoch().count())
#define SDP_TIME_TO_SYSTEM_CLOCK_TIME_POINT(A) (TimeActive::datetime_type(std::chrono::duration_cast<TimeActive::datetime_type::duration>(std::chrono::seconds(SDP_TIME_TO_UNIX_TIME(A)))))

LIBRTSDP_NAMESPACE_START

const TimeActive::datetime_type TimeActive::SDP_EPOCH(SDP_TIME_TO_SYSTEM_CLOCK_TIME_POINT(0));

TimeActive::TimeActive(const TimeActive::datetime_type &start, const TimeActive::datetime_type &end)
    :m_startTime(start)
    ,m_endTime(end)
{
}

TimeActive::TimeActive(const TimeActive &other)
    :m_startTime(other.m_startTime)
    ,m_endTime(other.m_endTime)
{
}

TimeActive::TimeActive(TimeActive &&other)
    :m_startTime(std::move(other.m_startTime))
    ,m_endTime(std::move(other.m_endTime))
{
}

TimeActive &TimeActive::operator=(const TimeActive &other)
{
    m_startTime = other.m_startTime;
    m_endTime = other.m_endTime;
    return *this;
}

TimeActive &TimeActive::operator=(TimeActive &&other)
{
    m_startTime = std::move(other.m_startTime);
    m_endTime = std::move(other.m_endTime);
    return *this;
}

bool TimeActive::operator==(const TimeActive &other) const
{
    return m_startTime == other.m_startTime && m_endTime == other.m_endTime;
}

std::size_t TimeActive::hash() const
{
    std::hash<datetime_type::rep> hr;
    return hr(m_startTime.time_since_epoch().count()) + hr(m_endTime.time_since_epoch().count()) * 2;
}

TimeActive::operator std::string() const
{
    auto start = UNIX_TIME_TO_SDP_TIME(std::chrono::time_point_cast<std::chrono::seconds>(m_startTime).time_since_epoch().count());
    auto end = UNIX_TIME_TO_SDP_TIME(std::chrono::time_point_cast<std::chrono::seconds>(m_endTime).time_since_epoch().count());

    return std::format("t={} {}\r\n", start, end);
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(TimeActive) &time_active)
{
    os << static_cast<std::string>(time_active);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
