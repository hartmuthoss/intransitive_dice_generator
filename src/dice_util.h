// Utility functions for dice
#pragma once
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace DiceUtil
{
    // Returns true, if a given number is a prime
    bool is_prime(size_t number);

    // Returns true, if a given string is a palindrome
    bool is_palindrome(const std::string& str);

    // Generates all combinations of dice sums recursively
    void generate_sums_recursively(const std::vector<std::vector<int>>& dice, size_t die_index, int current_sum, std::vector<int>& results);

    // Formats a vector of integers into a string
    std::string print(const std::vector<int>& vec);

    // Formats a vector of integers into a string
    std::string print(const std::vector<int>& vec, int width);

    // Formats a matrix (i.e. a vector of vectors of integers) into a string
    std::string print(const std::vector<std::vector<int>>& matrix);

    // Formats a matrix into a string
    template <int N> std::string print(const std::array<std::array<int, N>, N>& matrix)
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
