#pragma once

template<typename T1, typename T2>
struct StaticPair
{
    constexpr StaticPair() = default;
    constexpr StaticPair(const StaticPair&) = default;
    constexpr StaticPair(StaticPair&&) = default;

    constexpr StaticPair(T1&& val1, T2&& val2) noexcept :
        first(std::forward(val1)),
        second(std::forward(val2)) {}

    constexpr StaticPair& operator=(const StaticPair& other) noexcept {
        first = other.first;
        second = other.second;
        return *this;
    }

    constexpr StaticPair& operator=(StaticPair&& other) noexcept {
        first = std::forward(other.first);
        second = std::forward(other.second);
        return *this;
    }

    T1 first{};
    T2 second{};
};
