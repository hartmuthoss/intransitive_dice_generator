// Supporting classes for closed paths and cycles of (possibly intransitive) dice
#pragma once
#include <list>
#include <sstream>
#include <string>
#include <vector>
#include "dice_util.h"

// DicePath stores a single path, i.e. a list of dice indices
class DicePath
{
public:

  DicePath() {}

  DicePath(const std::vector<int>& start_indices)
  {
    m_dice_indices = start_indices;
  }

  const int& front()
  {
    return m_dice_indices.front();
  }

  const int& back()
  {
    return m_dice_indices.back();
  }

  void append(int index)
  {
    m_dice_indices.push_back(index);
  }

  bool append_if_not_in_list(int index)
  {
    if (!is_in_list(index))
    {
      append(index);
      return true; // index appended
    }
    return false; // index already inserted
  }

  // Returns true, if this path contains a given dice, or false otherwise
  bool is_in_list(int index)
  {
    for (size_t n = 0; n < m_dice_indices.size(); n++)
    {
      if (m_dice_indices[n] == index)
        return true;
    }
    return false;
  }

  // Returns true, if two paths contain identical dice, or false otherwise
  bool is_identical(const DicePath& path) const
  {
    if (m_dice_indices.size() == path.m_dice_indices.size())
      return std::equal(m_dice_indices.begin(), m_dice_indices.end(), path.m_dice_indices.begin());
    return false;
  }

  // Returns true, if this paths contains all dice (i.e. from 0 up to num_dices-1), or false otherwise
  bool contains_all_dices(size_t num_dices)
  {
    std::vector<int> side_range(num_dices, 0);
    for (size_t n = 0; n < m_dice_indices.size(); n++)
      side_range[m_dice_indices[n]] = 1;
    for (size_t n = 0; n < num_dices; n++)
      if (side_range[n] == 0)
        return false;
    return true;
  }

  // Returns the number of dice in this path
  size_t size(void) const
  {
    return m_dice_indices.size();
  }

  // Returns the n-th die in this path
  int at(size_t n) const
  {
    assert(n >= 0 && n < m_dice_indices.size());
    return m_dice_indices[n];
  }

  // Formats the dice in this path into a string
  std::string print(void) const
  {
    std::stringstream str;
    for (std::vector<int>::const_iterator dice_iter = m_dice_indices.cbegin(); dice_iter != m_dice_indices.cend(); dice_iter++)
      str << (dice_iter != m_dice_indices.cbegin() ? " " : "") << *dice_iter;
    return str.str();
  }

  // Formats the dice in this path into an decimal number
  size_t to_decimal(void) const
  {
    std::stringstream str;
    for (size_t n = 0; n < m_dice_indices.size(); n++)
      str << (m_dice_indices[n] + 1);
    return atoll(str.str().c_str());
  }

  // Returns a bonus message, if the path converted to decimal number is a prime or a palindrome
  std::string print_bonus(const std::string& prefix) const
  {
    size_t path_decimal = to_decimal();
    std::string path_str = print();
    bool decimal_prime = DiceUtil::is_prime(path_decimal); // path bonus, if the path converted to decimal number is a prime
    bool path_is_palindrome = DiceUtil::is_palindrome(std::to_string(path_decimal)); // path bonus, if the path converted to decimal number is a palindrome
    std::stringstream bonus;
    if (path_is_palindrome && decimal_prime)
      bonus << prefix << "bonus: path " << path_str << " has a decimal code " << path_decimal << ", which is a prime and a palindrome";
    else if (decimal_prime)
      bonus << prefix << "bonus: path " << path_str << " has a decimal code " << path_decimal << ", which is a prime";
    else if (path_is_palindrome)
      bonus << prefix << "bonus: path " << path_str << " has a decimal code " << path_decimal << ", which is a palindrome";
    return bonus.str();
  }

protected:
  std::vector<int> m_dice_indices; // list of dice representing the path
};

// DicePathList stores a list of dice paths
class DicePathList
{
public:

  DicePathList() { }

  DicePath& front(void) { return m_paths.front(); }

  const DicePath& front(void) const { return m_paths.front(); }

  DicePath& back(void) { return m_paths.back(); }

  const DicePath& back(void) const { return m_paths.back(); }

  void append(const DicePath& path) { m_paths.push_back(path); }

  void append(DicePathList& path) { m_paths.insert(m_paths.end(), path.m_paths.begin(), path.m_paths.end()); }

  void splice(DicePathList& path) { m_paths.splice(m_paths.end(), path.m_paths); }

  size_t size(void) const { return m_paths.size(); }

  std::list<DicePath>::iterator begin() { return m_paths.begin(); }

  std::list<DicePath>::const_iterator cbegin() const { return m_paths.cbegin(); }

  std::list<DicePath>::iterator end() { return m_paths.end(); }

  std::list<DicePath>::const_iterator cend() const { return m_paths.cend(); }

  std::list<DicePath>::iterator erase(std::list<DicePath>::iterator it) { return m_paths.erase(it); }

  // Sorts the list of paths by descending path length, such that longer paths (with a higher chance to complete) are prefered in recursive search
  void sort_by_path_length(void) { m_paths.sort([](const DicePath& a, const DicePath& b) { return a.size() > b.size(); }); }

  // Remove all paths ending with die D_<die_idx>
  void remove_paths_by_back_idx(int die_idx)
  {
    for (std::list<DicePath>::iterator path_iter = m_paths.begin(); path_iter != m_paths.end(); )
    {
      if (path_iter->back() == die_idx)
        path_iter = m_paths.erase(path_iter);
      else
        path_iter++;
    }
  }

  // Remove all duplicated paths
  void remove_duplicate_paths(void)
  {
    for (std::list<DicePath>::iterator path_iter1 = m_paths.begin(); path_iter1 != m_paths.end(); path_iter1++)
    {
      std::list<DicePath>::iterator path_iter2 = path_iter1;
      for (path_iter2++; path_iter2 != m_paths.end(); )
      {
        if (path_iter2->is_identical(*path_iter1))
        {
          std::cout << "remove_duplicate_paths(): path " << path_iter2->print() << " found twice, removing duplicate" << std::endl;
          path_iter2 = m_paths.erase(path_iter2);
        }
        else
          path_iter2++;
      }
    }
  }

  // Formats the list of all paths into a string
  std::string print(int max_paths_to_print = INT_MAX) const
  {
    int path_cnt = 0;
    std::stringstream str;
    str << "[ ";
    for (std::list<DicePath>::const_iterator path_iter = m_paths.cbegin(); path_iter != m_paths.cend() && path_cnt < max_paths_to_print; path_iter++, path_cnt++)
    {
      if (path_iter != m_paths.cbegin())
        str << ", ";
      str << "(" << path_iter->print() << ")";
    }
    if (path_cnt < m_paths.size())
      str << ", ... ";
    str << " ]";
    return str.str();
  }

protected:
  std::list<DicePath> m_paths; // list of paths
};
