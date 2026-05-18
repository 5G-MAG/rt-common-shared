/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: DocrootFile class
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

#include <sys/stat.h>

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "common.hh"
#include "hash.hh"

#include "DocrootFile.hh"

using namespace std::literals::string_view_literals;

HTTPXPP_NAMESPACE_START

static std::chrono::system_clock::time_point parse_http_datetime(const std::string &dt_str);

DocrootFile::DocrootFile(const std::filesystem::path &filename, const std::shared_ptr<const MimeTypeMap> &mime_type_map)
    :m_body()
    ,m_headers()
    ,m_mimeTypeMap(mime_type_map)
{
    std::ifstream infile(filename, std::ios::binary|std::ios_base::in);
    if (infile.good()) {
        infile.unsetf(std::ios_base::skipws);
        infile.seekg(0, std::ios_base::end);
        m_body.reserve(std::streamoff(infile.tellg()));
        infile.seekg(0, std::ios_base::beg);
        m_body.assign(std::istream_iterator<char>(infile), std::istream_iterator<char>());
    }
    m_headers["Content-Type"] = m_mimeTypeMap->typeForFilename(filename);
    m_headers["Content-Transfer-Encoding"] = "binary";

    try {
        auto metadata_filename = filename.parent_path() / ".metadata" / filename.filename();
        std::ifstream inmeta(metadata_filename);
        if (inmeta.good()) {
            for (char buf[2048]; inmeta.getline(buf, sizeof(buf));) {
                std::string_view line(buf);
                auto field_end = line.find_first_of(':');
                if (field_end != std::string_view::npos) {
                    auto field = line.substr(0,field_end);
                    auto value = line.substr(field_end+1);
                    auto pos = value.find_first_not_of(" \t\n\f\r\v"sv);
                    if (pos != std::string_view::npos) {
                        value.remove_prefix(pos);
                    } else {
                        value.remove_prefix(value.size());
                    }
                    pos = value.find_last_not_of(" \t\n\f\r\v"sv);
                    if (pos != std::string_view::npos) value.remove_suffix(value.size() - (pos + 1));
                    m_headers[std::string(field)] = std::string(value);
                }
            }
        }
    } catch (std::exception &ex) {
    }

    struct stat s;
    if (!lstat(filename.string().c_str(), &s)) {
        auto mod_time = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(s.st_mtim.tv_sec));
        if (s.st_mtim.tv_nsec) mod_time += std::chrono::seconds(1);
        m_headers["Last-Modified"] = std::format(std::locale("C"), "{0:%a}, {0:%d} {0:%b} {0:%Y} {0:%T} GMT", mod_time);

        m_headers["ETag"] = std::format("\"{}\"", calculate_hash(m_body));
    }

    m_headers["Content-Length"] = std::format("{}", m_body.size());
}

bool DocrootFile::isModified(const std::optional<std::string> &if_none_match,
                             const std::optional<std::string> &if_modified_since) const
{
    if (!if_none_match && !if_modified_since)
        return true;
    if (if_none_match && if_none_match.value() != m_headers.at("ETag"))
        return true;
    if (if_modified_since && parse_http_datetime(if_modified_since.value()) < parse_http_datetime(m_headers.at("Last-Modified")))
        return true;
    return false;
}

static std::chrono::system_clock::time_point parse_http_datetime(const std::string &dt_str)
{
    std::chrono::system_clock::time_point retval;
    std::istringstream iss{dt_str};
    iss.imbue(std::locale("C"));
    iss >> std::chrono::parse("%a, %d %b %Y %T", retval);
    if (!iss.fail()) {
        if (iss.peek() == '.') {
            /* extra fractions of a second */
            double frac;
            iss >> frac;
            retval += std::chrono::microseconds(static_cast<int>(frac*1000000.0));
        }
        char tz[5];
        iss.get(tz, sizeof(tz));
        if (std::string(tz) != " GMT") {
            retval = std::chrono::system_clock::time_point();
        }
    }
    return retval;
}

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
