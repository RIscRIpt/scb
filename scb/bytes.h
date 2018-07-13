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

        Bytes(size_t size = 0);
        Bytes(Byte byte, size_t count);

        Bytes(Byte const *begin, Byte const *end);
        Bytes(std::vector<Byte>::const_iterator begin, std::vector<Byte>::const_iterator end);
        Bytes(std::initializer_list<Byte> list);

        Bytes(StringAs as, char const *string);
        Bytes(StringAs as, std::string const &string);

        Bytes operator&(Bytes const &rhs);
        Bytes operator|(Bytes const &rhs);
        Bytes operator^(Bytes const &rhs);

        Bytes& operator&=(Bytes const &rhs);
        Bytes& operator|=(Bytes const &rhs);
        Bytes& operator^=(Bytes const &rhs);

    private:
        static Byte hex_char_to_nibble(char c);

        static std::vector<Byte> from_raw_string(char const *string);
        static std::vector<Byte> from_hex_string(char const *string);
    };

}