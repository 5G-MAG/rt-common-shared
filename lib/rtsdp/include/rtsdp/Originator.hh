#ifndef _LIBRTSDP_PUB_ORIGINATOR_HH_
#define _LIBRTSDP_PUB_ORIGINATOR_HH_
/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: Originator class
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

#include "common.hh"

LIBRTSDP_NAMESPACE_START

class Originator {
public:
    static std::shared_ptr<Originator> makeOriginator();
    static std::shared_ptr<Originator> makeOriginator(const std::string &hostname, int family = AF_UNSPEC, const std::string &user = std::string());
    static std::shared_ptr<Originator> makeOriginator(const struct sockaddr_in &sin, const std::string &user = std::string());
    static std::shared_ptr<Originator> makeOriginator(const struct sockaddr_in6 &sin6, const std::string &user = std::string());
    static std::shared_ptr<Originator> makeOriginator(const struct in_addr &ina, const std::string &user = std::string());
    static std::shared_ptr<Originator> makeOriginator(const struct in6_addr &in6a, const std::string &user = std::string());
    static std::shared_ptr<Originator> makeOriginator(const std::shared_ptr<Originator> &other); // Copies the underlying Originator object

    virtual ~Originator() {};

    bool operator==(const Originator &other) const;

    virtual std::size_t hash() const = 0;
    virtual operator std::string() const = 0;
    operator bool() const { return isValid(); };

    virtual bool isValid() const = 0;

    virtual const std::string &hostname() const = 0;
    virtual Originator &hostname(const std::string &host, int family = AF_UNSPEC) = 0;
    virtual Originator &hostname(std::string &&host, int family = AF_UNSPEC) = 0;

    virtual std::pair<const struct sockaddr&, socklen_t> address() const = 0;
    virtual Originator &address(const struct sockaddr &sa, socklen_t sa_len) = 0;
    virtual Originator &address(const struct sockaddr_in &sin) = 0;
    virtual Originator &address(const struct sockaddr_in6 &sin6) = 0;
    virtual Originator &address(const struct in_addr &ina) = 0;
    virtual Originator &address(const struct in6_addr &in6a) = 0;

    virtual const std::string &username() const = 0;
    virtual Originator &username(const std::string &user_name) = 0;
    virtual Originator &username(std::string &&user_name) = 0;

protected:
    Originator();
    Originator(const Originator &other);
    Originator(Originator &&other);

    Originator &operator=(const Originator &other);
    Originator &operator=(Originator &&other);
};

LIBRTSDP_NAMESPACE_STOP

namespace std {

template<>
struct formatter<LIBRTSDP_NAMESPACE_NAME(Originator), char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    };

    template <class FmtContext>
    FmtContext::iterator format(const LIBRTSDP_NAMESPACE_NAME(Originator) &origin, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(origin));
    };
};

template<>
struct hash<LIBRTSDP_NAMESPACE_NAME(Originator)> {
    std::size_t operator()(const LIBRTSDP_NAMESPACE_NAME(Originator) &origin) const { return origin.hash(); };
};

}

std::ostream &operator<<(std::ostream &, const LIBRTSDP_NAMESPACE_NAME(Originator) &origin);

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
#endif /* _LIBRTSDP_PUB_ORIGINATOR_HH_ */
