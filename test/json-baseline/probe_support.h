// Copyright 2026 ModSecurity contributors.
#ifndef TEST_JSON_BASELINE_PROBE_SUPPORT_H_
#define TEST_JSON_BASELINE_PROBE_SUPPORT_H_

#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>

namespace json_test {
// Distinguishes test-protocol/API misuse from an intentional JSON rejection.
class ProbeError : public std::runtime_error {
 public:
    using std::runtime_error::runtime_error;
};

inline unsigned int hexDigit(char digit) {
    if (digit >= '0' && digit <= '9') {
        return static_cast<unsigned int>(digit - '0');
    }
    if (digit >= 'a' && digit <= 'f') {
        return static_cast<unsigned int>(digit - 'a') + 10;
    }
    if (digit >= 'A' && digit <= 'F') {
        return static_cast<unsigned int>(digit - 'A') + 10;
    }
    throw ProbeError("invalid hex input");
}

inline std::string unhex(std::string_view text) {
    if (text.size() % 2 != 0) {
        throw ProbeError("odd hex input");
    }
    std::string result;
    result.reserve(text.size() / 2);
    for (size_t i = 0; i < text.size(); i += 2) {
        const auto byte = static_cast<std::byte>(
            hexDigit(text[i]) * 16 + hexDigit(text[i + 1]));
        result.push_back(static_cast<char>(
            std::to_integer<unsigned char>(byte)));
    }
    return result;
}

inline std::string hex(std::string_view text) {
    constexpr std::string_view digits = "0123456789abcdef";
    std::string result;
    for (char character : text) {
        const auto byte = static_cast<std::byte>(character);
        result += digits[std::to_integer<unsigned int>(byte >> 4)];
        result += digits[std::to_integer<unsigned int>(byte & std::byte{15})];
    }
    return result;
}
}  // namespace json_test

#endif  // TEST_JSON_BASELINE_PROBE_SUPPORT_H_
