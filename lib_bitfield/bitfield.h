// Copyright 2024 Marina Usova

#ifndef LIB_BITFIELD_BITFIELD_H_
#define LIB_BITFIELD_BITFIELD_H_

#include <stdexcept>

class BitField {
  unsigned int* _data;
  size_t _size;  // Number of bitblocks
  static constexpr size_t BITS_PER_BLOCK = sizeof(unsigned int) * 8;
  // ^ Evaluated at compile-time (32)

 public:
  explicit BitField(size_t size = 0);
  BitField(const BitField&);
  ~BitField();

  inline size_t get_index(size_t) const;
  // ^ Bit position to its index of bitblock
  inline bool get_bit(size_t) const;

  inline void set_bit(size_t);    // Sets 1
  inline void reset_bit(size_t);  // Sets 0

  BitField operator|(const BitField&);
  BitField operator&(const BitField&);
  BitField operator~();
  BitField& operator=(const BitField&);

 private:
  inline unsigned int get_mask(size_t) const noexcept;
  // ^ Mask of the bit in its bitblock
};

inline size_t BitField::get_index(size_t pos) const {
  if (pos >= _size * BITS_PER_BLOCK) {
    throw std::out_of_range("ERROR: Position out of range!");
  }
  return pos / BITS_PER_BLOCK;  // May be >> 5
}
inline bool BitField::get_bit(size_t pos) const {
  return _data[get_index(pos)] & get_mask(pos);
}
inline unsigned int BitField::get_mask(size_t pos) const noexcept {
  return 1u << (pos & (BITS_PER_BLOCK - 1));
  // Works exactly like 1u << (pos % BITS_PER_BLOCK)
  // Being used only with get_index, so doesn't need to make a throw itself
}

inline void BitField::set_bit(size_t pos) {
  _data[get_index(pos)] |= get_mask(pos);
}
inline void BitField::reset_bit(size_t pos) {
  _data[get_index(pos)] &= ~get_mask(pos);
}

#endif  // LIB_BITFIELD_BITFIELD_H_
