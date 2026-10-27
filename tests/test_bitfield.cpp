// Copyright 2024 Marina Usova

#include <gtest/gtest.h>
#include "../lib_bitfield/bitfield.h"

#include <stdexcept>  // For exceptions
#include <iostream>  // For ostream
#include <algorithm>  // For max
#include <utility>  // For move

TEST(BitFieldTest, can_create_with_default_constructor) {
  ASSERT_NO_THROW(BitField bf);
}

TEST(BitFieldTest, can_create_with_constructor_by_size) {
  ASSERT_NO_THROW(BitField bf(3));
}

TEST(BitFieldTest, can_create_with_copy_constructor) {
  BitField bf1(2);
  bf1.set_bit(10);
  BitField bf2(bf1);

  EXPECT_EQ(bf2.get_bit(10), true);

  bf1.reset_bit(10);

  EXPECT_EQ(bf2.get_bit(10), true);
  EXPECT_EQ(bf1.get_bit(10), false);
}

TEST(BitFieldTest, can_create_with_move_constructor) {
  size_t original_size = 10;
  BitField source(original_size);
  source.set_bit(5);
  BitField destination(std::move(source));

  EXPECT_EQ(destination.get_bit(5), true);
  EXPECT_THROW(source.get_bit(0), std::out_of_range);
  EXPECT_NO_THROW(destination.get_bit(40));
}

TEST(BitFieldTest, can_get_max_power) {
  BitField bf1(3);

  EXPECT_EQ(bf1.max_power(), 32 * 3);
}

TEST(BitFieldTest, can_assign) {
  BitField bf1(2);
  bf1.set_bit(38);
  BitField bf2(1);

  ASSERT_NO_THROW(bf2 = bf1);
  EXPECT_EQ(bf2.get_bit(38), true);
}

TEST(BitFieldTest, can_assign_to_itself) {
  BitField bf(2);
  bf.set_bit(5);

  ASSERT_NO_THROW(bf = bf);
  EXPECT_EQ(bf.get_bit(5), true);
}

TEST(BitFieldTest, can_set_and_get_bit) {
  BitField bf(1);
  bf.set_bit(15);

  EXPECT_EQ(bf.get_bit(15), true);
  EXPECT_EQ(bf.get_bit(14), false);
}

TEST(BitFieldTest, can_reset_bit) {
  BitField bf(1);
  bf.set_bit(20);

  EXPECT_EQ(bf.get_bit(20), true);

  bf.reset_bit(20);

  EXPECT_EQ(bf.get_bit(20), false);
}

TEST(BitFieldTest, throws_when_position_out_of_range) {
  BitField bf(1);

  EXPECT_THROW(bf.get_index(497), std::out_of_range);
  EXPECT_THROW(bf.get_bit(45), std::out_of_range);
  EXPECT_THROW(bf.set_bit(32), std::out_of_range);
  EXPECT_THROW(bf.reset_bit(45), std::out_of_range);
}

TEST(BitFieldTest, can_do_bitwise_or) {
  BitField bf1(1);
  BitField bf2(1);
  bf1.set_bit(2);
  bf2.set_bit(5);
  BitField result = bf1 | bf2;

  EXPECT_EQ(result.get_bit(2), true);
  EXPECT_EQ(result.get_bit(5), true);
  EXPECT_EQ(result.get_bit(0), false);
}

TEST(BitFieldTest, can_do_bitwise_and) {
  BitField bf1(1);
  BitField bf2(1);
  bf1.set_bit(3);
  bf1.set_bit(7);
  bf2.set_bit(7);
  BitField result = bf1 & bf2;

  EXPECT_EQ(result.get_bit(3), false);
  EXPECT_EQ(result.get_bit(7), true);
  EXPECT_EQ(result.get_bit(0), false);
}

TEST(BitFieldTest, can_do_bitwise_not) {
  BitField bf(1);
  bf.set_bit(0);
  BitField result = ~bf;

  EXPECT_EQ(result.get_bit(0), false);
  EXPECT_EQ(result.get_bit(1), true);
}

TEST(BitFieldTest, throws_when_bitwise_on_different_sizes) {
  BitField bf1(1);
  BitField bf2(2);

  EXPECT_THROW(bf1 | bf2, std::logic_error);
  EXPECT_THROW(bf1 & bf2, std::logic_error);
}
