//
// Created by doocm on 2022-06-06.
//

#include <cassert>
#include <cstdlib>
// ======================================================================================
// MIT License
//
// Copyright (c) 2022 Robin Söderholm
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
//         of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
//         to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//         copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
//         copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//         AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ======================================================================================

#include <etl/string_view.h>
#include <etl/variant.h>

#include <eetl/variant_visit.h>

struct variant_test_type1 {};

constexpr char const* get_string(variant_test_type1) noexcept {
    return "custom";
}

constexpr char const* get_string(int) noexcept {
    return "int";
}

constexpr char const* get_string(double) noexcept {
    return "double";
}

int main() {
    etl::variant<variant_test_type1, int, double> variant;
    char const* res1 = eetl::visit([] (auto const& v) {
        return get_string(v);
    }, variant);
    etl::string_view exp = get_string(variant_test_type1{});
    assert(res1 == exp);
    variant = 2.;
    exp = get_string(double{});
    res1 = eetl::visit([] (auto const& v) {
        return get_string(v);
    }, variant);
    assert(res1 == exp);
    return EXIT_SUCCESS;
}
