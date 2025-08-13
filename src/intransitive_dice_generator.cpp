// Generator for intransitiv dice.
// See https://en.wikipedia.org/wiki/Intransitive_dice and README.md for details.
#include "dice.h"
#include "dice_generator.h"
#include "dice_logger.h"

// Prints description and commandline options
std::string print_help(void)
{
    std::stringstream str;
    str << "intransitive_dice_generator generates intransitive dice. See https://github.com/hartmuthoss/intransitive_dice_generator for details." << std::endl;
    str << "By default (i.e.no command line options given), intransitive_dice_generator creates and prints examples of intransitive dice with different cycle lengths." << std::endl;
    str << "To create a chain of N intransitive M-sided dice with N>=M, call intransitive_dice_generator with arguments -N=<int> -M=<int>." << std::endl;
    str << "Example: \"intransitive_dice_generator -N=12 -M=6\" creates 12 intransitive 6 - sided dice." << std::endl;
    str << "All messages are logged in file intransitive_dice_generator.log." << std::endl;
    return str.str();
}

// Print dice die_i, die_j, die_k and their probabilities P(Di>Dk), P(Di>Dj), P(Dj>Dk).
void print_dice_ijk_probabilities(const Die& die_i, const Die& die_j, const Die& die_k, DiceLogger& logger)
{
    Die Di = die_i, Dj = die_j, Dk = die_k;
    double pDiDk = Di.probability_to_beat(Dk);
    double pDiDj = Di.probability_to_beat(Dj);
    double pDjDk = Dj.probability_to_beat(Dk);
    logger.cout() << "Di = (" << die_i.print() << ")" << std::endl << "Dj = (" << die_j.print() << ")" << std::endl << "Dk = (" << die_k.print() << ")" << std::endl;
    logger.cout() << "P(Di>Dk) = " << std::fixed << std::setprecision(3) << pDiDk << ", P(Di>Dj) = " << pDiDj << ", P(Dj>Dk) = " << pDjDk << std::endl << std::endl;
}

// Prints the intransitive paths of a set of dice
template <typename DiceSetType> void print_intransitive_paths(DiceSetType& dice_set, const DicePathList& intransitive_paths, DiceLogger& logger, int max_paths = INT_MAX, int max_paths_to_print = 10)
{
    if (dice_set.has_intransitive_paths())
        logger.cout() << dice_set.name() << " is intransitive and has " << intransitive_paths.size() << " intransitive paths"
        << (intransitive_paths.size() >= max_paths ? " (or more, path limit reached)" : "") << ": " << intransitive_paths.print(max_paths_to_print) << std::endl;
    else
        logger.cout() << dice_set.name() << " is NOT intransitive and has " << intransitive_paths.size() << " intransitive paths" << std::endl;
    int path_cnt = 0;
    for (std::list<DicePath>::const_iterator path_iter = intransitive_paths.cbegin(); path_iter != intransitive_paths.cend() && path_cnt < max_paths_to_print; path_iter++, path_cnt++)
        logger.cout() << dice_set.name() << ", " << (path_cnt + 1) << ". intransitive path probabilities: " << dice_set.print_intransitive_path_probabilities(*path_iter) << path_iter->print_bonus(", ") << std::endl;
    if (path_cnt < intransitive_paths.size())
        logger.cout() << "..." << std::endl;
    logger.cout() << "Probability matrix of " << dice_set.name() << ":" << std::endl;
    logger.cout() << dice_set.print_probability_matrix() << std::endl;
}

// Searches and prints the intransitive paths of a set of dice
template <typename DiceSetType> void search_print_intransitive_paths(DiceSetType& dice_set, DiceLogger& logger, int max_paths = INT_MAX, int max_paths_to_print = 10)
{
    logger.cout() << dice_set.name() << ":" << std::endl << dice_set.print_dice();
    const DicePathList& intransitive_paths = dice_set.search_intransitive_paths(max_paths);
    print_intransitive_paths(dice_set, intransitive_paths, logger, max_paths, max_paths_to_print);
}

// Examples and tests of intransitive dice and dice tuples
int main(int argc, char** argv)
{
  // Commandline options
  DiceLogger logger("intransitive_dice_generator.log");
  int N = 0, M = 0;
  if (DiceUtil::has_cli_arg(argc, argv, "-N=", N) && DiceUtil::has_cli_arg(argc, argv, "-M=", M) && N >= 3 && M >= 3)
  {
    // Create N intransitive M-sided dice for given N and M by extending N N-sided Munnoz-Perera dice
    DiceSet  mp_dice = DiceGenerator::munnoz_perera(M);
    DicePath mp_dice_path = DiceGenerator::munnoz_perera_path(M);
    DiceSet mp_extended("Extended Munnoz Perera " + std::to_string(M) + "-sided dice", {});
    DicePath mp_extended_path;
    bool success = DiceGenerator::extend_set_by_intransitive_dice_insertion(mp_dice, mp_dice_path, N, mp_extended, mp_extended_path);
    if (success)
      logger.cout() << mp_extended.print_path_probabilities_x(mp_extended_path, true) << std::endl;
    else
      logger.cerr() << "## WARNING: DiceGenerator failed with " << N << " " << M << "-sided Munnoz Perera dice" << std::endl;
    exit(success ? EXIT_SUCCESS : EXIT_FAILURE);
  }
  else if (argc > 1)
  {
    logger.cout() << std::endl << print_help() << std::endl;
    exit(EXIT_SUCCESS);
  }

  // Create some well known examples of intransitive dice
  logger.cout() << "Intransitive dice generator test started." << std::endl << std::endl;
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
    search_print_intransitive_paths(dice_sets[set_idx], logger, max_paths, max_paths_to_print);

  // Test a pair of dice (double dice) with two identical oskar dice rolled at the same time
  DoubleDiceSet double_dice_set = DiceGenerator::double_oskar();
  search_print_intransitive_paths(double_dice_set, logger, max_paths, max_paths_to_print);

  // Split 12 intransitive 12-sided Munnoz-Perera dice into 12 pairs of 6-sided dice (and let the sum of of 2 values in a pair win)
  MultiDiceSet multi_dice_set = DiceGenerator::create_multi_dice_set_munnoz_perera(12);
  search_print_intransitive_paths(multi_dice_set, logger, max_paths, max_paths_to_print);

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
    logger.cout() << dice_set.print_path_probabilities_x(dice_path, true) << std::endl;
    logger.cout() << multi_dice_set.print_path_probabilities_x(dice_path, true) << std::endl;
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
        logger.cout() << multi_dice_set.print_path_probabilities_x(dice_path, true) << std::endl;
      }
    }
  }

  // We may try to partition the N N-sided Munnoz-Perera dice into N dice tuples with a random number of sides in each tuple.
  // Example: Die D_n is one of the 14 14-sided Munnoz-Perera dice. D_n is partitioned into  dice { D_n1, D_n2, D_n3 }, and D_n1 has 3 sides, D_n2 has 6 sides, D_n3 has 5 sides.
  // Both the number of dice D_ni and their numbers of sides are choosen randomly (but all dice have at least 3 sides).
  // A random partition into 3 or more sides breaks the intransitivity immediately, e.g. for N = 6: A random partition of 6x6 Munnoz-Perera dice has no intransitive paths.
  // std::mt19937 random_generator(0);
  // DiceSet dice_set = DiceGenerator::munnoz_perera(6);
  // DicePath dice_path = DiceGenerator::munnoz_perera_path(6);
  // MultiDiceSet random_multi_dice_set = DiceGenerator::create_multi_dice_set_munnoz_perera_random_split(6, random_generator);
  // logger.cout() << dice_set.print_path_probabilities_x(dice_path, true) << std::endl;
  // logger.cout() << random_multi_dice_set.print_path_probabilities_x(dice_path, true) << std::endl;
  // search_print_intransitive_paths(random_multi_dice_set, logger, max_paths, max_paths_to_print);

  // Start with Oskar dice and create longer cycles of intransitive dice
  DiceSet oskar_dice = DiceGenerator::oskar();
  DicePath oskar_dice_path({ 0, 1, 2, 3, 4, 5, 6, 0 });
  logger.cout() << oskar_dice.print_path_probabilities_x(oskar_dice_path, true) << std::endl;

  // Iteratively insert new dice D_j between D_i and D_(i+1), such that P(D_i > D_j) > 0.5 and P(D_j > D_(i+1)) > 0.5.
  // Start with a given set of dice and a given intransitive path, and repeat the process until
  // the given max. number of dice are reached, or no new dice D_j can be found.
  DiceSet oskar_extended("Extended Oskar dice", {});
  DicePath oskar_extended_path;
  size_t max_num_dice = 1000; // Tested with up to 10 million dice - feel free to create intransitive cycles with as many dice as desired.
  if (DiceGenerator::extend_set_by_intransitive_dice_insertion(oskar_dice, oskar_dice_path, max_num_dice, oskar_extended, oskar_extended_path))
      logger.cout() << oskar_extended.print_path_probabilities_x(oskar_extended_path, true) << std::endl;
  else
      logger.cerr() << "## WARNING: DiceGenerator::extend_set_by_intransitive_dice_insertion() failed with oskar dice" << std::endl << std::endl;

  // Create long cycles of intransitive dice by extending Grime dice
  DiceSet grime_dice = DiceGenerator::grime();
  DicePath grime_dice_path({ 0, 1, 2, 3, 4, 0 });
  logger.cout() << grime_dice.print_path_probabilities_x(grime_dice_path, true) << std::endl;
  DiceSet grime_extended("Extended Grime dice", {});
  DicePath grime_extended_path;
  if (DiceGenerator::extend_set_by_intransitive_dice_insertion(grime_dice, grime_dice_path, max_num_dice, grime_extended, grime_extended_path))
      logger.cout() << grime_extended.print_path_probabilities_x(grime_extended_path, true) << std::endl;
  else
      logger.cerr() << "## WARNING: DiceGenerator::extend_set_by_intransitive_dice_insertion() failed with grime dice" << std::endl << std::endl;


  // Create long cycles of intransitive dice by extending N N-sided Munnoz-Perera dice (N >= 3)
  for (size_t num_dice_sides = 3; num_dice_sides <= 48; num_dice_sides *= 2)
  {
      DiceSet  mp_dice = DiceGenerator::munnoz_perera(num_dice_sides);
      DicePath mp_dice_path = DiceGenerator::munnoz_perera_path(num_dice_sides);
      logger.cout() << mp_dice.print_path_probabilities_x(mp_dice_path, true) << std::endl;
      DiceSet mp_extended("Extended Munnoz Perera " + std::to_string(num_dice_sides) + "-sided dice", {});
      DicePath mp_extended_path;
      if (DiceGenerator::extend_set_by_intransitive_dice_insertion(mp_dice, mp_dice_path, max_num_dice, mp_extended, mp_extended_path))
          logger.cout() << mp_extended.print_path_probabilities_x(mp_extended_path, true) << std::endl;
      else
          logger.cerr() << "## WARNING: DiceGenerator::extend_set_by_intransitive_dice_insertion() failed with " << num_dice_sides << " " << num_dice_sides << "-sided Munnoz Perera dice" << std::endl << std::endl;
  }

  logger.cout() << "Intransitive dice generator test finished." << std::endl;
  return 0;
}
