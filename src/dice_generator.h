// Generator for intransitive dice and pairs of dice
#pragma once
#include "dice.h"

// DiceGenerator implements some generators for intransitive dice and tuples of dice
namespace DiceGenerator
{
	// Returns the intransitive efron dice
	DiceSet efron(void);

	// Returns the intransitive miwin dice
	DiceSet miwin(void);

	// Returns the intransitive oskar dice
	DiceSet oskar(void);

	// Returns the intransitive grime dice
	DiceSet grime(void);

	// Returns an intransitive example dice
	DiceSet example_1(void);

	// Returns an intransitive example dice (identical to Munnoz-Perera dice for N=6)
	DiceSet example_2(void);

	// Doubled oskar dice with two identical dice rolled at the same time
	DoubleDiceSet double_oskar(void);

	// Create a matrix of intransitive dice values using  Muñoz-Perera's formula for N>=3 (https://en.wikipedia.org/wiki/Intransitive_dice): 
	// "To obtain a set of N fair intransitive dice of N faces it is enough to set the values v_n,j = (j-1)*N+(n-j)%(N)+1 for n,j=1,...,N.
	// Using this expression, it can be verified that P(D_m < D_n) = 0.5 + 1/(2*N) - ((n-m)%N)/(N*N). So each die beats floor(N/2-1) dice in the set."
	// Note: The path { D_N,..., D_2, D_1, D_N } is always intransitive for intransitive Munnoz-Perera dice, since P(D_N > D_(N-1)) > 0.5,..., P(D_2 > D_1) > 0.5, P(D_1 > D_N) > 0.5
	// for these dice (see https://pereradrian.github.io/doc/adrian_munnoz_perera_generalized_intransitive_dice_2024.pdf)
	// Note: Formula v_n,j = (j - 1) * N + (n - j) % (N) + 1 is replaced by v_n,j = (j - 1) * N + (N + n - j) % (N) + 1
	// to handle (n - j) % (N) correctly for all values of (n - j) < 0. 
	// MSVC calculates correctly ((-1 + 6) % 6) == 5, but incorrectly ((-1) % 6) == -1.
	std::vector<std::vector<int>> munnoz_perera_matrix(int N);

	// Create a set of intransitive dice using  Muñoz-Perera's formula for N>=3 (see function munnoz_perera_matrix).
	// Examples given in https://pereradrian.github.io/doc/adrian_munnoz_perera_generalized_intransitive_dice_2024.pdf, e.g. for N = 6:
	// D0: [1 12 17 22 27 32]
	// D1: [2  7 18 23 28 33]
	// D2: [3  8 13 24 29 34]
	// D3: [4  9 14 19 30 35]
	// D4: [5 10 15 20 25 36]
	// D5: [6 11 16 21 26 31]
	// with D5 > D4, D4 > D3, D3 > D2, D2 > D1, D1 > D0, D0 > D5.
	// Returns a set of N intransitive N-sided dice.
	DiceSet munnoz_perera(int N);

	// Create an intransitive path for dice created by munnoz_perera(N).
	// Examples for N = 6:
	// D0: [1 12 17 22 27 32]
	// D1: [2  7 18 23 28 33]
	// D2: [3  8 13 24 29 34]
	// D3: [4  9 14 19 30 35]
	// D4: [5 10 15 20 25 36]
	// D5: [6 11 16 21 26 31]
	// with D5 > D4, D4 > D3, D3 > D2, D2 > D1, D1 > D0, D0 > D5.
	// munnoz_perera_path(6) returns the intransitive path { 5, 4, 3, 2, 1, 0, 5 }
	DicePath munnoz_perera_path(int N);

	// Split N intransitive N-sided Munnoz-Perera dice with N = 6*K into N tuples of K M-sided dice (and let the sum of K values in a tuple win)
	// Default: M = 6, i.e. N tuples of K=N/M=N/6 six-sided dice are generated.
	MultiDiceSet create_multi_dice_set_munnoz_perera(int N, int M = 6);

	// Split N intransitive N-sided Munnoz-Perera dice into N dice tuples with randomly choosen number of sides (and let the sum of values in a tuple win).
	// Each tuple is splitted into multiple dice, and both the number of dice and their numbers of sides are choosen randomly. Example: 
	// Die D_n is one of the 14 14-sided Munnoz-Perera dice. D_n is splitted into  dice { D_n1, D_n2, D_n3 }, and D_n1 has 3 sides, D_n2 has 6 sides, D_n3 has 5 sides.
	MultiDiceSet create_multi_dice_set_munnoz_perera_random_split(int N, std::mt19937& random_generator);

	// Create a random list of DoubleDiceSets from a 12x12-matrix of Munnoz-Perera dice values
	std::vector<DoubleDiceSet> create_random_double_dice_sets_from_12x12(int num_sets);

} // namespace DiceGenerator
