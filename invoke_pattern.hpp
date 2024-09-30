#pragma once
#include <concepts>
#include <type_traits>

template <typename>
struct is_invoke_pattern;

template <typename>
struct is_invoke_pattern
{
    static constexpr bool value = false;

    template <typename>
    static constexpr bool matches = false;
};

template <typename _Res, typename... Args>
struct is_invoke_pattern<_Res(Args...)>
{
    static constexpr bool value = true;

    template <typename F>
    static constexpr bool matches = std::invocable<F, Args...> && std::same_as<std::invoke_result_t<F, Args...>, _Res>;
};

template <typename T>
concept invoke_pattern = is_invoke_pattern<T>::value;

template <typename F, typename T>
concept single_pattern_invocable = is_invoke_pattern<T>::template matches<F>;

template <typename T, typename... Patterns>
concept pattern_invokable = (single_pattern_invocable<T, Patterns> && ...);

struct MultiCallable
{
    void operator()(int, int);
    int operator()(float);
};

static_assert(pattern_invokable<MultiCallable, void(int, int), int(float)>);
static_assert(!pattern_invokable<MultiCallable, void(int, int), int(float), void(float)>);
