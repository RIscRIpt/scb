#pragma once

#include <vector>
#include <string>

namespace scb {

    using Byte = unsigned char;

    class Bytes {
    public:
        enum StringAs {
            Raw,
            Hex,
        };

        using container = std::vector<Byte>;

        Bytes(size_t size);
        Bytes(Byte byte, size_t count);

        Bytes(container const &bytes);
        Bytes(container &&bytes);

        Bytes(StringAs as, char const *string);
        Bytes(StringAs as, std::string const &string);

        inline size_t size() const { return bytes_.size(); }

        inline Byte operator[](size_t i) const { return bytes_[i]; }
        inline Byte& operator[](size_t i) { return bytes_[i]; }

        inline bool operator==(Bytes const &rhs) { return bytes_ == rhs.bytes_; }
        inline bool operator!=(Bytes const &rhs) { return bytes_ != rhs.bytes_; }

        Bytes operator&(Bytes const &rhs);
        Bytes operator|(Bytes const &rhs);
        Bytes operator^(Bytes const &rhs);

        Bytes& operator&=(Bytes const &rhs);
        Bytes& operator|=(Bytes const &rhs);
        Bytes& operator^=(Bytes const &rhs);

    private:
        static Byte hex_char_to_nibble(char c);

        static container from_raw_string(char const *string);
        static container from_hex_string(char const *string);

        container bytes_;
    };

}