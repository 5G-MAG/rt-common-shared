#ifndef _LIBRTSDP_PUB_ORIGINATOR_IMPL_HH_
#define _LIBRTSDP_PUB_ORIGINATOR_IMPL_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: OriginatorImpl class
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

#include <netinet/in.h>

#include <format>
#include <iostream>
#include <memory>
#include <string>

#include <rtsdp/common.hh>
#include "Origin.hh"
#include <rtsdp/Originator.hh>

LIBRTSDP_NAMESPACE_START

class SDP;

class OriginatorImpl : public Originator {
public:
    OriginatorImpl();
    OriginatorImpl(const std::string &hostname, int family = AF_UNSPEC, const std::string &user = std::string());
    OriginatorImpl(const struct sockaddr_in &sin, const std::string &user = std::string());
    OriginatorImpl(const struct sockaddr_in6 &sin6, const std::string &user = std::string());
    OriginatorImpl(const struct in_addr &ina, const std::string &user = std::string());
    OriginatorImpl(const struct in6_addr &in6a, const std::string &user = std::string());
    OriginatorImpl(const OriginatorImpl &other);
    OriginatorImpl(OriginatorImpl &&other);

    OriginatorImpl(const std::shared_ptr<Origin> &origin);

    virtual ~OriginatorImpl() {};

    OriginatorImpl &operator=(const OriginatorImpl &other);
    OriginatorImpl &operator=(OriginatorImpl &&other);

    bool operator==(const OriginatorImpl &other) const;

    virtual std::size_t hash() const;
    virtual operator std::string() const;
    virtual bool isValid() const;

    virtual const std::string &hostname() const;
    virtual Originator &hostname(const std::string &host, int family = AF_UNSPEC);
    virtual Originator &hostname(std::string &&host, int family = AF_UNSPEC);

    virtual std::pair<const struct sockaddr&, socklen_t> address() const;
    virtual Originator &address(const struct sockaddr &sa, socklen_t sa_len);
    virtual Originator &address(const struct sockaddr_in &sin);
    virtual Originator &address(const struct sockaddr_in6 &sin6);
    virtual Originator &address(const struct in_addr &ina);
    virtual Originator &address(const struct in6_addr &in6a);

    virtual const std::string &username() const;
    virtual Originator &username(const std::string &user_name);
    virtual Originator &username(std::string &&user_name);

    const std::shared_ptr<Origin> &origin() const { return m_origin; };

    void setParent(const std::weak_ptr<SDP> &parent) { m_parent = parent; };

private:
    std::weak_ptr<SDP> m_parent;
    std::shared_ptr<Origin> m_origin;
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(OriginatorImpl), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(OriginatorImpl) &origin, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(origin));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(OriginatorImpl)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(OriginatorImpl) &origin) const { return origin.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(OriginatorImpl) &origin);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_ORIGINATOR_IMPL_HH_ */
