# License: 5G-MAG Public License (v1.0)
# Copyright: (C) 2026 British Broadcasting Corporation
# Author(s): David Waring <david.waring2@bbc.co.uk>
#
# For full license terms please see the LICENSE file distributed with this
# program. If this file is missing then the license can be retrieved from
# https://drive.google.com/file/d/1cinCiA778IErENZ3JN52VFW-1ffHpx7Z/view

# Meson module fs and its functions like fs.hash_file require atleast meson 0.59.0 

project('rt-common-shared', 'c', 'cpp', 
    version : '0.1.0',
    license : '5G-MAG Public',
    meson_version : '>= 1.4.0',
    default_options : [
        'c_std=gnu99',
        'cpp_std=gnu++20',
    ],
)

cplusplus = meson.get_compiler('cpp')
if not cplusplus.links('''#include <chrono>

int main(int argc, char *argv[])
{
  std::chrono::system_clock::time_point tmptime;
  std::chrono::parse("%Y", tmptime);
  return 0;
}
''', name: 'STL contains std::chrono::parse')
  error('std::chrono::parse function not found in STL, please use gcc 14.1 or later as the compiler')
endif

subdir('lib')
