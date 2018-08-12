#pragma once

#include <vector>
#include <string>
#include <iomanip>

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
        Bytes(wchar_t const *string, StringAs as = Hex);
        Bytes(std::string const &string, StringAs as = Hex);
        Bytes(std::wstring const &string, StringAs as = Hex);
        Bytes(std::initializer_list<Bytes> concat);

        template<typename Char>
        void dump(std::basic_ostream<Char, std::char_traits<Char>> &os) const {
            std::ios::fmtflags flags(os.flags());
            os << std::hex << std::uppercase << std::setfill(Char('0'));

            os << "      00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F       ASCII\r\n";
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
                os << "\r\n";
            }

            os.flags(flags);
        }

        template<typename Char>
        void print(std::basic_ostream<Char, std::char_traits<Char>> &os, Char const *separator = L"") const {
            if (empty())
                return;
            std::ios::fmtflags flags(os.flags());
            os << std::hex << std::uppercase << std::setfill(Char('0'));
            for (size_t i = 0; i < size() - 1; i++)
                os << std::setw(2) << static_cast<int>((*this)[i]) << separator;
            os << std::setw(2) << static_cast<int>(back());
            os.flags(flags);
        }

        template<typename Char>
        std::basic_string<Char> to_hex_string(Char const *separator = L"") const {
            std::basic_stringstream<Char, std::char_traits<Char>> os;
            print(os, separator);
            return os.str();
        }

        template<typename Char>
        std::basic_string<Char> to_hex_dump() const {
            std::basic_stringstream<Char, std::char_traits<Char>> os;
            dump(os);
            return os.str();
        }

        Bytes bytes(size_t offset, size_t length) const;
        Bytes left(size_t length) const;
        Bytes right(size_t length) const;

        Bytes concat(Bytes const &bytes) const;
        Bytes& join(Bytes const &bytes);

        bool all_ascii() const noexcept;
        char const* ascii();

        Bytes operator&(Bytes const &rhs) const;
        Bytes operator|(Bytes const &rhs) const;
        Bytes operator^(Bytes const &rhs) const;

        Bytes operator+(Bytes const &rhs) const;

        Bytes& operator&=(Bytes const &rhs);
        Bytes& operator|=(Bytes const &rhs);
        Bytes& operator^=(Bytes const &rhs);

        Bytes& operator+=(Bytes const &rhs);

    private:
        static bool is_ascii_char(char c);
        static char to_dump_char(char c);
        static Byte hex_char_to_nibble(char c);

        template<typename Char>
        static bool is_hex_char(Char c) {
            return (c >= '0' && c <= '9')
                || (c >= 'A' && c <= 'F')
                || (c >= 'a' && c <= 'f');
        }

        static std::vector<Byte> from_raw_string(char const *string);
        static std::vector<Byte> from_raw_string(wchar_t const *string);

        template<typename Char>
        std::vector<Byte> from_hex_string(Char const *string) {
            std::vector<Byte> bytes;
            size_t i = 0;
            while (true) {
                Char a = string[i];
                if (!a)
                    break;
                i++;
                if (!is_hex_char(a))
                    continue;
                Char b = string[i];
                if (!is_hex_char(b))
                    throw std::runtime_error("invalid hex string");
                i++;
                bytes.emplace_back(
                    (hex_char_to_nibble(static_cast<char>(a)) << 4) |
                    (hex_char_to_nibble(static_cast<char>(b)))
                );
            }

            return bytes;
        }

        static std::vector<Byte> from_list_of_bytes(std::initializer_list<Bytes> list);
    };

}

std::ostream& operator<<(std::ostream &os, scb::Bytes const &bytes);
std::istream& operator>>(std::istream &is, scb::Bytes &bytes);
