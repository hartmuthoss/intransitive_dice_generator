// Intransitive dice and tuple of dice
#pragma once
#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <climits>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include "dice_matrix.h"
#include "dice_path.h"
#include "dice_util.h"

// class Die implements a single die and the base class for all dice
class Die
{
public:

  // Default constructor
  Die() { }

  // Initializing constructor given the values on all sides
  Die(const std::vector<int>& values) : m_values(values) {}

  // Counts the number of eyes beating another die, i.e. returns the number of (*this > other) and (other > *this) sides.
  // Probability P(*this > other) = count_beats(other)[0] / (double)(NumSides * NumSides).
  // Probability P(other > *this) = count_beats(other)[1] / (double)(NumSides * NumSides).
  virtual std::array<size_t, 2> count_beats(const Die& other) const;

  // Computes and returns the probability of beating another die, i.e. returns P(*this > other).
  virtual double probability_to_beat(Die& other);

  // Sorts the values of this die in descending order
  virtual void sort_values();

  // Print the values of this die
  virtual std::string print(void) const { return DiceUtil::print(values(), 3); }

  // Returns the eye values of this die
  virtual inline const std::vector<int>& values(void) const { return m_values; }

  // Returns the number of values (sides) of this die
  virtual inline size_t num_values(void) const { return m_values.size(); }

protected:

  // Returns the eye values of this die
  virtual inline std::vector<int>& values(void) { return m_values; }

  std::vector<int> m_values; // the values (eyes) of each side
  bool values_sorted = false; // if true, m_values are sorted in ascending order, otherwise not
};

// class MultiDie implements a tuple of multiple dice simulating one die, where the value of rolling the multi die is the sum of each single die
class MultiDie : public Die
{
public:

  // Default constructor
  MultiDie() : Die() {}

  // Initializing constructor given the values of two single dice by two vectors
  MultiDie(const std::vector<std::vector<int>>& die_values);

  // Print the values of all single dice of this dice tuple
  virtual std::string print(void) const;

protected:

  // Initializes the values of this multi die given the values (eyes) of each single die
  virtual void init(const std::vector<std::vector<int>> die_values);

  // the values (eyes) of each single die
  std::vector<std::vector<int>> m_die_values; 
};

// A set of (possibly intransitive) dice
template <typename DieType> class DiceSetT
{
public:

  // Initializing constructor given a set of dice
  DiceSetT(const std::string& name = "", const std::vector<DieType>&dice = {}) : m_name(name), m_dice(dice) {}

  // Searches and returns a list of intransitive paths, each path contains a list of intransitive dice.
  // Example: const DicePathList& paths = search_intransitive_paths();
  // paths[0] = (D0, D1, D2, D3, D0) means: P(D0>D1) > 0.5, P(D1>D2) > 0.5, P(D2>D3) > 0.5, P(D3>D0) > 0.5.
  // paths[1] = (D0, D2, D3, D1, D0) means: P(D0>D2) > 0.5, P(D2>D3) > 0.5, P(D3>D1) > 0.5, P(D1>D0) > 0.5.
  const DicePathList& search_intransitive_paths(size_t max_final_paths = INT_MAX, size_t max_open_paths = INT_MAX);

  // Returns true, if this set of dice is intransitive, i.e. there is at least one intransitive path.
  bool is_intransitive();

  // Prints the matrix of beat probabilities P(D_i>D_j) for all i, j.
  void print_probability_matrix();

  // Prints the beat probabilities P(D_i>D_j) for all i, j in a given path.
  std::string print_path_probabilities(const DicePath& path, bool& path_is_intransitive, bool warn_if_path_is_not_intransitive);

  // Prints the list of probabilities P(D_i>D_j) for all D_i, D_j in a given path and
  // checks their intransitivity, i.e. checks that all P(D_i>D_j) > 0.5 resp. all P(D_i>D_j) < 0.5.
  std::string print_intransitive_path_probabilities(const DicePath& path);

  // Print the dice of this set
  std::string print_dice(void) const;

  // Returns the name of this set
  const std::string& name(void) const { return m_name; }

protected:

  // Searches and returns a list of closed cycles (i.e. an intransitive chain of dice), which contain all dice exactly once.
  DicePathList search_closed_cycle(const SqrMatrix<int>& matrix, size_t max_final_paths, size_t max_open_paths);

  // Recursive search of a closed cycle, which contains all dice exactly once.
  void search_closed_cycle_recursive(const std::vector<std::vector<int>>& col_indices_per_row, DicePathList& open_path_list, DicePathList& closed_path_list, int row_idx, size_t max_final_paths, size_t max_open_paths);

  // Fast pre-check for intransitive chains: All columns of a beat/lose-matrix must have at least one "1", otherwise the dice are not intransitive.
  bool precheck_intransitive_is_possible(const SqrMatrix<int>& matrix);

  std::string m_name; // descriptional name like "Efrons dice"
  std::vector<DieType> m_dice; // the dice in this set
  DicePathList m_intransitive_paths; // list of all intransitive paths (which is empty if the dice are not intransitive)
  bool m_intransitive_paths_computed = false; // becomes true after m_intransitive_paths has been computed

}; // end of class DiceSetT

typedef MultiDie DoubleDie;
typedef DiceSetT<Die> DiceSet;
typedef DiceSetT<DoubleDie> DoubleDiceSet;
typedef DiceSetT<MultiDie> MultiDiceSet;
