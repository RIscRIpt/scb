#include "Bytes.h"

#include <sstream>

using namespace scb;

Bytes::Bytes()
    : std::vector<Byte>()
{}

Bytes::Bytes(char const *string, StringAs as)
    : std::vector<Byte>(as == Hex ? from_hex_string(string)
                                  : from_raw_string(string))
{}

Bytes::Bytes(wchar_t const *string, StringAs as)
    : std::vector<Byte>(as == Hex ? from_hex_string(string)
                                  : from_raw_string(string))
{}

Bytes::Bytes(std::string const &string, StringAs as)
    : Bytes(string.c_str(), as)
{}

Bytes::Bytes(std::wstring const &string, StringAs as)
    : Bytes(string.c_str(), as)
{}

Bytes::Bytes(std::initializer_list<Bytes> list)
    : std::vector<Byte>(from_list_of_bytes(list))
{}

Bytes Bytes::bytes(size_t offset, size_t length) const {
    if (offset + length > size())
        throw std::runtime_error("out of range");
    return Bytes(begin() + offset, begin() + offset + length);
}

Bytes Bytes::left(size_t length) const {
    return bytes(0, length);
}

Bytes Bytes::right(size_t length) const {
    return bytes(size() - length, length);
}

Bytes Bytes::concat(Bytes const &bytes) const {
    auto result = Bytes(size() + bytes.size());
    std::copy(begin(), end(), result.begin());
    std::copy(bytes.begin(), bytes.end(), result.begin() + size());
    return result;
}

Bytes& Bytes::join(Bytes const &bytes) {
    resize(size() + bytes.size());
    std::copy(bytes.begin(), bytes.end(), end() - bytes.size());
    return *this;
}

Bytes Bytes::operator&(Bytes const &rhs) const {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = (*this)[i] & rhs[i];
    return b;
}

Bytes Bytes::operator|(Bytes const &rhs) const {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = (*this)[i] | rhs[i];
    return b;
}

Bytes Bytes::operator^(Bytes const &rhs) const {
    if (size() != rhs.size())
        throw std::runtime_error("sizes do not match");
    Bytes b(size());
    for (size_t i = 0; i < size(); i++)
        b[i] = (*this)[i] ^ rhs[i];
    return b;
}

Bytes Bytes::operator+(Bytes const &rhs) const {
    return concat(rhs);
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

Bytes& Bytes::operator+=(Bytes const &rhs) {
    return join(rhs);
}

char Bytes::to_dump_char(char c) {
    if (c >= ' ' && c <= '~')
        return c;
    return '.';
}

Byte Bytes::hex_char_to_nibble(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    throw std::runtime_error("invalid hex character");
}

std::vector<Byte> Bytes::from_raw_string(char const *string) {
    return std::vector<Byte>(string, string + strlen(string));
}

std::vector<Byte> Bytes::from_raw_string(wchar_t const *string) {
    return std::vector<Byte>(reinterpret_cast<char const*>(string), reinterpret_cast<char const*>(string + wcslen(string)));
}

std::vector<Byte> Bytes::from_list_of_bytes(std::initializer_list<Bytes> list) {
    size_t total_size = 0;
    for (auto b : list)
        total_size += b.size();
    std::vector<Byte> bytes(total_size);
    size_t copied = 0;
    for (auto b : list) {
        std::copy(b.begin(), b.end(), bytes.begin() + copied);
        copied += b.size();
    }
    return bytes;
}

std::ostream& operator<<(std::ostream &os, Bytes const &bytes) {
    if (bytes.size() <= 16)
        bytes.print(os, "");
    else
        bytes.dump(os);
    return os;
}

std::istream& operator>>(std::istream &is, scb::Bytes &bytes) {
    std::string str;
    getline(is, str);
    bytes = str;
    return is;
}
