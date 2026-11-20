/******************************************************************************
 * 5G-MAG Reference Tools: HTTPx Server: DirectoryIndexHandler class
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
#include <sys/types.h>
#include <dirent.h>

#include <chrono>
#include <filesystem>
#include <format>
#include <string>

#include "common.hh"
#include "DocrootHTTPRequestHandler.hh"
#include "HTTPResponse.hh"
#include "HTTPServer.hh"

#include "DirectoryIndexHandler.hh"

HTTPXPP_NAMESPACE_START

HTTPResponse DirectoryIndexHandler::makeResponseForDir(const std::string &dir_path, const std::string &url_path, const HTTPServer &server)
{
    std::string body = std::format("<html><head><title>Index of {0}</title></head><body><h1>Index of {0}</h1><hr/><table><tr><th>Name</th><th>Type</th><th>Modified</th></tr>", url_path);
    std::filesystem::path url(url_path);
    std::string link_prefix;
    if (url.has_filename()) {
        link_prefix = url.filename().string() + "/";
    }
    auto dir = opendir(dir_path.c_str());
    if (dir) {
        for (auto entry = readdir(dir); entry; entry = readdir(dir)) {
            std::string name{entry->d_name};
            if (name.starts_with('.') && name != "..") continue;
            if (name == ".." && url_path == "/") continue;
            std::string link_postfix;
            std::string typ;
            std::string mod_time;
            bool can_link = false;
            switch (entry->d_type) {
            case DT_BLK:
                typ = "Blk";
                break;
            case DT_CHR:
                typ = "Chr";
                break;
            case DT_DIR:
                typ = "Dir";
                link_postfix = "/";
                can_link = true;
                break;
            case DT_FIFO:
                typ = "Fif";
                break;
            case DT_LNK:
                typ = "Lnk";
                break;
            case DT_REG:
                {
                    struct stat s;
                    auto entry_path = std::filesystem::path(dir_path) / name;
                    if (!lstat(entry_path.string().c_str(), &s)) {
                        auto modified = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(s.st_mtime));
                        modified += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds(s.st_mtim.tv_nsec));
                        mod_time = std::format("{0:%a}, {0:%d} {0:%b} {0:%Y} {0:%H}:{0:%M}:{0:%S} GMT", modified);
                    }
                    can_link = true;
                }
                break;
            case DT_SOCK:
                typ = "Skt";
                break;
            default:
                typ = "Unk";
                break;
            }
            if (can_link) {
                body += std::format("<tr><td><a href=\"{1}{0}{2}\">{0}</a></td><td>{3}</td><td>{4}</td></tr>", name, link_prefix, link_postfix, typ, mod_time);
            } else {
                body += std::format("<tr><td>{0}</td><td>{1}</td><td>{2}</td></tr>", name, typ, mod_time);
            }
        }
        body += "</table></body></html>";
        closedir(dir);
    }
    return server.makeResponse(std::vector<char>(body.begin(), body.end())).addHeader("Content-Type", "text/html");
}

HTTPXPP_NAMESPACE_STOP

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
