// Copyright 2026 smvax

#include <stdexcept>  // For exceptions
#include <iostream>  // For ostream
#include <algorithm>  // For max
#include <utility>  // For move
#include <string>  // For string
#include <vector>  // For vector

#include "../lib_bitfield/bitfield.h"

BitField::BitField(size_t length) : _length(length) {
  if (_length > 0) {
    size_t size = get_blocks_count();
    _data = new unsigned int[size]();  // Inits with zeros
  } else {
    _data = nullptr;
  }
}
BitField::BitField(const BitField& other) :
    _length(other._length) {
  if (_length > 0) {
    size_t size = get_blocks_count();
    _data = new unsigned int[size];
    std::copy(other._data, other._data + size, _data);
  } else {
    _data = nullptr;
  }
}
BitField::BitField(BitField&& other) noexcept
    : _length(other._length),
    _data(other._data) {
  other._length = 0;
  other._data = nullptr;
}

BitField::~BitField() {
    delete[] _data;
}

BitField BitField::operator|(const BitField& other) const {
  if (_length != other._length) {
    throw std::logic_error("ERROR: Lengths of bitfields must be same!");
  }
  BitField result(_length);
  size_t size = get_blocks_count();
  for (size_t i = 0; i < size; ++i) {
    result._data[i] = _data[i] | other._data[i];
  }
  return result;
}
BitField BitField::operator&(const BitField& other) const {
  if (_length != other._length) {
    throw std::logic_error("ERROR: Lengths of bitfields must be same!");
  }
  BitField result(_length);
  size_t size = get_blocks_count();
  for (size_t i = 0; i < size; ++i) {
    result._data[i] = _data[i] & other._data[i];
  }
  return result;
}
BitField BitField::operator~() const {
  BitField result(_length);
  size_t size = get_blocks_count();
  for (size_t i = 0; i < size; ++i) {
    result._data[i] = ~_data[i];
  }
  return result;
}
BitField& BitField::operator=(const BitField& other) {
  if (this != &other) {
    unsigned int* new_data = nullptr;
    if (other._length > 0) {
      size_t size = other.get_blocks_count();
      new_data = new unsigned int[size];
      std::copy(other._data, other._data + size, new_data);
    }
    delete[] _data;
    _data = new_data;
    _length = other._length;
  }
  return *this;
}
BitField& BitField::operator=(BitField&& other) noexcept {
  if (this != &other) {
    delete[] _data;
    _data = other._data;
    _length = other._length;
    other._data = nullptr;
    other._length = 0;
  }
  return *this;
}

std::ostream& operator<<(std::ostream& out, const BitField& bf) {
  size_t length = bf.get_length();
  for (size_t i = 0; i < length; ++i) {
    out << (bf.get_bit(i) ? '1' : '0');
  }
  return out;
}
std::istream& operator>>(std::istream& in, BitField& bf) {
  std::string s;
  if (in >> s) {
    size_t length = s.length();
    for (size_t i = 0; i < length; ++i) {
      if (s[i] != '0' && s[i] != '1') {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    BitField temp(length);
    for (size_t i = 0; i < length; ++i) {
      if (s[i] == '1') {
        temp.set_bit(i);
      }
    }
    bf = std::move(temp);
  }
  return in;
}
