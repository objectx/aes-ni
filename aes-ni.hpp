#pragma once

#include <array>
#include <cstdint>
#include <iomanip>
#include <ostream>
#include <sstream>

#include <fmt/format.h>

namespace clt {
namespace aes128 {
constexpr size_t block_bytes = 16;
constexpr size_t key_bytes = 16;
constexpr uint8_t zero_key[key_bytes] = {0};
constexpr size_t num_rounds = 10;
} // namespace aes128
class AES128 {
    uint8_t expanded_keys_[aes128::block_bytes * 2 * aes128::num_rounds];

public:
    using block_t = std::array<uint8_t, aes128::block_bytes>;
    using key_t = std::array<uint8_t, aes128::key_bytes>;
    explicit AES128(const void *key) noexcept;
    explicit AES128(const key_t &key) : AES128(key.data()) {}
    explicit AES128() : AES128(aes128::zero_key) {}
    friend std::ostream &operator<<(std::ostream &ost, const AES128 &x);
    void enc(void *out, const void *in) const noexcept;
    void enc(void *out, const void *in, const size_t num_blocks) const noexcept;
    void dec(void *out, const void *in) const noexcept;
    void dec(void *out, const void *in, const size_t num_blocks) const noexcept;
    auto ctr_stream(void *out, const uint64_t num_blocks,
                    const uint64_t start_count) const noexcept
        -> decltype(num_blocks + start_count);
};

class MMO128 {
    uint8_t expanded_keys_[aes128::block_bytes * (aes128::num_rounds + 1)];

public:
    explicit MMO128(const void *key) noexcept;
    explicit MMO128() : MMO128(aes128::zero_key) {}
    friend std::ostream &operator<<(std::ostream &ost, const MMO128 &x);
    void operator()(void *out, const void *in) const noexcept;
    void operator()(void *out, const void *in, const size_t num_blocks) const
        noexcept;
};

class AESPRF128 {
    uint8_t expanded_keys_[aes128::block_bytes * (aes128::num_rounds + 1)];

public:
    explicit AESPRF128(const void *key) noexcept;
    explicit AESPRF128() : AESPRF128(aes128::zero_key) {}
    friend std::ostream &operator<<(std::ostream &ost, const AESPRF128 &x);
    void operator()(void *out, const void *in) const noexcept;
    void operator()(void *out, const void *in, const size_t num_blocks) const
        noexcept;
    auto ctr_stream(void *out, const uint64_t num_blocks,
                    const uint64_t start_count) const noexcept
        -> decltype(num_blocks + start_count);
};
} // namespace clt

#include "aes-ni_impl/aes-ni_inline.hpp"

// vim: set expandtab :
