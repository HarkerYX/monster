//
// Copyright (c) 2016-present DeepGrace (complex dot invoke at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/deepgrace/monster
//

#ifndef CURRIED_HPP
#define CURRIED_HPP

#include <tuple>
#include <utility>
    
namespace monster
{
    template <typename F, typename... Args>
    class curry
    {
        public:
            using tuple_t = std::tuple<std::decay_t<Args>...>;

            curry(F func, Args... args) : func(func), args(to_tuple(std::move(args)...))
            {
            }

            curry(F func, std::tuple<Args...> args) : func(func), args(std::move(args))
            {
            }

            template <typename... T>
            auto operator()(T&&... t) const
            {
                auto tuples = std::tuple_cat(args, to_tuple(std::forward<T>(t)...));
        
                if constexpr(std::is_invocable_v<F, Args..., T...>)
                   return std::apply(func, tuples);
                else
                   return curry<F, Args..., T...>(func, tuples);
            }

        private:

            template <typename... T>
            static auto to_tuple(T&&... t)
            {
                return std::tuple<std::decay_t<T>...>(std::forward<T>(t)...);
            }

            F func;
            std::tuple<Args...> args;
    };
}
    
#endif
