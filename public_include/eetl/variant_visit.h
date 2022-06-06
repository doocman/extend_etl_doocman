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

#pragma once

#include <utility>
#include <type_traits>

#include <exception>

#include <etl/variant.h>

namespace eetl {
    class bad_variant_access : public std::exception {
    public:
        const char *what() const override {
            return "Bad variant access";
        }
    };
    namespace details {
        template<typename TCallable, typename T>
        struct single_visit_result_type {
            using type = decltype(std::declval<TCallable>()(std::declval<T>()));
        };
        template<typename TCallable, typename T>
        using single_visit_result_type_t = typename single_visit_result_type<TCallable, T>::type;
        template<typename, typename>
        struct visit_result;
        template<typename TCallable, typename... Ts>
        struct visit_result<TCallable, etl::variant<Ts...> &&> {
            using type = typename std::common_type<single_visit_result_type_t<TCallable, Ts &&>...>::type;
        };
        template<typename TCallable, typename... Ts>
        struct visit_result<TCallable, etl::variant<Ts...> &> {
            using type = typename std::common_type<single_visit_result_type_t<TCallable, Ts &>...>::type;
        };
        template<typename TCallable, typename... Ts>
        struct visit_result<TCallable, etl::variant<Ts...> const &&> {
            using type = typename std::common_type<single_visit_result_type_t<TCallable, Ts const &&>...>::type;
        };
        template<typename TCallable, typename... Ts>
        struct visit_result<TCallable, etl::variant<Ts...> const &> {
            using type = typename std::common_type<single_visit_result_type_t<TCallable, Ts const &&>...>::type;
        };
        template<typename TCallable, typename... Ts>
        using visit_result_t = typename visit_result<TCallable, Ts...>::type;

        template<typename TRet, typename TCallable, typename TVariant, std::size_t tIndex>
        constexpr TRet do_visit_single(TCallable &&f, TVariant &&v) {
            return static_cast<TCallable &&>(f)(etl::get<tIndex>(static_cast<TVariant &&>(v)));
        }

        template<typename TRet, typename TCallable, typename TVariant, typename tIndex>
        struct do_visit_single_t {
            static constexpr TRet call(TCallable &&f, TVariant &&v) {
                return static_cast<TCallable &&>(f)(etl::get<tIndex>(static_cast<TVariant &&>(v)));;
            }
        };

        template<typename TRet, typename TCallable, typename TVariant, std::size_t... tIndices>
        constexpr TRet do_visit(TCallable &&f, TVariant &&v, std::index_sequence<tIndices...>) {
            if (!v.valueless_by_exception()) {
                constexpr typename std::add_pointer<TRet(TCallable &&, TVariant &&)>::type jmp_table[]{
                        &do_visit_single<TRet, TCallable, TVariant, tIndices>...
                };
                return jmp_table[v.index()](static_cast<TCallable &&>(f), static_cast<TVariant &&>(v));
            }
            // What to do when we don't have exceptions?
            throw bad_variant_access{};
        }
    }

    template<typename TCallable, typename TVariant>
    constexpr auto visit(TCallable &&f, TVariant &&v) -> details::visit_result_t<TCallable &&, TVariant &&> {
        constexpr std::size_t variants = etl::variant_size<std::remove_reference_t<TVariant>>::value;
        return details::do_visit<details::visit_result_t<TCallable &&, TVariant &&>>(static_cast<TCallable &&>(f),
                                                                                     static_cast<TVariant &&>(v),
                                                                                     std::make_index_sequence<variants>{});
    }
}

