#include <clt/util.hpp>

namespace clt {
const std::string default_format_str<uint64_t>::value = "{:>016x}";
const std::string default_format_str<uint32_t>::value = "{:>08x}";
const std::string default_format_str<uint16_t>::value = "{:>04x}";
const std::string default_format_str<uint8_t>::value = "{:>02x}";
}