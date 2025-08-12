// Utility functions for dice
#include "dice_util.h"

namespace DiceUtil
{
  // Returns true, if a given number is a prime
  bool is_prime(size_t number)
  {
    if (number < 2)
      return false;
    for (int i = 2; i * i <= number; ++i)
      if (number % i == 0)
        return false;
    return true;
  }

  // Returns true, if a given string is a palindrome
  bool is_palindrome(const std::string& str)
  {
    return std::equal(str.begin(), str.begin() + str.size() / 2, str.rbegin());
  }

  // Generates all combinations of dice sums recursively
  template <typename T> void generate_sums_recursively(const std::vector<std::vector<T>>& dice, size_t die_index, T current_sum, std::vector<T>& results)
  {
    if (die_index == dice.size()) 
    {
      results.push_back(current_sum);
      return;
    }
    for (int value : dice[die_index]) 
    {
      generate_sums_recursively(dice, die_index + 1, current_sum + value, results);
    }
  }

  // Formats a vector of integers into a string
  template <typename T> std::string print(const std::vector<T>& vec)
  {
    std::stringstream str;
    for (size_t i = 0; i < vec.size(); i++)
      str << " " << vec[i];
    return str.str();
  }

  // Formats a vector of integers into a string
  template <typename T> std::string print(const std::vector<T>& vec, int width)
  {
    std::stringstream str;
    for (size_t i = 0; i < vec.size(); i++)
      str << " " << std::setw(width) << vec[i];
    return str.str();
  }

  // Formats a matrix (i.e. a vector of vectors of integers) into a string
  template <typename T> std::string print(const std::vector<std::vector<T>>& matrix)
  {
    std::stringstream str;
    for (size_t i = 0; i < matrix.size(); i++)
    {
      str << " D" << std::setfill('0') << std::setw(2) << i << ": ";
      for (size_t j = 0; j < matrix[i].size(); j++)
        str << " " << std::setfill(' ') << std::setw(3) << matrix[i][j];
      str << std::endl;
    }
    return str.str();
  }

  // Explicit template instantiation
  template void generate_sums_recursively<int>(const std::vector<std::vector<int>>& dice, size_t die_index, int current_sum, std::vector<int>& results);
  template void generate_sums_recursively<int64_t>(const std::vector<std::vector<int64_t>>& dice, size_t die_index, int64_t current_sum, std::vector<int64_t>& results);
  template std::string print<int>(const std::vector<int>& vec);
  template std::string print<int64_t>(const std::vector<int64_t>& vec);
  template std::string print<int>(const std::vector<int>& vec, int width);
  template std::string print<int64_t>(const std::vector<int64_t>& vec, int width);
  template std::string print<int>(const std::vector<std::vector<int>>& matrix);
  template std::string print<int64_t>(const std::vector<std::vector<int64_t>>& matrix);

} // namespace DiceUtil
