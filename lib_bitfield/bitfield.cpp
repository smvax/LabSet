// Copyright 2024 Marina Usova

#include "../lib_bitfield/bitfield.h"

#include <algorithm>
#include <stdexcept>

BitField::BitField(size_t size) : _size(size) {
  if (_size > 0) {
    _data = new unsigned int[_size]();  // Inits with zeros
  } else {
    _data = nullptr;
  }
}
BitField::BitField(const BitField& other) :
    _size(other._size) {
  if (_size > 0) {
    _data = new unsigned int[_size];
    std::copy(other._data, other._data + _size, _data);
  } else {
    _data = nullptr;
  }
}
BitField::BitField(BitField&& other) noexcept :
    _size(other._size),
    _data(other._data) {
  other._size = 0;
  other._data = nullptr;
}

BitField::~BitField() { delete[] _data; }

BitField BitField::operator|(const BitField& other) const {
  if (_size != other._size) {
    throw std::logic_error("ERROR: BitFields must have the same size!");
  }
  BitField result(_size);
  for (size_t i = 0; i < _size; ++i) {
    result._data[i] = _data[i] | other._data[i];
  }
  return result;
}
BitField BitField::operator&(const BitField& other) const {
  if (_size != other._size) {
    throw std::logic_error("ERROR: BitFields must have the same size!");
  }
  BitField result(_size);
  for (size_t i = 0; i < _size; ++i) {
    result._data[i] = _data[i] & other._data[i];
  }
  return result;
}
BitField BitField::operator~() const {
  BitField result(_size);
  for (size_t i = 0; i < _size; ++i) {
    result._data[i] = ~_data[i];
  }
  return result;
}
BitField& BitField::operator=(const BitField& other) {
  if (this != &other) {
    unsigned int* new_data = nullptr;
    if (other._size > 0) {
      new_data = new unsigned int[other._size];
      std::copy(other._data, other._data + other._size, new_data);
    }
    delete[] _data;
    _data = new_data;
    _size = other._size;
  }
  return *this;
}
BitField& BitField::operator=(BitField&& other) noexcept {
  if (this != &other) {
    delete[] _data;
    _data = other._data;
    _size = other._size;

    other._data = nullptr;
    other._size = 0;
  }
  return *this;
}
