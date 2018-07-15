#include "Bytes.h"

#include <sstream>
#include <iomanip>

using namespace scb;

Bytes::Bytes()
    : std::vector<Byte>()
{}

Bytes::Bytes(char const *string, StringAs as)
    : std::vector<Byte>(as == Hex ? from_hex_string(string)
                                  : from_raw_string(string))
{}

Bytes::Bytes(std::string const &string, StringAs as)
    : Bytes(string.c_str(), as)
{}

Bytes::Bytes(std::initializer_list<Bytes> list)
    : std::vector<Byte>(from_list_of_bytes(list))
{}

void Bytes::dump(std::ostream &os) const {
    std::ios::fmtflags flags(os.flags());
    os << std::hex << std::uppercase << std::setfill('0');

    os << "      00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F       ASCII\n";
    for (size_t i = 0; i < size(); i += 16) {
        os << std::setw(4) << i << "  ";
        for (size_t j = i; j < i + 16 && j < size(); j++) {
            os << std::setw(2) << static_cast<int>((*this)[j]) << ' ';
        }
        if (i + 16 > size()) {
            for (size_t j = size() % 16; j < 16; j++)
                os << "   ";
        }
        os << ' ';
        for (size_t j = i; j < i + 16 && j < size(); j++) {
            os << to_dump_char((*this)[j]);
        }
        os << '\n';
    }

    os.flags(flags);
}

void Bytes::print(std::ostream &os, char const *separator) const {
    std::ios::fmtflags flags(os.flags());
    os << std::hex << std::uppercase << std::setfill('0');
    for (size_t i = 0; i < size() - 1; i++)
        os << std::setw(2) << static_cast<int>((*this)[i]) << separator;
    os << std::setw(2) << static_cast<int>(back());
    os.flags(flags);
}

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
