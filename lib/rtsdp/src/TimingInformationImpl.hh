#ifndef _LIBRTSDP_TIMING_INFORMATION_IMPL_HH_
#define _LIBRTSDP_TIMING_INFORMATION_IMPL_HH_
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
#include <rtsdp/SDP.hh>
#include <rtsdp/TimingInformation.hh>

LIBRTSDP_NAMESPACE_START

class TimingInformationImpl : public TimingInformation {
public:
    using timestamp_type = TimingInformation::timestamp_type;

    TimingInformationImpl();
    TimingInformationImpl(const timestamp_type &start = TimingInformation::NO_TIMESTAMP,
                          const timestamp_type &end = TimingInformation::NO_TIMESTAMP);
    TimingInformationImpl(const TimingInformationImpl &other);
    TimingInformationImpl(TimingInformationImpl &&other);

    TimingInformationImpl(const std::shared_ptr<TimingInfo> &timing_info);

    virtual ~TimingInformationImpl() {};

    TimingInformationImpl &operator=(const TimingInformationImpl &other);
    TimingInformationImpl &operator=(TimingInformationImpl &&other);

    bool operator==(const TimingInformationImpl &other) const;

    TimingInformationImpl &setParent(const std::weak_ptr<SDP> &sdp) { m_parent = sdp; return *this; };
    const std::shared_ptr<TimingInfo> &timingInfo() const { return m_timingInfo; };

    virtual std::size_t hash() const;
    virtual operator std::string() const;

    virtual bool isValid() const;

    virtual timestamp_type startTime() const;
    virtual TimingInformation &startTime(const timestamp_type &start_time);
    virtual TimingInformation &startTime(timestamp_type &&start_time);

    virtual timestamp_type endTime() const;
    virtual TimingInformation &endTime(const timestamp_type &end_time);
    virtual TimingInformation &endTime(timestamp_type &&end_time);

    virtual const std::list<RepeatField> &repeatFields() const;
    virtual TimingInformation &repeatFields(const std::list<RepeatField> &repeat_fields);
    virtual TimingInformation &repeatFieldAdd(const RepeatField &repeat_field);
    virtual TimingInformation &repeatFieldAdd(RepeatField &&repeat_field);
    virtual TimingInformation &repeatFieldRemove(const RepeatField &repeat_field);

private:
    std::weak_ptr<SDP> m_parent;
    std::shared_ptr<TimingInfo> m_timingInfo;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(TimingInformationImpl), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(TimingInformationImpl) &timing_info, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(timing_info));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(TimingInformationImpl)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(TimingInformationImpl) &timing_info) const { return timing_info.hash(); };
};

}

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(TimingInformationImpl) &timing_info);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_TIMING_INFORMATION_IMPL_HH_ */
