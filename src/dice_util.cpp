// Utility functions for dice
#include <assert.h>
#include "dice_util.h"

namespace DiceUtil
{
  // Cumulative distribution function: return the probability P(x<t), i.e. number of values below or equal threshold t divided by number of elements
  template <typename T> double cdf(const std::vector<T>& X, double t)
  {
      assert(X.size() > 0);
      T val_cnt = 0;
      for (int n = 0; n < X.size(); n++)
          if (X[n] <= t)
              val_cnt++;
      return (double)val_cnt / (double)X.size();
  }

  // Appends value x n-times to vector X
  template <typename T> void append(std::vector<T>& X, const T& x, size_t n)
  {
      if (X.capacity() < X.size() + n)
          X.reserve(X.size() + n);
      for (size_t i = 0; i < n; i++)
          X.push_back(x);
  }

  // Returns P(A>B)
  template <typename A_type, typename B_type> double probability_to_beat(const std::vector<A_type>& A, const std::vector<B_type>& B)
  {
      size_t beat_cnt = 0;
      for (size_t i = 0; i < A.size(); i++)
          for (size_t j = 0; j < B.size(); j++)
              if (A[i] > B[j])
                  beat_cnt++;
      return (double)beat_cnt / (double)(A.size() * B.size());
  }

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

  // Checks given commandline arguments for an option
  bool has_cli_arg(int argc, char** argv, const std::string& arg_name)
  {
    for (int n = 1; n < argc; n++)
    {
      std::string arg_str(argv[n]);
      if (arg_str.substr(0, arg_name.size()) == arg_name)
        return true;
    }
    return false;
  }

  // Checks given commandline arguments for arg_name. If found, arg_value is set and true returned; otherwise just false is returned. 
  bool has_cli_arg(int argc, char** argv, const std::string& arg_name, int& arg_value)
  {
    for (int n = 1; n < argc; n++)
    {
      std::string arg_str(argv[n]);
      if (arg_str.substr(0, arg_name.size()) == arg_name)
      {
          arg_value = std::stoi(arg_str.substr(arg_name.size()));
          return true;
      }
    }
    return false;
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
  template double cdf<int>(const std::vector<int>& X, double t);
  template double cdf<int64_t>(const std::vector<int64_t>& X, double t);
  template void append(std::vector<int>& X, const int& x, size_t n);
  template void append(std::vector<int64_t>& X, const int64_t& x, size_t n);
  template void append(std::vector<double>& X, const double& x, size_t n);
  template double probability_to_beat<int64_t, int64_t>(const std::vector<int64_t>& A, const std::vector<int64_t>& B);
  template double probability_to_beat<double, double>(const std::vector<double>& A, const std::vector<double>& B);
  template double probability_to_beat<int64_t, double>(const std::vector<int64_t>& A, const std::vector<double>& B);
  template double probability_to_beat<double, int64_t>(const std::vector<double>& A, const std::vector<int64_t>& B);
  template void generate_sums_recursively<int>(const std::vector<std::vector<int>>& dice, size_t die_index, int current_sum, std::vector<int>& results);
  template void generate_sums_recursively<int64_t>(const std::vector<std::vector<int64_t>>& dice, size_t die_index, int64_t current_sum, std::vector<int64_t>& results);
  template std::string print<int>(const std::vector<int>& vec);
  template std::string print<int64_t>(const std::vector<int64_t>& vec);
  template std::string print<double>(const std::vector<double>& vec);
  template std::string print<int>(const std::vector<int>& vec, int width);
  template std::string print<int64_t>(const std::vector<int64_t>& vec, int width);
  template std::string print<double>(const std::vector<double>& vec, int width);
  template std::string print<int>(const std::vector<std::vector<int>>& matrix);
  template std::string print<int64_t>(const std::vector<std::vector<int64_t>>& matrix);
  template std::string print<double>(const std::vector<std::vector<double>>& matrix);

} // namespace DiceUtil
