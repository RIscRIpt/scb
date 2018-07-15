#pragma once

#include <vector>
#include <string>

namespace scb {

    using Byte = unsigned char;

    class Bytes : public std::vector<Byte> {
    public:
        enum StringAs {
            Raw,
            Hex,
        };

        using std::vector<Byte>::vector;

        Bytes();
        Bytes(char const *string, StringAs as = Hex);
        Bytes(std::string const &string, StringAs as = Hex);
        Bytes(std::initializer_list<Bytes> concat);

        void dump(std::ostream &os) const;
        void print(std::ostream &os, char const *separator = "") const;

        Bytes bytes(size_t offset, size_t length) const;
        Bytes left(size_t length) const;
        Bytes right(size_t length) const;

        Bytes concat(Bytes const &bytes) const;
        Bytes& join(Bytes const &bytes);

        Bytes operator&(Bytes const &rhs) const;
        Bytes operator|(Bytes const &rhs) const;
        Bytes operator^(Bytes const &rhs) const;

        Bytes operator+(Bytes const &rhs) const;

        Bytes& operator&=(Bytes const &rhs);
        Bytes& operator|=(Bytes const &rhs);
        Bytes& operator^=(Bytes const &rhs);

        Bytes& operator+=(Bytes const &rhs);

    private:
        static char to_dump_char(char c);
        static Byte hex_char_to_nibble(char c);
        static bool is_hex_char(char c);

        static std::vector<Byte> from_raw_string(char const *string);
        static std::vector<Byte> from_hex_string(char const *string);
        static std::vector<Byte> from_list_of_bytes(std::initializer_list<Bytes> list);
    };

}

std::ostream& operator<<(std::ostream &os, scb::Bytes const &bytes);
std::istream& operator>>(std::istream &is, scb::Bytes &bytes);
