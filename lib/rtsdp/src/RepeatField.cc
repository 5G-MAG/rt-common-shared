/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: RepeatField class
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
#include <rtsdp/RepeatTime.hh>
#include <rtsdp/TimeZoneAdjustment.hh>

#include <rtsdp/RepeatField.hh>

LIBRTSDP_NAMESPACE_START

RepeatField::RepeatField()
    :m_repeatTime()
    ,m_timeZoneAdjustments()
{
}

RepeatField::RepeatField(const RepeatTime &repeat_time)
    :m_repeatTime(repeat_time)
    ,m_timeZoneAdjustments()
{
}

RepeatField::RepeatField(const RepeatTime &repeat_time, std::initializer_list<TimeZoneAdjustment> init_zones)
    :m_repeatTime(repeat_time)
    ,m_timeZoneAdjustments(init_zones)
{
}

RepeatField::RepeatField(const RepeatField &other)
    :m_repeatTime(other.m_repeatTime)
    ,m_timeZoneAdjustments(other.m_timeZoneAdjustments)
{
}

RepeatField::RepeatField(RepeatField &&other)
    :m_repeatTime(std::move(other.m_repeatTime))
    ,m_timeZoneAdjustments(std::move(other.m_timeZoneAdjustments))
{
}

RepeatField &RepeatField::operator=(const RepeatField &other)
{
    m_repeatTime = other.m_repeatTime;
    m_timeZoneAdjustments = other.m_timeZoneAdjustments;
    return *this;
}

RepeatField &RepeatField::operator=(RepeatField &&other)
{
    m_repeatTime = std::move(other.m_repeatTime);
    m_timeZoneAdjustments = std::move(other.m_timeZoneAdjustments);
    return *this;
}

bool RepeatField::operator==(const RepeatField &other) const
{
    return m_repeatTime == other.m_repeatTime && m_timeZoneAdjustments == other.m_timeZoneAdjustments;
}

std::size_t RepeatField::hash() const
{
    std::hash<RepeatTime> hrt;
    std::hash<TimeZoneAdjustment> htza;
    std::hash<size_t> hs;

    std::size_t result = hrt(m_repeatTime);
    size_t idx = 1;
    for (auto &tza : m_timeZoneAdjustments) {
        result += hs(idx++) ^ htza(tza);
    }

    return result;
}

RepeatField::operator std::string() const
{
    if (!m_repeatTime) return std::string();

    std::string result = std::format("{}", m_repeatTime);
    if (!m_timeZoneAdjustments.empty()) {
        result += "z=";
    }
    const char *sep = "";
    for (auto &tza : m_timeZoneAdjustments) {
        result += std::format("{}{}", sep, tza);
        sep = " ";
    }
    if (!m_timeZoneAdjustments.empty()) {
        result += "\r\n";
    }

    return result;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(RepeatField) &repeat_field)
{
    os << static_cast<std::string>(repeat_field);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
