#include "bytes.h"

#include <sstream>
#include <iomanip>

using namespace scb;

Bytes::Bytes()
    : std::vector<Byte>()
{}

Bytes::Bytes(StringAs as, char const *string)
    : std::vector<Byte>(as == Hex ? from_hex_string(string)
                                  : from_raw_string(string))
{}

Bytes::Bytes(StringAs as, std::string const &string)
    : Bytes(as, string.c_str())
{}

Bytes Bytes::operator&(Bytes const &rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = (*this)[i] & rhs[i];
    return b;
}

Bytes Bytes::operator|(Bytes const &rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = (*this)[i] | rhs[i];
    return b;
}

Bytes Bytes::operator^(Bytes const &rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = (*this)[i] ^ rhs[i];
    return b;
}

Bytes& Bytes::operator&=(Bytes const &rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    for (size_t i = 0; i < size(); i++)
        (*this)[i] &= rhs[i];
    return *this;
}

Bytes& Bytes::operator|=(Bytes const &rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    for (size_t i = 0; i < size(); i++)
        (*this)[i] |= rhs[i];
    return *this;
}

Bytes & Bytes::operator^=(Bytes const &rhs) {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    for (size_t i = 0; i < size(); i++)
        (*this)[i] ^= rhs[i];
    return *this;
}

Byte Bytes::hex_char_to_nibble(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'f' + 10;
    throw std::runtime_error("invalid hex character");
}

bool scb::Bytes::is_hex_char(char c) {
    return (c >= '0' && c <= '9')
        || (c >= 'A' && c <= 'F')
        || (c >= 'a' && c <= 'f');
}

std::vector<Byte> Bytes::from_raw_string(char const *string) {
    return std::vector<Byte>(string, string + strlen(string));
}

std::vector<Byte> Bytes::from_hex_string(char const *string) {
    std::vector<Byte> bytes;
    size_t i = 0;
    while (true) {
        char a = string[i];
        if (!a)
            break;
        i++;
        if (!is_hex_char(a))
            continue;
        char b = string[i];
        if (!b)
            throw std::runtime_error("invalid hex string");
        i++;
        bytes.emplace_back(
            (hex_char_to_nibble(a) << 4) |
            (hex_char_to_nibble(b))
        );
    }

    return bytes;
}

std::ostream& operator<<(std::ostream &os, Bytes const &bytes) {
    std::ios::fmtflags flags(os.flags());
    os << std::hex << std::uppercase << std::setfill('0');
    for (auto b : bytes) {
        os << std::setw(2) << static_cast<int>(b) << ' ';
    }
    os.flags(flags);
    return os;
}

std::istream& operator>>(std::istream &is, scb::Bytes &bytes) {
    std::string str;
    getline(is, str);
    bytes = Bytes(scb::Bytes::Hex, str);
    return is;
}
