/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: HTTPRequest unit test
 ******************************************************************************
 * Copyright: (C)2026 British Broadcasting Corporation
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

/* Covers HTTPRequest's own header/query-arg accessors directly. The class carries no
 * App::self()-style singleton dependency, so this genuinely link-isolates. Query-arg support
 * itself was added this session (see HTTPRequest.hh's own constructor comment, and
 * HTTPServer.cc's __accessHandlerCallback() fix) after a real, live-observed defect: MHD's own
 * MHD_GET_ARGUMENT_KIND was never fetched at all, so a handler had no way to read "?foo=bar". */

#include <cstdio>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "common.hh"
#include "HTTPRequest.hh"

HTTPXPP_NAMESPACE_USING(HTTPRequest);

static size_t total = 0, failed = 0;

#define CHECK(cond, msg) do { \
    total++; \
    if (!(cond)) { failed++; std::fprintf(stderr, "FAILED: %s\n", msg); } \
    else { std::fprintf(stderr, "OK: %s\n", msg); } \
} while (0)

int main()
{
    // Case 1: the 5-argument (no query_args) constructor still exists and defaults to no query
    // arguments at all -- the pre-existing behaviour this session's own fix had to preserve for
    // every caller that predates query-argument support.
    {
        HTTPRequest req("/path", "GET", "HTTP/1.1",
                        std::map<std::string, std::list<std::string>>{{"Content-Type", {"application/json"}}},
                        std::vector<char>{});
        CHECK(req.queryArgs().empty(), "the 5-argument constructor leaves queryArgs() empty");
        CHECK(req.getQueryArg("foo").empty(), "getQueryArg() on a request with no query args returns an empty list");
        CHECK(!req.getQueryArgFirst("foo").has_value(), "getQueryArgFirst() on a request with no query args returns nullopt");
    }

    // Case 2: the 6-argument constructor populates real query arguments, retrievable by exact name.
    {
        HTTPRequest req("/path", "GET", "HTTP/1.1",
                        std::map<std::string, std::list<std::string>>{},
                        std::map<std::string, std::list<std::string>>{{"service-class", {"urn:oma:bcast:oma_bsc:st:1.0"}}},
                        std::vector<char>{});
        CHECK(req.getQueryArg("service-class").size() == 1, "getQueryArg() finds a query argument set via the 6-argument constructor");
        CHECK(req.getQueryArgFirst("service-class") == "urn:oma:bcast:oma_bsc:st:1.0",
              "getQueryArgFirst() returns the query argument's value");
    }

    // Case 3: query argument names are case-SENSITIVE (RFC 3986 clause 3.4's query component has
    // no case-insensitivity rule, unlike RFC 9110 clause 5.1's header field names) -- a lookup
    // with a differently-cased name must not match.
    {
        HTTPRequest req("/path", "GET", "HTTP/1.1",
                        std::map<std::string, std::list<std::string>>{},
                        std::map<std::string, std::list<std::string>>{{"Service-Class", {"x"}}},
                        std::vector<char>{});
        CHECK(req.getQueryArg("service-class").empty(), "getQueryArg() is case-sensitive, unlike getHeader()");
        CHECK(req.getQueryArg("Service-Class").size() == 1, "getQueryArg() matches the exact case it was given");
    }

    // Case 4: getHeader()/getHeaderFirst() ARE case-insensitive (RFC 9110 clause 5.1) -- the
    // pre-existing behaviour that must not regress alongside the new query-arg accessors.
    {
        HTTPRequest req("/path", "GET", "HTTP/1.1",
                        std::map<std::string, std::list<std::string>>{{"Content-Type", {"application/json"}}},
                        std::vector<char>{});
        CHECK(req.getHeader("content-type").size() == 1, "getHeader() matches a differently-cased field name");
        CHECK(req.getHeaderFirst("CONTENT-TYPE") == "application/json", "getHeaderFirst() is also case-insensitive");
        CHECK(!req.getHeaderFirst("x-not-present").has_value(), "getHeaderFirst() on a missing header returns nullopt");
    }

    // Case 5: multiple values for one query argument name are preserved in order, and
    // getQueryArgFirst() returns only the first.
    {
        HTTPRequest req("/path", "GET", "HTTP/1.1",
                        std::map<std::string, std::list<std::string>>{},
                        std::map<std::string, std::list<std::string>>{{"tag", {"a", "b", "c"}}},
                        std::vector<char>{});
        const auto &values = req.getQueryArg("tag");
        CHECK(values.size() == 3, "getQueryArg() preserves all values for a repeated query argument name");
        CHECK(!values.empty() && values.front() == "a", "the first value is preserved in order");
        CHECK(req.getQueryArgFirst("tag") == "a", "getQueryArgFirst() returns only the first of several values");
    }

    // Case 6: copy and move construction/assignment both carry queryArgs() over correctly --
    // added alongside the other members in each of HTTPRequest's existing special members.
    {
        HTTPRequest req("/path", "GET", "HTTP/1.1",
                        std::map<std::string, std::list<std::string>>{},
                        std::map<std::string, std::list<std::string>>{{"a", {"1"}}},
                        std::vector<char>{});
        HTTPRequest copy(req);
        CHECK(copy.getQueryArgFirst("a") == "1", "copy construction carries queryArgs() over");

        HTTPRequest moved(std::move(req));
        CHECK(moved.getQueryArgFirst("a") == "1", "move construction carries queryArgs() over");
    }

    std::fprintf(stderr, "%zu/%zu checks passed\n", total - failed, total);
    return failed == 0 ? 0 : 1;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
