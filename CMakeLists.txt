#
# MIT License
#
# Copyright (c) 2022 Robin Söderholm
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#


cmake_minimum_required(VERSION 3.5)

set(CURRENT_TARGET extend_etl.variant_visit)

option(EXTEND_ETL_BUIlD_TESTS "Add tests to build tree" OFF)

# eetl: extend etl.
add_library(${CURRENT_TARGET} INTERFACE)
add_library(eetl::${CURRENT_TARGET} ALIAS ${CURRENT_TARGET})

target_include_directories(${CURRENT_TARGET} INTERFACE
        public_include
        )

target_link_libraries(${CURRENT_TARGET}
        INTERFACE
        etl::etl
        )

#if (EXTEND_ETL_BUILD_TESTS)
    add_subdirectory(tests)
#endif ()
