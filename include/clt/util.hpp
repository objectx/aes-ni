#pragma once

#include <cassert>
#include <cstdint>
#include <sstream>
#include <array>
#include <type_traits>
#include <vector>

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace clt {

template <class ElemType> class default_format_str;
template <> class default_format_str<uint64_t> {
public:
    static const std::string value;
};
template <> class default_format_str<uint32_t> {
public:
    static const std::string value;
};
template <> class default_format_str<uint16_t> {
public:
    static const std::string value;
};
template <> class default_format_str<uint8_t> {
public:
    static const std::string value;
};

template <class IntType>
inline std::string
join(const IntType *in, const size_t &n,
     const std::string &format = default_format_str<IntType>::value)
{
    std::stringstream sst;
    if (n == 0) {
        return "";
    }
    sst << fmt::format(format, uint_fast64_t(in[0]));
    for (size_t i = 1; i < n; i++) {
        sst << ":" << fmt::format(format, uint_fast64_t(in[i]));
    }
    return sst.str();
}

template <class T, size_t N, class U = typename T::value_type>
inline auto join(const T (&in)[N],
                 const std::string &format = default_format_str<U>::value)
{
    return join(in, N, format);
}

template <class T, size_t N>
inline auto join(const std::array<T, N> &in,
                 const std::string &format = default_format_str<T>::value)
{
    return join(in.data(), N, format);
}

template <class T>
inline auto join(const std::vector<T> &in,
                 const std::string &format = default_format_str<T>::value)
{
    return join(in.data(), in.size(), format);
}

template <class T>
void init_iota(T &out, const size_t n, const size_t elem_per_block = 2)
{
    assert(0 < elem_per_block);
    assert((n * elem_per_block) <= size(out));
    for (size_t i = 0; i < n; i++) {
        out[i * elem_per_block] = i;
        for (size_t j = 1; j < elem_per_block; j++) {
            out[i * elem_per_block + j] = 0;
        }
    }
}

template <class T> auto all_xor(T &vec)
{
    using value_t = typename T::value_type;
    value_t v = 0;
    for (auto &&x : vec) {
        v ^= x;
    }
    return v;
}

inline auto int_ceiling(const size_t n, const size_t d)
{
    const auto q = n / d;
    const auto r = n % d;
    if (r == 0) {
        return q;
    } else {
        return q + 1;
    }
}
} // namespace clt
