#ifndef __CONCEPTS_H__
#define __CONCEPTS_H__

#include <iterator>
#include <concepts>

template <typename T>
concept MatrixType = requires { std::semiregular<T>; };

template <typename T, typename T2>
concept PermittedType = std::constructible_from<T2, T> && std::convertible_to<T2, T>;

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <typename T>
concept NumericMatrix = requires(T m) {
    {
        -m
    } -> Numeric;
};

template <typename T, typename T2>
concept MatrixEquality = std::equality_comparable<T> && std::equality_comparable<T2> && std::equality_comparable_with<T, T2>;

template <typename T, typename T2>
concept MatrixSum = requires(T a, T2 b) {
    PermittedType<T, T2>;
    {
        a + b
    } -> std::convertible_to<T>;
};

template <typename T, typename T2>
concept MatrixSub = requires(T a, T2 b) {
    PermittedType<T, T2>;
    {
        a - b
    } -> std::convertible_to<T>;
};

template <typename T, typename T2>
concept MatrixMul = requires(T a, T2 b) {
    PermittedType<T, T2>;
    MatrixSum<T, T2>;
    {
        a *b
    } -> std::convertible_to<T>;
};

template <typename T>
concept MatrixFloatPoint = requires {
    std::floating_point<T>;
};

template <typename T, typename T2>
concept MatrixDiv = requires(T a, T2 b) {
    MatrixFloatPoint<T>;
    MatrixFloatPoint<T2>;
    MatrixSum<T, T2>;
    MatrixMul<T, T2>;
    {
        a / b
    } -> std::convertible_to<T>;
};

template <typename T>
concept FriendlyContainer = requires(T &u) {
    {
        u.begin()
    } -> std::input_iterator;
    {
        u.end()
    } -> std::sentinel_for<decltype(u.begin())>;

    std::constructible_from<T, typename std::iterator_traits<decltype(u.begin())>::reference>;
};

template <typename T, typename T2>
concept PermittedContainer = requires(T2 &u) {
    FriendlyContainer<T2>;
    {
        u.get_rows()
    } noexcept -> std::same_as<typename T::size_type>;
    {
        u.get_cols()
    } noexcept -> std::same_as<typename T::size_type>;
};

#endif // __CONCEPTS_H__