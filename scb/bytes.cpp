#include "bytes.h"

using namespace scb;

Bytes::Bytes(size_t size)
    : std::vector<Byte>(size, 0)
{}

Bytes::Bytes(Byte byte, size_t count)
    : std::vector<Byte>(count, byte)
{}

Bytes::Bytes(Byte const *begin, Byte const *end)
    : std::vector<Byte>(begin, end)
{}

Bytes::Bytes(std::vector<Byte>::const_iterator begin, std::vector<Byte>::const_iterator end)
    : std::vector<Byte>(begin, end)
{}

Bytes::Bytes(std::initializer_list<Byte> list)
    : std::vector<Byte>(list.begin(), list.end())
{}

Bytes::Bytes(StringAs as, char const *string) {
    switch (as) {
        default:
            std::vector<Byte>(from_raw_string(string));
            break;
        case Hex:
            std::vector<Byte>(from_hex_string(string));
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
        b[i] = (*this)[i] & rhs[i];
    return b;
}

Bytes scb::Bytes::operator|(Bytes const & rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = (*this)[i] | rhs[i];
    return b;
}

Bytes scb::Bytes::operator^(Bytes const & rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = (*this)[i] ^ rhs[i];
    return b;
}

Bytes& scb::Bytes::operator&=(Bytes const & rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    for (size_t i = 0; i < size(); i++)
        (*this)[i] &= rhs[i];
    return *this;
}

Bytes& scb::Bytes::operator|=(Bytes const & rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    for (size_t i = 0; i < size(); i++)
        (*this)[i] |= rhs[i];
    return *this;
}

Bytes & scb::Bytes::operator^=(Bytes const & rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    for (size_t i = 0; i < size(); i++)
        (*this)[i] ^= rhs[i];
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

std::vector<Byte> Bytes::from_raw_string(char const *string) {
    return std::vector<Byte>(string, string + strlen(string));
}

std::vector<Byte> Bytes::from_hex_string(char const *string) {
    size_t length = strlen(string);
    if (length & 1)
        throw std::runtime_error("invalid hex string");

    std::vector<Byte> bytes(length / 2);

    for (size_t i = 0; i < length; i += 2) {
        bytes[i / 2] = (
            hex_char_to_nibble(string[i]) << 8) |
            hex_char_to_nibble(string[i + 1]
            );
    }

    return bytes;
}
