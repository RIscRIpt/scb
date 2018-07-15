#pragma once

#include "bytes.h"

#include <variant>
#include <cstdint>

namespace scb {

    class ByteStream {
    public:
        enum class Endian : bool {
            Little,
            Big,
        };

        ByteStream(Bytes const &bytes);
        ByteStream(Bytes &&bytes);

        inline Bytes const& buffer() const noexcept;
        inline bool eob() const noexcept { return pos_ >= buffer().size(); }

        int8_t next_i8();
        uint8_t next_u8();

        int16_t next_i16(Endian endian = Endian::Big);
        uint16_t next_u16(Endian endian = Endian::Big);

        int32_t next_i32(Endian endian = Endian::Big);
        uint32_t next_u32(Endian endian = Endian::Big);

        Bytes next_bytes(size_t length);

    private:
        std::variant<std::reference_wrapper<Bytes const>, Bytes> buffer_;
        size_t pos_;
    };

}
