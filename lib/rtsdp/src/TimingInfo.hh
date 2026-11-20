#ifndef _LIBSDP_TIMING_INFO_HH_
#define _LIBSDP_TIMING_INFO_HH_
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

LIBRTSDP_NAMESPACE_START

class TimingInfo {
public:
    TimingInfo();
    TimingInfo(const TimeActive &time_active, std::initializer_list<RepeatField> repeat_fields);
    TimingInfo(const TimingInfo &other);
    TimingInfo(TimingInfo &&other);

    virtual ~TimingInfo() {};

    TimingInfo &operator=(const TimingInfo &other);
    TimingInfo &operator=(TimingInfo &&other);

    bool operator==(const TimingInfo &other) const;

    std::size_t hash() const;
    operator std::string() const;

    bool isValid() const;

    TimeActive &timeActive() { return m_timeActive; };
    const TimeActive &timeActive() const { return m_timeActive; };

    std::list<RepeatField> &repeatFields() { return m_repeatFields; };
    const std::list<RepeatField> &repeatFields() const { return m_repeatFields; };
    TimingInfo &repeatFields(const std::list<RepeatField> &repeat_fields) { m_repeatFields = repeat_fields; return *this; };
    TimingInfo &repeatFieldAdd(const RepeatField &repeat_field) { m_repeatFields.push_back(repeat_field); return *this; };
    TimingInfo &repeatFieldAdd(RepeatField &&repeat_field) { m_repeatFields.push_back(std::move(repeat_field)); return *this; };
    TimingInfo &repeatFieldRemove(const RepeatField &repeat_field) { m_repeatFields.remove(repeat_field); return *this; };

private:
    TimeActive m_timeActive;
    std::list<RepeatField> m_repeatFields;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(TimingInfo), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(TimingInfo) &timing_information, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(timing_information));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(TimingInfo)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(TimingInfo) &timing_information) const { return timing_information.hash(); };
};

}

std::ostream &operator<<(std::ostream &os, const LIBRTSDP_NAMESPACE_NAME(TimingInfo) &timing_information);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBSDP_TIMING_INFO_HH_ */
