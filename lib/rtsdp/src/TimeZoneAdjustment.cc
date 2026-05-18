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

#include <rtsdp/TimeZoneAdjustment.hh>

LIBRTSDP_NAMESPACE_START

TimeZoneAdjustment::TimeZoneAdjustment()
    :m_startTime(std::chrono::duration_cast<datetime_type::duration>(std::chrono::seconds(-2208988800)))
    ,m_offset(0)
{
}

TimeZoneAdjustment::TimeZoneAdjustment(const TimeZoneAdjustment::datetime_type &start_time, const TimeZoneAdjustment::duration_type &offset)
    :m_startTime(start_time)
    ,m_offset(offset)
{
}

TimeZoneAdjustment::TimeZoneAdjustment(const TimeZoneAdjustment &other)
    :m_startTime(other.m_startTime)
    ,m_offset(other.m_offset)
{
}

TimeZoneAdjustment::TimeZoneAdjustment(TimeZoneAdjustment &&other)
    :m_startTime(std::move(other.m_startTime))
    ,m_offset(std::move(other.m_offset))
{
}

TimeZoneAdjustment &TimeZoneAdjustment::operator=(const TimeZoneAdjustment &other)
{
    m_startTime = other.m_startTime;
    m_offset = other.m_offset;
    return *this;
}

TimeZoneAdjustment &TimeZoneAdjustment::operator=(TimeZoneAdjustment &&other)
{
    m_startTime = std::move(other.m_startTime);
    m_offset = std::move(other.m_offset);
    return *this;
}

bool TimeZoneAdjustment::operator==(const TimeZoneAdjustment &other) const
{
    return m_offset == other.m_offset && m_startTime == other.m_startTime;
}

std::size_t TimeZoneAdjustment::hash() const
{
    std::hash<datetime_type::rep> hdt;
    std::hash<duration_type::rep> hdurn;

    return hdt(m_startTime.time_since_epoch().count()) + hdurn(m_offset.count()) * 2;
}

TimeZoneAdjustment::operator std::string() const
{
    auto durn = m_offset.count();
    if (durn == 0) return std::string();
    std::string durn_str;
    if (durn % 86400 == 0) {
        durn_str = std::format("{}d", durn/86400);
    } else if (durn % 3600 == 0) {
        durn_str = std::format("{}h", durn/3600);
    } else if (durn % 60 == 0) {
        durn_str = std::format("{}m", durn/60);
    } else {
        durn_str = std::format("{}", durn);
    }
    auto start_time = std::chrono::time_point_cast<std::chrono::seconds>(m_startTime).time_since_epoch().count() + 2208988800;
    return std::format("{} {}", start_time, durn_str);
}

TimeZoneAdjustment::operator bool() const
{
    return m_offset.count() != 0;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(TimeZoneAdjustment) &time_zone_adj)
{
    os << static_cast<std::string>(time_zone_adj);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
