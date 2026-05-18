/******************************************************************************
 * 5G-MAG Reference Tools: SDP Library: SDP library unit tests
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

#include <rtsdp/SDP.hh>
#include <rtsdp/Originator.hh>
#include <rtsdp/ConnectionInformation.hh>
#include <rtsdp/MediaDescription.hh>
#include <rtsdp/TimingInformation.hh>

using namespace std::literals::chrono_literals;

LIBRTSDP_NAMESPACE_USING(SDP);
LIBRTSDP_NAMESPACE_USING(Originator);
LIBRTSDP_NAMESPACE_USING(ConnectionInformation);
LIBRTSDP_NAMESPACE_USING(MediaDescription);
LIBRTSDP_NAMESPACE_USING(TimingInformation);

int main(int argc, char *argv[])
{
    std::cout << "Testing librtsdp v" << SDP::library_version << std::endl;

    auto now = std::chrono::system_clock::now();
    auto sdp_now = std::chrono::time_point_cast<std::chrono::seconds>(now).time_since_epoch().count() + 2208988800;

    int tests = 0, successes = 0, failures = 0;

    // Test 1 - Empty SDP
    try {
        tests++;
        auto sdp = SDP::makeSDP();
        static_cast<std::string>(*sdp);
        failures++;
    } catch (std::out_of_range &ex) {
        successes++;
    }

    // Test 2 - Basic configuration, no media
    try {
        tests++;
        auto originator = Originator::makeOriginator("localhost", AF_INET6);
        auto connection = ConnectionInformation::makeConnectionInformation("localhost", AF_INET);
        auto timings = TimingInformation::makeTimingInformation(now, now + 6s);
        auto sdp = SDP::makeSDP(originator, "Session Name", timings);
        sdp->connectionInformation(connection);
        auto expected = std::format("v=0\r\no=- {} {} IN IP6 localhost\r\ns=Session Name\r\nc=IN IP4 localhost\r\nt={} {}\r\n", sdp->sessionId(), sdp->sessionVersion(), sdp_now, sdp_now + 6);
        auto sdp_record = static_cast<std::string>(*sdp);
        if (sdp_record != expected) {
            std::cerr << "Test 2: Unexpected SDP record:" << std::endl
                      << "        Expected:" << std::endl
                      << expected << std::endl
                      << "        Got:" << std::endl
                      << sdp_record << std::endl;
            failures++;
        } else {
            successes++;
        }
    } catch (std::out_of_range &ex) {
        std::cerr << "Test 2: Unexpected exception generating SDP: " << ex.what() << std::endl;
        failures++;
    }

    // Test 3
    try {
        tests++;
        auto originator = Originator::makeOriginator("localhost", AF_INET);
        auto connection = ConnectionInformation::makeConnectionInformation("localhost", AF_INET);
        auto timings = TimingInformation::makeTimingInformation(now, now + 6s);
        auto sdp = SDP::makeSDP(originator, "Session Name", timings, "Session Info", "http://example.com/service-description", "user@example.com", "+44 208 743 8000", connection);
        auto expected = std::format("v=0\r\no=- {} {} IN IP4 localhost\r\ns=Session Name\r\ni=Session Info\r\nu=http://example.com/service-description\r\ne=user@example.com\r\np=+44 208 743 8000\r\nc=IN IP4 localhost\r\nt={} {}\r\n", sdp->sessionId(), sdp->sessionVersion(), sdp_now, sdp_now + 6);
        auto sdp_record = static_cast<std::string>(*sdp);
        if (sdp_record != expected) {
            std::cerr << "Test " << tests << ": Unexpected SDP record:" << std::endl
                      << "        Expected:" << std::endl
                      << expected << std::endl
                      << "        Got:" << std::endl
                      << sdp_record << std::endl;
            failures++;
        } else {
            successes++;
        }
    } catch (std::out_of_range &ex) {
        std::cerr << "Test " << tests << ": Unexpected exception generating SDP: " << ex.what() << std::endl;
        failures++;
    }

    // Test 4
    try {
        tests++;
        auto originator = Originator::makeOriginator("localhost", AF_INET);
        auto connection = ConnectionInformation::makeConnectionInformation("localhost", AF_INET);
        auto timings = TimingInformation::makeTimingInformation(now, now + 6s);
        auto media = MediaDescription::makeMediaDescription("video", 1234, "RTP/AVP", "31");
        auto sdp = SDP::makeSDP(originator, "Session Name", timings, "", "", "", "", connection);
        sdp->mediaDescriptionAdd(media);
        auto expected = std::format("v=0\r\no=- {} {} IN IP4 localhost\r\ns=Session Name\r\nc=IN IP4 localhost\r\nt={} {}\r\nm=video 1234 RTP/AVP 31\r\n", sdp->sessionId(), sdp->sessionVersion(), sdp_now, sdp_now + 6);
        auto sdp_record = static_cast<std::string>(*sdp);
        if (sdp_record != expected) {
            std::cerr << "Test " << tests << ": Unexpected SDP record:" << std::endl
                      << "        Expected:" << std::endl
                      << expected << std::endl
                      << "        Got:" << std::endl
                      << sdp_record << std::endl;
            failures++;
        } else {
            successes++;
        }
    } catch (std::out_of_range &ex) {
        std::cerr << "Test " << tests << ": Unexpected exception generating SDP: " << ex.what() << std::endl;
        failures++;
    }

    // Test 5
    try {
        tests++;
        auto originator = Originator::makeOriginator("localhost", AF_INET);
        auto connection = ConnectionInformation::makeConnectionInformation("localhost", AF_INET);
        auto timings = TimingInformation::makeTimingInformation(now, now + 6s);
        auto media = MediaDescription::makeMediaDescription("video", 1234, "RTP/AVP", "31", 2);
        auto sdp = SDP::makeSDP(originator, "Session Name", timings, "", "", "", "", connection);
        sdp->mediaDescriptionAdd(media);
        auto expected = std::format("v=0\r\no=- {} {} IN IP4 localhost\r\ns=Session Name\r\nc=IN IP4 localhost\r\nt={} {}\r\nm=video 1234/2 RTP/AVP 31\r\n", sdp->sessionId(), sdp->sessionVersion(), sdp_now, sdp_now + 6);
        auto sdp_record = static_cast<std::string>(*sdp);
        if (sdp_record != expected) {
            std::cerr << "Test " << tests << ": Unexpected SDP record:" << std::endl
                      << "        Expected:" << std::endl
                      << expected << std::endl
                      << "        Got:" << std::endl
                      << sdp_record << std::endl;
            failures++;
        } else {
            successes++;
        }
    } catch (std::out_of_range &ex) {
        std::cerr << "Test " << tests << ": Unexpected exception generating SDP: " << ex.what() << std::endl;
        failures++;
    }

    // Test 6
    try {
        tests++;
        auto originator = Originator::makeOriginator("localhost", AF_INET);
        auto connection = ConnectionInformation::makeConnectionInformation("localhost", AF_INET);
        auto timings = TimingInformation::makeTimingInformation(now, now + 6s);
        auto media = MediaDescription::makeMediaDescription("video", 1234, "RTP/AVP", "31", 2);
        auto sdp = SDP::makeSDP(originator, "Session Name", timings);
        media->connectionInformationAdd(connection);
        sdp->mediaDescriptionAdd(media);
        auto expected = std::format("v=0\r\no=- {} {} IN IP4 localhost\r\ns=Session Name\r\nt={} {}\r\nm=video 1234/2 RTP/AVP 31\r\nc=IN IP4 localhost\r\n", sdp->sessionId(), sdp->sessionVersion(), sdp_now, sdp_now + 6);
        auto sdp_record = static_cast<std::string>(*sdp);
        if (sdp_record != expected) {
            std::cerr << "Test " << tests << ": Unexpected SDP record:" << std::endl
                      << "        Expected:" << std::endl
                      << expected << std::endl
                      << "        Got:" << std::endl
                      << sdp_record << std::endl;
            failures++;
        } else {
            successes++;
        }
    } catch (std::out_of_range &ex) {
        std::cerr << "Test " << tests << ": Unexpected exception generating SDP: " << ex.what() << std::endl;
        failures++;
    }

    std::cout << "Performed " << tests << " tests, " << successes << " passed, " << failures << " failed" << std::endl;

    return failures?1:0;
}

/* vim:ts=8:sts=4:sw=4:expandtab:
 */
