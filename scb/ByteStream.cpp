#include "ByteStream.h"

#include <type_traits>

using namespace scb;

ByteStream::ByteStream(Bytes const &bytes)
    : buffer_(bytes)
    , pos_(0)
{}

ByteStream::ByteStream(Bytes &&bytes)
    : buffer_(std::move(bytes))
    , pos_(0)
{}

inline Bytes const& ByteStream::buffer() const noexcept {
    if (std::holds_alternative<std::reference_wrapper<Bytes const>>(buffer_))
        return std::get<std::reference_wrapper<Bytes const>>(buffer_);
    else //if (std::holds_alternative<Bytes>(buffer_))
        return std::get<Bytes>(buffer_);
}

int8_t ByteStream::next_i8() { return next_u8(); }
uint8_t ByteStream::next_u8() {
    if (eob())
        throw std::runtime_error("end of ByteStream buffer");
    return buffer()[pos_++];
}

int16_t ByteStream::next_i16(Endian endian) { return next_u16(endian); }
uint16_t ByteStream::next_u16(Endian endian) {
    auto buf = buffer();
    auto a = next_u8();
    auto b = next_u8();
    if (endian == Endian::Big) {
        return (a << 8) | b;
    } else {
        return a | (b << 8);
    }
}

int32_t ByteStream::next_i32(Endian endian) { return next_u32(endian); }
uint32_t ByteStream::next_u32(Endian endian) {
    auto buf = buffer();
    auto a = next_u16(endian);
    auto b = next_u16(endian);
    if (endian == Endian::Big) {
        return (a << 16) | b;
    } else {
        return a | (b << 16);
    }
}

Bytes ByteStream::next_bytes(size_t length) {
    if (eob())
        throw std::runtime_error("end of ByteStream buffer");
    auto buf = buffer();
    auto begin = buf.begin() + pos_;
    auto end = begin;
    if (pos_ + length <= buf.size())
        end += length;
    else
        end = buf.end();
    pos_ += length;
    return Bytes(begin, end);
}
