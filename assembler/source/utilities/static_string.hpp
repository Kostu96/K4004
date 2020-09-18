#pragma once
#include <cstddef>

class StaticString
{
public:
    constexpr StaticString() = default;

    template<std::size_t N>
    constexpr StaticString(const char (&str)[N]) :
        m_size(N - 1), m_data(str) {}

    constexpr std::size_t size() { return m_size; }
    constexpr const char* c_str() { return m_data; }

    constexpr bool operator==(const StaticString& other) {
        if (m_size != other.m_size) return false;
        for (std::size_t i = 0; i < m_size; ++i)
            if (m_data[i] != other.m_data[i])
                return false;
        return true;
    }
private:
    std::size_t m_size = 0;
    const char* m_data = nullptr;
};
