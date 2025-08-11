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
  void generate_sums_recursively(const std::vector<std::vector<int>>& dice, size_t die_index, int current_sum, std::vector<int>& results)
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
  std::string print(const std::vector<int>& vec)
  {
    std::stringstream str;
    for (size_t i = 0; i < vec.size(); i++)
      str << " " << vec[i];
    return str.str();
  }

  // Formats a vector of integers into a string
  std::string print(const std::vector<int>& vec, int width)
  {
    std::stringstream str;
    for (size_t i = 0; i < vec.size(); i++)
      str << " " << std::setw(width) << vec[i];
    return str.str();
  }

  // Formats a matrix (i.e. a vector of vectors of integers) into a string
  std::string print(const std::vector<std::vector<int>>& matrix)
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

} // namespace DiceUtil
