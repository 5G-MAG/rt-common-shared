/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: TimingInfo class
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
#include <rtsdp/RepeatField.hh>
#include "TimeActive.hh"

#include "TimingInfo.hh"

LIBRTSDP_NAMESPACE_START

TimingInfo::TimingInfo()
    :m_timeActive()
    ,m_repeatFields()
{
}

TimingInfo::TimingInfo(const TimeActive &time_active, std::initializer_list<RepeatField> repeat_fields)
    :m_timeActive(time_active)
    ,m_repeatFields(repeat_fields)
{
}

TimingInfo::TimingInfo(const TimingInfo &other)
    :m_timeActive(other.m_timeActive)
    ,m_repeatFields(other.m_repeatFields)
{
}

TimingInfo::TimingInfo(TimingInfo &&other)
    :m_timeActive(std::move(other.m_timeActive))
    ,m_repeatFields(std::move(other.m_repeatFields))
{
}

TimingInfo &TimingInfo::operator=(const TimingInfo &other)
{
    m_timeActive = other.m_timeActive;
    m_repeatFields = other.m_repeatFields;
    return *this;
}

TimingInfo &TimingInfo::operator=(TimingInfo &&other)
{
    m_timeActive = std::move(other.m_timeActive);
    m_repeatFields = std::move(other.m_repeatFields);
    return *this;
}

bool TimingInfo::operator==(const TimingInfo &other) const
{
    return m_timeActive == other.m_timeActive && m_repeatFields == other.m_repeatFields;
}

std::size_t TimingInfo::hash() const
{
    std::hash<TimeActive> hta;
    std::hash<RepeatField> hrf;
    std::hash<size_t> hsz;

    std::size_t result = hta(m_timeActive);
    size_t idx = 0;
    for (auto &rf : m_repeatFields) {
        result += hsz(++idx) ^ hrf(rf);
    }

    return result;
}

TimingInfo::operator std::string() const
{
    if (!m_timeActive) return std::string();
    std::string result = std::format("{}", m_timeActive);
    for (auto &rf : m_repeatFields) {
        result += std::format("{}", rf);
    }
    return result;
}

bool TimingInfo::isValid() const
{
    return !!m_timeActive;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(TimingInfo) &timing_information)
{
    os << static_cast<std::string>(timing_information);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
