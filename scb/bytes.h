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
        static bool is_hex_char(char c);

        static std::vector<Byte> from_raw_string(char const *string);
        static std::vector<Byte> from_hex_string(char const *string);
    };

}

std::ostream& operator<<(std::ostream &os, scb::Bytes const &bytes);
std::istream& operator>>(std::istream &is, scb::Bytes &bytes);
