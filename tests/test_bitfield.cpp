// Copyright 2026 smvax

#include <gtest/gtest.h>

#include <stdexcept>  // For exceptions
#include <iostream>  // For ostream
#include <algorithm>  // For max
#include <utility>  // For move
#include <string>  // For string
#include <vector>  // For vector

#include "../lib_bitfield/bitfield.h"

TEST(BitFieldTest, can_create_with_default_constructor) {
  ASSERT_NO_THROW(BitField bf);
}

TEST(BitFieldTest, can_create_with_constructor_by_length) {
  ASSERT_NO_THROW(BitField bf(334));
}

TEST(BitFieldTest, can_create_with_copy_constructor) {
  BitField bf1(20);
  bf1.set_bit(10);
  BitField bf2(bf1);

  EXPECT_EQ(bf2.get_bit(10), true);

  bf1.reset_bit(10);

  EXPECT_EQ(bf2.get_bit(10), true);
  EXPECT_EQ(bf1.get_bit(10), false);
}

TEST(BitFieldTest, can_create_with_move_constructor) {
  size_t original_length = 10;
  BitField source(original_length);
  source.set_bit(5);
  BitField destination(std::move(source));

  EXPECT_EQ(destination.get_bit(5), true);
  EXPECT_THROW(source.get_bit(0), std::out_of_range);
  EXPECT_NO_THROW(destination.get_bit(9));
}

TEST(BitFieldTest, can_get_zero_length) {
  BitField bf1(0);

  EXPECT_EQ(bf1.get_length(), 0);
}

TEST(BitFieldTest, can_get_positive_length) {
  BitField bf1(3);

  EXPECT_EQ(bf1.get_length(), 3);
}

TEST(BitFieldTest, can_get_small_blocks_count) {
  BitField bf1(3);

  EXPECT_EQ(bf1.get_blocks_count(), 1);
}

TEST(BitFieldTest, can_get_border_blocks_count) {
  BitField bf1(32);
  BitField bf2(64);

  EXPECT_EQ(bf1.get_blocks_count(), 1);
  EXPECT_EQ(bf2.get_blocks_count(), 2);
}

TEST(BitFieldTest, can_get_big_blocks_count) {
  BitField bf1(300);

  EXPECT_EQ(bf1.get_blocks_count(), 10);
}

TEST(BitFieldTest, can_assign) {
  BitField bf1(40);
  bf1.set_bit(38);
  BitField bf2(1);

  ASSERT_NO_THROW(bf2 = bf1);
  EXPECT_EQ(bf2.get_bit(38), true);
}

TEST(BitFieldTest, can_assign_to_itself) {
  BitField bf(20);
  bf.set_bit(5);

  ASSERT_NO_THROW(bf = bf);
  EXPECT_EQ(bf.get_bit(5), true);
}

TEST(BitFieldTest, can_set_and_get_bit) {
  BitField bf(100);
  bf.set_bit(15);

  EXPECT_EQ(bf.get_bit(15), true);
  EXPECT_EQ(bf.get_bit(14), false);
}

TEST(BitFieldTest, can_reset_bit) {
  BitField bf(100);
  bf.set_bit(20);

  EXPECT_EQ(bf.get_bit(20), true);

  bf.reset_bit(20);

  EXPECT_EQ(bf.get_bit(20), false);
}

TEST(BitFieldTest, throws_when_position_out_of_range) {
  BitField bf(31);

  EXPECT_THROW(bf.get_index(32), std::out_of_range);
  EXPECT_THROW(bf.get_bit(45), std::out_of_range);
  EXPECT_THROW(bf.set_bit(32), std::out_of_range);
  EXPECT_THROW(bf.reset_bit(45), std::out_of_range);
}

TEST(BitFieldTest, can_do_bitwise_or) {
  BitField bf1(10);
  BitField bf2(10);
  bf1.set_bit(2);
  bf2.set_bit(5);
  BitField result = bf1 | bf2;

  EXPECT_EQ(result.get_bit(2), true);
  EXPECT_EQ(result.get_bit(5), true);
  EXPECT_EQ(result.get_bit(0), false);
}

TEST(BitFieldTest, can_do_bitwise_and) {
  BitField bf1(10);
  BitField bf2(10);
  bf1.set_bit(3);
  bf1.set_bit(7);
  bf2.set_bit(7);
  BitField result = bf1 & bf2;

  EXPECT_EQ(result.get_bit(3), false);
  EXPECT_EQ(result.get_bit(7), true);
  EXPECT_EQ(result.get_bit(0), false);
}

TEST(BitFieldTest, can_do_bitwise_not) {
  BitField bf(10);
  bf.set_bit(0);
  BitField result = ~bf;

  EXPECT_EQ(result.get_bit(0), false);
  EXPECT_EQ(result.get_bit(1), true);
}

TEST(BitFieldTest,
    throws_when_or_n_and_bitwise_on_different_sizes) {
  BitField bf1(10);
  BitField bf2(200);
  bf1.set_bit(9);
  bf2.set_bit(45);

  EXPECT_THROW(bf1 | bf2, std::logic_error);
  EXPECT_THROW(bf1 & bf2, std::logic_error);
}

TEST(BitFieldTest, can_cout) {
  BitField bf(63);
  bf.set_bit(0);
  bf.set_bit(2);
  bf.set_bit(62);
  std::stringstream ss;
  ss << bf;
  std::string expected(63, '0');
  expected[0] = '1';
  expected[2] = '1';
  expected[62] = '1';

  EXPECT_EQ(ss.str(), expected);
}

TEST(BitFieldTest, can_cin) {
  std::string input_str = "01110101";
  std::stringstream ss(input_str);
  BitField bf;
  ss >> bf;

  EXPECT_EQ(bf.get_length(), 8);
  EXPECT_EQ(bf.get_bit(0), false);
  EXPECT_EQ(bf.get_bit(1), true);
  EXPECT_EQ(bf.get_bit(2), true);
  EXPECT_EQ(bf.get_bit(3), true);
  EXPECT_EQ(bf.get_bit(4), false);
  EXPECT_EQ(bf.get_bit(5), true);
  EXPECT_EQ(bf.get_bit(6), false);
  EXPECT_EQ(bf.get_bit(7), true);
}
