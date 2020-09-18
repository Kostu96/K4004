#pragma once
#include "static_pair.hpp"

#include <array>
#include <cstddef>

template<typename Key, typename Value, std::size_t Size>
class StaticMap
{
public:
    constexpr StaticMap() = default;

    constexpr Value& operator[](Key&& key) {
        //auto& e = find(key);
        //if (e != end()) return e.second;

        if (m_size >= Size) throw std::range_error("Exceeded StaticMap capacity!");

        m_data[m_size++] = std::move(StaticPair(std::forward(key), Value()));
        return m_data[m_size - 1].second;
    }

    constexpr auto begin() { return m_data.begin(); }
    constexpr auto end() { return m_data.begin() + m_size; }
    constexpr auto find(const Key& key) {
        for (auto& e : m_data)
            if (e->first == key)
                return e;
        return end();
    }
private:
    using storage_t = std::array<StaticPair<Key, Value>, Size>;
    

    std::size_t m_size = 0;
    storage_t m_data{};
};
