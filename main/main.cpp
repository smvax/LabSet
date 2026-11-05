// Copyright 2026 smvax

#include <iostream>
#include <cmath>
#include <limits>

#include "../lib_set/set.h"

void print_sieve_of_eratosthenes(size_t max_num) {
  if (max_num < 2) {
    std::cout << "There are no prime numbers in \
        this range (number must be >= 2)."
              << std::endl;
    return;
  }
  Set primes(max_num);
  for (size_t i = 2; i <= max_num; ++i) {
    primes.insert(static_cast<int>(i));
  }
  size_t limit = static_cast<size_t>(std::sqrt(max_num));
  for (size_t p = 2; p <= limit; ++p) {
    if (primes.get_element(p)) {
      for (size_t step = p * p; step <= max_num; step += p) {
        if (primes.get_element(step)) {
          primes.erase(static_cast<int>(step));
        }
      }
    }
  }
  std::cout << "[Result] Prime numbers in the range [2; " << max_num
            << "]" << std::endl;
  std::cout << primes << std::endl;
}

int main() {
  int input_range = 0;
  std::cout << "=== Sieve of Eratosthenes ===" << std::endl;
  while (true) {
    std::cout << "Enter the upper bound of the range (>=2): ";
    std::cin >> input_range;
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n');
      std::cout << "ERROR: Please enter a valid integer!"
          << std::endl;
      continue;
    }
    if (input_range < 2) {
      std::cout << "The number must be at least 2!" << std::endl;
      continue;
    }
    break;
  }
  std::cout << std::endl;
  std::cout << "Running calculations for the range from 2 to " << input_range
            << "..." << std::endl;
  print_sieve_of_eratosthenes(static_cast<size_t>(input_range));
  return 0;
}
