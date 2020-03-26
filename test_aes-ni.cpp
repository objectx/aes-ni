#include <vector>
#include <numeric>
#include <algorithm>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <clt/aes-ni.hpp>
#include <clt/rng.hpp>

using namespace std;
using namespace clt;
using namespace clt::rng;

template <class T>
void init_iota(T &&out, const size_t n, const size_t elem_per_block = 2)
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

void init_random(void *out, const size_t byte_size)
{
    const auto status = rng_global(out, byte_size);
    if (!status) {
        fmt::print(cerr, "ERROR!! init_random is failed.\n");
        abort();
    }
}

void test_aes_ni()
{
    constexpr uint8_t sample_key[clt::aes128::key_bytes] = {
        // 2b7e151628aed2a6abf7158809cf4f3c
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
    };
    cout << "A sample key = " << clt::join(sample_key) << endl;
    clt::AES128 cipher(sample_key);
    cout << cipher << endl;
    const uint8_t sample_plaintexts[] = {
        // 6bc1bee22e409f96e93d7e117393172a
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11,
        0x73, 0x93, 0x17, 0x2a,
        // ae2d8a571e03ac9c9eb76fac45af8e51
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac,
        0x45, 0xaf, 0x8e, 0x51,
        // 30c81c46a35ce411e5fbc1191a0a52ef
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19,
        0x1a, 0x0a, 0x52, 0xef,
        // f69f2445df4f9b17ad2b417be66c3710
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b,
        0xe6, 0x6c, 0x37, 0x10,
        //
    };
    const uint8_t sample_ciphertexts[] = {
        // 3ad77bb40d7a3660a89ecaf32466ef97
        0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3,
        0x24, 0x66, 0xef, 0x97,
        // f5d3d58503b9699de785895a96fdbaaf
        0xf5, 0xd3, 0xd5, 0x85, 0x03, 0xb9, 0x69, 0x9d, 0xe7, 0x85, 0x89, 0x5a,
        0x96, 0xfd, 0xba, 0xaf,
        // 43b1cd7f598ece23881b00e3ed030688
        0x43, 0xb1, 0xcd, 0x7f, 0x59, 0x8e, 0xce, 0x23, 0x88, 0x1b, 0x00, 0xe3,
        0xed, 0x03, 0x06, 0x88,
        // 7b0c785e27e8ad3f8223207104725dd4
        0x7b, 0x0c, 0x78, 0x5e, 0x27, 0xe8, 0xad, 0x3f, 0x82, 0x23, 0x20, 0x71,
        0x04, 0x72, 0x5d, 0xd4,
        //
    };
    assert(size(sample_plaintexts) == size(sample_ciphertexts));
    assert((size(sample_plaintexts) % clt::aes128::block_bytes) == 0);
    const auto num_blocks = size(sample_plaintexts) / clt::aes128::block_bytes;
    uint8_t sample_out[size(sample_plaintexts)];
    cout << "A sample plaintext = " << clt::join(sample_plaintexts) << endl;
    cout << "A sample ciphertext = " << clt::join(sample_ciphertexts) << endl;
    cipher.enc(sample_out, sample_plaintexts, num_blocks);
    cout << "enc(k, p) = " << clt::join(sample_out) << endl;
    for (size_t i = 0; i < size(sample_plaintexts); i++) {
        if (sample_ciphertexts[i] != sample_out[i]) {
            cout << "!!!!" << endl;
        }
    }
    cipher.dec(sample_out, sample_out, num_blocks);
    cout << "dec(k, c) = " << clt::join(sample_out) << endl;
    for (size_t i = 0; i < size(sample_plaintexts); i++) {
        if (sample_plaintexts[i] != sample_out[i]) {
            cout << "!!!!" << endl;
        }
    }
}

void test_mmo()
{
    constexpr uint8_t sample_key[clt::aes128::key_bytes] = {
        // 2b7e151628aed2a6abf7158809cf4f3c
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
    };
    cout << "A sample key = " << clt::join(sample_key) << endl;
    clt::MMO128 crh(sample_key);
    cout << crh << endl;
    const uint8_t sample_plaintexts[] = {
        // 6bc1bee22e409f96e93d7e117393172a
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11,
        0x73, 0x93, 0x17, 0x2a,
        // ae2d8a571e03ac9c9eb76fac45af8e51
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac,
        0x45, 0xaf, 0x8e, 0x51,
        // 30c81c46a35ce411e5fbc1191a0a52ef
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19,
        0x1a, 0x0a, 0x52, 0xef,
        // f69f2445df4f9b17ad2b417be66c3710
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b,
        0xe6, 0x6c, 0x37, 0x10,
        //
    };
    const uint8_t sample_ciphertexts[] = {
        // 3ad77bb40d7a3660a89ecaf32466ef97
        0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3,
        0x24, 0x66, 0xef, 0x97,
        // f5d3d58503b9699de785895a96fdbaaf
        0xf5, 0xd3, 0xd5, 0x85, 0x03, 0xb9, 0x69, 0x9d, 0xe7, 0x85, 0x89, 0x5a,
        0x96, 0xfd, 0xba, 0xaf,
        // 43b1cd7f598ece23881b00e3ed030688
        0x43, 0xb1, 0xcd, 0x7f, 0x59, 0x8e, 0xce, 0x23, 0x88, 0x1b, 0x00, 0xe3,
        0xed, 0x03, 0x06, 0x88,
        // 7b0c785e27e8ad3f8223207104725dd4
        0x7b, 0x0c, 0x78, 0x5e, 0x27, 0xe8, 0xad, 0x3f, 0x82, 0x23, 0x20, 0x71,
        0x04, 0x72, 0x5d, 0xd4,
        //
    };
    assert(size(sample_plaintexts) == size(sample_ciphertexts));
    assert((size(sample_plaintexts) % clt::aes128::block_bytes) == 0);
    const auto num_blocks = size(sample_plaintexts) / clt::aes128::block_bytes;
    uint8_t sample_out[size(sample_plaintexts)];
    cout << "A sample plaintext = " << clt::join(sample_plaintexts) << endl;
    crh(sample_out, sample_plaintexts, num_blocks);
    cout << "crh(p) = " << clt::join(sample_out) << endl;
    for (size_t i = 0; i < size(sample_plaintexts); i++) {
        if ((sample_plaintexts[i] ^ sample_ciphertexts[i]) != sample_out[i]) {
            cout << "!!!!" << endl;
        }
    }
}

void test_aes_ctr_stream()
{
    clt::AES128::key_t key;
    init_random(key.data(), key.size());
    clt::AES128 cipher(key);
    vector<uint64_t> buff, enc_buff, dec_buff, str_buff;
    constexpr size_t num_blocks = 1 << 10;
    static_assert((clt::aes128::block_bytes % sizeof(uint64_t)) == 0);
    buff.resize(num_blocks * clt::aes128::block_bytes / sizeof(uint64_t));
    enc_buff.resize(buff.size());
    dec_buff.resize(buff.size());
    str_buff.resize(buff.size());
    init_iota(buff, num_blocks, 2);
    fmt::print("buff[:8] = {}\n", clt::join(buff.data(), 8));
    fmt::print("buff[:64] = {}\n", clt::join((uint8_t *)buff.data(), 64));
    cipher.enc(enc_buff.data(), buff.data(), num_blocks);
    cipher.ctr_stream(str_buff.data(), num_blocks, 0);
    for (size_t i = 0; i < enc_buff.size(); i++) {
        if (enc_buff[i] != str_buff[i]) {
            fmt::print("!!!!\n");
        }
    }
}

void test_shuffle()
{
    // TODO
    vector<uint32_t> vec(1 << 10);
    iota(begin(vec), end(vec), 0);
    fmt::print("Before: vec = {}\n", clt::join(vec.data(), 20));
    shuffle(vec.data(), vec.size(), rng_global);
    fmt::print(" After: vec = {}\n", clt::join(vec.data(), 20));
    const auto vec_size = vec.size();
    for (size_t i = 0; i < vec_size; i++) {
        const auto at_v = find(begin(vec), end(vec), i);
        if (at_v == end(vec)) {
            throw runtime_error(fmt::format("Not found index: {}", i));
        }
    }
}

int main()
{
    test_aes_ni();
    test_mmo();
    test_aes_ctr_stream();
    test_shuffle();
    return 0;
}
// vim: set expandtab :
