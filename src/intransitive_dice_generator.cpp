// Search for and generation of intransitiv dice.
// See https://en.wikipedia.org/wiki/Intransitive_dice and README.md for details.
#include "dice.h"
#include "dice_generator.h"

// Prints the intransitive paths of a set of dice
template <typename DiceSetType> void print_intransitive_paths(DiceSetType& dice_set, const DicePathList& intransitive_paths, int max_paths = INT_MAX, int max_paths_to_print = 10)
{
  if (dice_set.is_intransitive())
    std::cout << dice_set.name() << " is intransitive and has " << intransitive_paths.size() << " intransitive paths"
      << (intransitive_paths.size() >= max_paths ? " (or more, path limit reached)" : "") << ": " << intransitive_paths.print(max_paths_to_print) << std::endl;
  else
    std::cout << dice_set.name() << " is NOT intransitive and has " << intransitive_paths.size() << " intransitive paths" << std::endl;
  int path_cnt = 0;
  for (std::list<DicePath>::const_iterator path_iter = intransitive_paths.cbegin(); path_iter != intransitive_paths.cend() && path_cnt < max_paths_to_print; path_iter++, path_cnt++)
    std::cout << dice_set.name() << ", " << (path_cnt + 1) << ". intransitive path probabilities: " << dice_set.print_intransitive_path_probabilities(*path_iter) << path_iter->print_bonus(", ") << std::endl;
  if (path_cnt < intransitive_paths.size())
    std::cout << "..." << std::endl;
  std::cout << "Probability matrix of " << dice_set.name() << ":" << std::endl;
  dice_set.print_probability_matrix();
  std::cout << std::endl;
}

// Searches and prints the intransitive paths of a set of dice
template <typename DiceSetType> void search_print_intransitive_paths(DiceSetType& dice_set, int max_paths = INT_MAX, int max_paths_to_print = 10)
{
  std::cout << dice_set.name() << ":" << std::endl << dice_set.print_dice();
  const DicePathList& intransitive_paths = dice_set.search_intransitive_paths(max_paths);
  print_intransitive_paths(dice_set, intransitive_paths, max_paths, max_paths_to_print);
}

// Examples and tests of intransitive dice and dice tuples
int main(int argc, char** argv)
{
  std::cout << "Intransitive dice test started." << std::endl << std::endl;

  // TODO: "The hunt for the 8. Oskar": Start with Oskar's dice and find an eight intransitive dice!
  
  // Some well known examples of intransitive dice
  std::vector<DiceSet> dice_sets = {
    DiceGenerator::efron(),
    DiceGenerator::miwin(),
    DiceGenerator::oskar(),
    DiceGenerator::grime(),
    DiceGenerator::example_1(),
    DiceGenerator::example_2(),
    DiceGenerator::munnoz_perera(6),
    DiceGenerator::munnoz_perera(12)
  };

  // Search all intransitive paths
  int max_paths = INT_MAX, max_paths_to_print = 10;
  for (size_t set_idx = 0; set_idx < dice_sets.size(); set_idx++)
    search_print_intransitive_paths(dice_sets[set_idx], max_paths, max_paths_to_print);

  // Test a pair of dice (double dice) with two identical oskar dice rolled at the same time
  DoubleDiceSet double_dice_set = DiceGenerator::double_oskar();
  search_print_intransitive_paths(double_dice_set, max_paths, max_paths_to_print);

  // Split 12 intransitive 12-sided Munnoz-Perera dice into 12 pairs of 6-sided dice (and let the sum of of 2 values in a pair win)
  MultiDiceSet multi_dice_set = DiceGenerator::create_multi_dice_set_munnoz_perera(12);
  search_print_intransitive_paths(multi_dice_set, max_paths, max_paths_to_print);

  // The path { D_N,..., D_2, D_1, D_N } is always intransitive for the Munnoz-Perera dice, since P(D_N > D_(N-1)) > 0.5,..., P(D_2 > D_1) > 0.5, P(D_1 > D_N) > 0.5
  // for these dice (see https://pereradrian.github.io/doc/adrian_munnoz_perera_generalized_intransitive_dice_2024.pdf). This relation seems to preserve,
  // if a N N-sided Munnoz-Perera dice with N = 6*K are partitioned into N tuples of 6-sided dice, where each tuple has K 6-sided dice and the sum of the 6-sided dice in the tuple counts.
  // I haven't found a mathematical proof that this relation remains under partitioning. But we can test it by simulation: We just partition N N-sided Munnoz-Perera dice with N=6*K
  // into N dice tuples (where each tuple has K 6-sided dice).
  for (int N = 12, M = 6; N <= 30; N+=M) // for (int N = 12, M = 6; N <= 30; N+=M) // N = [12, 18, 24, 30] = number of dice tupel, M = 6 = number of sides of a die. Tested up to N = 42. Note that the calculation for N > 30 becomes very time consuming...
  {
    DiceSet dice_set = DiceGenerator::munnoz_perera(N);
    DicePath dice_path = DiceGenerator::munnoz_perera_path(N);
    MultiDiceSet multi_dice_set = DiceGenerator::create_multi_dice_set_munnoz_perera(N, M);
    bool dice_path_is_intransitive = false, multi_dice_path_is_intransitive = false;
    std::cout << dice_set.name() << ":" << std::endl << dice_set.print_dice();
    std::cout << dice_set.print_path_probabilities(dice_path, dice_path_is_intransitive, true) << std::endl << std::endl;
    std::cout << multi_dice_set.name() << ":" << std::endl << multi_dice_set.print_dice();
    std::cout << multi_dice_set.print_path_probabilities(dice_path, multi_dice_path_is_intransitive, true) << std::endl << std::endl;
  }

  // Now we partition the N N-sided Munnoz-Perera dice into N tuples of M-sided dice.
  // The path { D_N,..., D_2, D_1, D_N } seems to remain still intransitive under dice partioning.
  for (int N = 6; N <= 26; N++) // for (int N = 6; N <= 26; N++) // Tested up to N = 38. Note that the calculation for N > 26 becomes very time consuming...
  {
    for (int M = 3; M <= N / 2; M++)
    {
      if ((N % M) == 0) // i.e. a N-sided die can be partitioned into K=N/M M-sided dice
      {
        DicePath dice_path = DiceGenerator::munnoz_perera_path(N);
        MultiDiceSet multi_dice_set = DiceGenerator::create_multi_dice_set_munnoz_perera(N, M);
        bool dice_path_is_intransitive = false;
        std::cout << multi_dice_set.name() << ":" << std::endl << multi_dice_set.print_dice();
        std::cout << multi_dice_set.print_path_probabilities(dice_path, dice_path_is_intransitive, true) << std::endl << std::endl;
      }
    }
  }

  // We may try to partition the N N-sided Munnoz-Perera dice into N dice tuples with a random number of sides in each tuple.
  // Example: Die D_n is one of the 14 14-sided Munnoz-Perera dice. D_n is partitioned into  dice { D_n1, D_n2, D_n3 }, and D_n1 has 3 sides, D_n2 has 6 sides, D_n3 has 5 sides.
  // Both the number of dice D_ni and their numbers of sides are choosen randomly (but all dice have at least 3 sides).
  // A random partition into 3 or more sides breaks the intransitivity already for N = 6: A random partition of 6x6 Munnoz-Perera dice has no intransitive paths.
  // std::mt19937 random_generator(0);
  // DiceSet dice_set = DiceGenerator::munnoz_perera(6);
  // DicePath dice_path = DiceGenerator::munnoz_perera_path(6);
  // MultiDiceSet random_multi_dice_set = DiceGenerator::create_multi_dice_set_munnoz_perera_random_split(6, random_generator);
  // bool dice_path_is_intransitive = false;
  // std::cout << dice_set.name() << ":" << std::endl << dice_set.print_dice();
  // std::cout << dice_set.print_path_probabilities(dice_path, dice_path_is_intransitive, true) << std::endl << std::endl;
  // std::cout << random_multi_dice_set.name() << ":" << std::endl << random_multi_dice_set.print_dice();
  // std::cout << random_multi_dice_set.print_path_probabilities(dice_path, dice_path_is_intransitive, false) << std::endl << std::endl;
  // search_print_intransitive_paths(random_multi_dice_set, max_paths, max_paths_to_print);

  // Split 18 intransitive 18-sided Munnoz-Perera dice into 18 triples of 6-sided dice (and let the sum of 3 values in a triple win)
  // Note: the graph search becomes extremely large, the graph is extremly time and memory consuming and may exceed system ressources!
  // MultiDiceSet triple_dice_set = DiceGenerator::create_multi_dice_set_munnoz_perera(18);
  // search_print_intransitive_paths(triple_dice_set, 1, max_paths_to_print);

  // Randomly select sets of 12 double dice from a matrix of 12 intransitive 12-sided dice
  // std::vector<DoubleDiceSet> double_dice_sets = DiceGenerator::create_random_double_dice_sets_from_12x12(5);
  // for (int n = 0; n < double_dice_sets.size(); n++)
  //   search_print_intransitive_paths(double_dice_sets[n], max_paths, max_paths_to_print);

  std::cout << "Intransitive dice test finished." << std::endl;
  return 0;
}
