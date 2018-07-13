#include "bytes.h"

using namespace scb;

Bytes::Bytes(size_t size)
    : bytes_(size, 0)
{}

scb::Bytes::Bytes(Byte byte, size_t count)
    : bytes_(count, byte)
{}

Bytes::Bytes(container const &bytes)
    : bytes_(bytes)
{}

Bytes::Bytes(container &&bytes)
    : bytes_(std::move(bytes))
{}

Bytes::Bytes(StringAs as, char const *string) {
    switch (as) {
        default:
            bytes_ = from_raw_string(string);
            break;
        case Hex:
            bytes_ = from_hex_string(string);
            break;
    }
}

Bytes::Bytes(StringAs as, std::string const &string)
    : Bytes(as, string.c_str())
{}

Bytes scb::Bytes::operator&(Bytes const &rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = bytes_[i] & rhs[i];
    return b;
}

Bytes scb::Bytes::operator|(Bytes const & rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = bytes_[i] | rhs[i];
    return b;
}

Bytes scb::Bytes::operator^(Bytes const & rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = bytes_[i] ^ rhs[i];
    return b;
}

Bytes& scb::Bytes::operator&=(Bytes const & rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    for (size_t i = 0; i < size(); i++)
        bytes_[i] &= rhs[i];
    return *this;
}

Bytes& scb::Bytes::operator|=(Bytes const & rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    for (size_t i = 0; i < size(); i++)
        bytes_[i] |= rhs[i];
    return *this;
}

Bytes & scb::Bytes::operator^=(Bytes const & rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    for (size_t i = 0; i < size(); i++)
        bytes_[i] ^= rhs[i];
    return *this;
}

Byte scb::Bytes::hex_char_to_nibble(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'f' + 10;
    throw std::runtime_error("invalid hex character");
}

Bytes::container Bytes::from_raw_string(char const *string) {
    return container(string, string + strlen(string));
}

Bytes::container Bytes::from_hex_string(char const *string) {
    size_t length = strlen(string);
    if (length & 1)
        throw std::runtime_error("invalid hex string");

    container bytes(length / 2);

    for (size_t i = 0; i < length; i += 2) {
        bytes[i / 2] = (
            hex_char_to_nibble(string[i]) << 8) |
            hex_char_to_nibble(string[i + 1]
            );
    }

    return bytes;
}
