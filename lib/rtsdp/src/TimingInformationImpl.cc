/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: TimingInformation class
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
#include <memory>
#include <string>

#include <rtsdp/common.hh>
#include <rtsdp/RepeatField.hh>
#include <rtsdp/TimingInformation.hh>
#include "TimeActive.hh"
#include "TimingInfo.hh"

#include "TimingInformationImpl.hh"

LIBRTSDP_NAMESPACE_START

TimingInformationImpl::TimingInformationImpl()
    :m_parent()
    ,m_timingInfo()
{
}

TimingInformationImpl::TimingInformationImpl(const timestamp_type &start, const timestamp_type &end)
    :m_parent()
    ,m_timingInfo(new TimingInfo(TimeActive(start,end), {}))
{
}

TimingInformationImpl::TimingInformationImpl(const TimingInformationImpl &other)
    :m_parent(other.m_parent)
    ,m_timingInfo(other.m_timingInfo)
{
}

TimingInformationImpl::TimingInformationImpl(TimingInformationImpl &&other)
    :m_parent(std::move(other.m_parent))
    ,m_timingInfo(std::move(other.m_timingInfo))
{
}

TimingInformationImpl::TimingInformationImpl(const std::shared_ptr<TimingInfo> &timing_info)
    :m_parent()
    ,m_timingInfo(timing_info)
{
}

TimingInformationImpl &TimingInformationImpl::operator=(const TimingInformationImpl &other)
{
    m_parent = other.m_parent;
    m_timingInfo = other.m_timingInfo;
    return *this;
}

TimingInformationImpl &TimingInformationImpl::operator=(TimingInformationImpl &&other)
{
    m_parent = std::move(other.m_parent);
    m_timingInfo = std::move(other.m_timingInfo);
    return *this;
}

bool TimingInformationImpl::operator==(const TimingInformationImpl &other) const
{
    if (m_timingInfo == other.m_timingInfo) return true;
    if (!m_timingInfo || !other.m_timingInfo) return false;
    return *m_timingInfo == *other.m_timingInfo;
}

std::size_t TimingInformationImpl::hash() const
{
    return m_timingInfo?m_timingInfo->hash():0;
}

TimingInformationImpl::operator std::string() const
{
    return m_timingInfo?static_cast<std::string>(*m_timingInfo):std::string();
}

bool TimingInformationImpl::isValid() const
{
    return m_timingInfo && m_timingInfo->isValid();
}

TimingInformationImpl::timestamp_type TimingInformationImpl::startTime() const
{
    return m_timingInfo?m_timingInfo->timeActive().start():TimingInformation::NO_TIMESTAMP;
}

TimingInformation &TimingInformationImpl::startTime(const TimingInformationImpl::timestamp_type &start_time)
{
    if (!m_timingInfo) m_timingInfo.reset(new TimingInfo);
    m_timingInfo->timeActive().start(start_time);
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

TimingInformation &TimingInformationImpl::startTime(TimingInformationImpl::timestamp_type &&start_time)
{
    if (!m_timingInfo) m_timingInfo.reset(new TimingInfo);
    m_timingInfo->timeActive().start(std::move(start_time));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

TimingInformationImpl::timestamp_type TimingInformationImpl::endTime() const
{
    return m_timingInfo?m_timingInfo->timeActive().end():TimingInformation::NO_TIMESTAMP;
}

TimingInformation &TimingInformationImpl::endTime(const TimingInformationImpl::timestamp_type &end_time)
{
    if (!m_timingInfo) m_timingInfo.reset(new TimingInfo);
    m_timingInfo->timeActive().end(end_time);
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

TimingInformation &TimingInformationImpl::endTime(TimingInformationImpl::timestamp_type &&end_time)
{
    if (!m_timingInfo) m_timingInfo.reset(new TimingInfo);
    m_timingInfo->timeActive().end(std::move(end_time));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

const std::list<RepeatField> &TimingInformationImpl::repeatFields() const
{
    if (m_timingInfo) return m_timingInfo->repeatFields();
    static const std::list<RepeatField> empty_repeat_fields;
    return empty_repeat_fields;
}

TimingInformation &TimingInformationImpl::repeatFields(const std::list<RepeatField> &repeat_fields)
{
    if (!m_timingInfo) m_timingInfo.reset(new TimingInfo);
    m_timingInfo->repeatFields(repeat_fields);
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

TimingInformation &TimingInformationImpl::repeatFieldAdd(const RepeatField &repeat_field)
{
    if (!m_timingInfo) m_timingInfo.reset(new TimingInfo);
    m_timingInfo->repeatFieldAdd(repeat_field);
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

TimingInformation &TimingInformationImpl::repeatFieldAdd(RepeatField &&repeat_field)
{
    if (!m_timingInfo) m_timingInfo.reset(new TimingInfo);
    m_timingInfo->repeatFieldAdd(std::move(repeat_field));
    if (!m_parent.expired()) m_parent.lock()->sessionModified();
    return *this;
}

TimingInformation &TimingInformationImpl::repeatFieldRemove(const RepeatField &repeat_field)
{
    if (m_timingInfo) {
        m_timingInfo->repeatFieldRemove(repeat_field);
        if (!m_parent.expired()) m_parent.lock()->sessionModified();
    }
    return *this;
}

LIBRTSDP_NAMESPACE_STOP

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(TimingInformationImpl) &timing_info)
{
    os << static_cast<std::string>(timing_info);
    return os;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
