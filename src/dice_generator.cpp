// Generator for intransitive dice and tuples of dice
#include "dice_generator.h"

namespace DiceGenerator
{
    // Returns the intransitive efron dice
    DiceSet efron(void)
    {
        return DiceSet("Efrons dice", { Die({4, 4, 4, 4, 0, 0}), Die({3, 3, 3, 3, 3, 3}), Die({6, 6, 2, 2, 2, 2}), Die({5, 5, 5, 1, 1, 1}) });
    }

    // Returns the intransitive miwin dice
    DiceSet miwin(void)
    {
        return DiceSet("Miwins dice", { Die({1, 2, 5, 6, 7, 9}), Die({1, 3, 4, 5, 8, 9}), Die({2, 3, 4, 6, 7, 8}) });
    }

    // Returns the intransitive oskar dice
    DiceSet oskar(void)
    {
        return DiceSet("Oskars dice", { Die({2, 2, 14, 14, 17, 17}), Die({7, 7, 10, 10, 16, 16}), Die({5, 5, 13, 13, 15, 15}), Die({3, 3, 9, 9, 21, 21}), Die({1, 1, 12, 12, 20, 20}), Die({6, 6, 8, 8, 19, 19}), Die({4, 4, 11, 11, 18, 18}) });
    }

    // Returns the intransitive grime dice
    DiceSet grime(void)
    {
        return DiceSet("Grimes dice", { Die({2, 2, 2, 7, 7, 7}), Die({1, 1, 6, 6, 6, 6}), Die({0, 5, 5, 5, 5, 5}), Die({4, 4, 4, 4, 4, 9}), Die({3, 3, 3, 3, 8, 8}) });
    }

    // Returns an intransitive example dice
    DiceSet example_1(void)
    {
        return DiceSet("Example 1 dice", { Die({2, 2, 4, 4, 9, 9}), Die({1, 1, 6, 6, 8, 8}), Die({3, 3, 5, 5, 7, 7}) });
    }

    // Returns an intransitive example dice (identical to Munnoz-Perera dice for N=6)
    DiceSet example_2(void)
    {
        return DiceSet("Example 2 dice", { Die({1, 12, 17, 22, 27, 32}), Die({2, 7, 18, 23, 28, 33}), Die({3, 8, 13, 24, 29, 34}), Die({4, 9, 14, 19, 30, 35}), Die({5, 10, 15, 20, 25, 36}), Die({6, 11, 16, 21, 26, 31}) });
    }

    // Doubled oskar dice with two identical dice rolled at the same time
    DoubleDiceSet double_oskar(void)
    {
        return DoubleDiceSet("Oskars double dice", {
          DoubleDie({{ 2, 2, 14, 14, 17, 17 }, { 2, 2, 14, 14, 17, 17 }}),
          DoubleDie({{ 7, 7, 10, 10, 16, 16 }, { 7, 7, 10, 10, 16, 16 }}),
          DoubleDie({{ 5, 5, 13, 13, 15, 15 }, { 5, 5, 13, 13, 15, 15 }}),
          DoubleDie({{ 3, 3,  9,  9, 21, 21 }, { 3, 3,  9,  9, 21, 21 }}),
          DoubleDie({{ 1, 1, 12, 12, 20, 20 }, { 1, 1, 12, 12, 20, 20 }}),
          DoubleDie({{ 6, 6,  8,  8, 19, 19 }, { 6, 6,  8,  8, 19, 19 }}),
          DoubleDie({{ 4, 4, 11, 11, 18, 18 }, { 4, 4, 11, 11, 18, 18 }}) });
    }

    // Create a matrix of intransitive dice values using  Muñoz-Perera's formula for N>=3 (https://en.wikipedia.org/wiki/Intransitive_dice): 
    // "To obtain a set of N fair intransitive dice of N faces it is enough to set the values v_n,j = (j-1)*N+(n-j)%(N)+1 for n,j=1,...,N.
    // Using this expression, it can be verified that P(D_m < D_n) = 0.5 + 1/(2*N) - ((n-m)%N)/(N*N). So each die beats floor(N/2-1) dice in the set."
    // Note: The path { D_N,..., D_2, D_1, D_N } is always intransitive for intransitive Munnoz-Perera dice, since P(D_N > D_(N-1)) > 0.5,..., P(D_2 > D_1) > 0.5, P(D_1 > D_N) > 0.5
    // for these dice (see https://pereradrian.github.io/doc/adrian_munnoz_perera_generalized_intransitive_dice_2024.pdf)
    // Note: Formula v_n,j = (j - 1) * N + (n - j) % (N) + 1 is replaced by v_n,j = (j - 1) * N + (N + n - j) % (N) + 1
    // to handle (n - j) % (N) correctly for all values of (n - j) < 0. 
    // MSVC calculates correctly ((-1 + 6) % 6) == 5, but incorrectly ((-1) % 6) == -1.
    std::vector<std::vector<DieValueT>> munnoz_perera_matrix(int N)
    {
        assert(N >= 3);
        std::vector<std::vector<DieValueT>> matrix(N);
        for (int n = 1; n <= N; n++)
        {
            matrix[n - 1] = std::vector<DieValueT>(N, 0);
            for (int j = 1; j <= N; j++)
            {
                matrix[n - 1][j - 1] = (j - 1) * N + (N + n - j) % (N)+1;
            }
        }
        return matrix;
    }

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
    DiceSet munnoz_perera(int N)
    {
        std::vector<std::vector<DieValueT>> matrix = munnoz_perera_matrix(N);
        std::vector<Die> dice(N);
        for (int n = 0; n < N; n++)
            dice[n] = Die(matrix[n]);
        return DiceSet("Munnoz Perera " + std::to_string(N) + "-sided dice", dice);
    }

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
    DicePath munnoz_perera_path(int N)
    {
        std::vector<int> path_indices;
        path_indices.reserve(N + 1);
        for (int n = N - 1; n >= 0; n--)
            path_indices.push_back(n);
        path_indices.push_back(N - 1);
        return DicePath(path_indices);
    }

    // Split N intransitive N-sided Munnoz-Perera dice with N = 6*K into N tuples of K M-sided dice (and let the sum of K values in a tuple win)
    // Default: M = 6, i.e. N tuples of K=N/M=N/6 six-sided dice are generated.
    MultiDiceSet create_multi_dice_set_munnoz_perera(int N, int M)
    {
        assert((N % M) == 0);
        int K = N / M;
        std::vector<std::vector<DieValueT>> munnoz_perera_matrix = DiceGenerator::munnoz_perera_matrix(N);
        std::vector<MultiDie> multi_dice;
        multi_dice.reserve(munnoz_perera_matrix.size());
        for (int row_idx = 0; row_idx < munnoz_perera_matrix.size(); row_idx++)
        {
            size_t row_size = munnoz_perera_matrix[row_idx].size();
            assert(munnoz_perera_matrix[0].size() == row_size && row_size % K == 0);
            std::vector<std::vector<DieValueT>> dice_values;
            dice_values.reserve(K);
            for (int start_offset = 0, end_offset = M; end_offset <= row_size; start_offset += M, end_offset += M)
            {
                std::vector<DieValueT> die_values(munnoz_perera_matrix[row_idx].begin() + start_offset, munnoz_perera_matrix[row_idx].begin() + end_offset);
                dice_values.push_back(die_values);
            }
            multi_dice.push_back(MultiDie(dice_values));
        }
        std::stringstream dice_name;
        dice_name << N << " tupel of " << K << " " << M << "-sided dice from " << N << "x" << N << " Munnoz-Perera dice";
        return MultiDiceSet(dice_name.str(), multi_dice);
    }

    // Split N intransitive N-sided Munnoz-Perera dice into N dice tuples with randomly choosen number of sides (and let the sum of values in a tuple win).
    // Each tuple is splitted into multiple dice, and both the number of dice and their numbers of sides are choosen randomly. Example: 
    // Die D_n is one of the 14 14-sided Munnoz-Perera dice. D_n is splitted into  dice { D_n1, D_n2, D_n3 }, and D_n1 has 3 sides, D_n2 has 6 sides, D_n3 has 5 sides.
    MultiDiceSet create_multi_dice_set_munnoz_perera_random_split(int N, std::mt19937& random_generator)
    {
        std::vector<std::vector<DieValueT>> munnoz_perera_matrix = DiceGenerator::munnoz_perera_matrix(N);
        std::vector<MultiDie> multi_dice;
        multi_dice.reserve(munnoz_perera_matrix.size());
        const int min_num_sides = 3; // each die must have at least 3 sides
        for (int row_idx = 0; row_idx < munnoz_perera_matrix.size(); row_idx++)
        {
            int row_size = (int)munnoz_perera_matrix[row_idx].size();
            assert(munnoz_perera_matrix[0].size() == row_size);
            std::vector<std::vector<DieValueT>> dice_values;
            dice_values.reserve(row_size);
            for (int start_offset = 0; start_offset < row_size; )
            {
                std::uniform_int_distribution<int> distribution(start_offset + min_num_sides, row_size);
                int end_offset = distribution(random_generator);
                if (end_offset + min_num_sides > row_size)
                    end_offset = row_size;
                std::vector<DieValueT> die_values(munnoz_perera_matrix[row_idx].begin() + start_offset, munnoz_perera_matrix[row_idx].begin() + end_offset);
                dice_values.push_back(die_values);
                start_offset = end_offset;
            }
            multi_dice.push_back(MultiDie(dice_values));
        }
        std::stringstream dice_name;
        dice_name << N << " tupel of randomly splitted dice from " << N << "x" << N << " Munnoz-Perera dice";
        return MultiDiceSet(dice_name.str(), multi_dice);
    }

    // Create a random list of DoubleDiceSets from a 12x12-matrix of Munnoz-Perera dice values
    std::vector<DoubleDiceSet> create_random_double_dice_sets_from_12x12(int num_sets)
    {
        std::mt19937 random_generator(0);
        std::vector<std::vector<DieValueT>> munnoz_perera_mat_12 = DiceGenerator::munnoz_perera_matrix(12);
        std::vector<std::array<std::array<DieValueT, 12>, 12>> double_dice_values_list;
        double_dice_values_list.reserve(num_sets);
        while (double_dice_values_list.size() < num_sets)
        {
            // std::cout << std::endl << "munnoz_perera_mat_12 = " << std::endl << DiceUtil::print(munnoz_perera_mat_12);
            std::array<std::array<DieValueT, 12>, 12> double_dice_values;
            for (int row_idx = 0; row_idx < 12; row_idx++)
            {
                assert(munnoz_perera_mat_12[row_idx].size() == 12);
                std::array<DieValueT, 12> twelve_die_values;
                bool double_die_already_used = false;
                do
                {
                    std::copy(munnoz_perera_mat_12[row_idx].begin(), munnoz_perera_mat_12[row_idx].end(), twelve_die_values.begin());
                    std::sort(twelve_die_values.begin(), twelve_die_values.begin() + 6, std::less<DieValueT>());
                    std::sort(twelve_die_values.begin() + 6, twelve_die_values.end(), std::less<DieValueT>());
                    double_die_already_used = false;
                    for (int n = 0; n < double_dice_values_list.size() && !double_die_already_used; n++)
                    {
                        std::array<DieValueT, 12>& double_die_used = double_dice_values_list[n][row_idx];
                        if (std::equal(double_die_used.begin(), double_die_used.end(), twelve_die_values.begin()))
                            double_die_already_used = true;
                    }
                    std::shuffle(munnoz_perera_mat_12[row_idx].begin(), munnoz_perera_mat_12[row_idx].end(), random_generator);
                } while (double_die_already_used);
                double_dice_values[row_idx] = twelve_die_values;
            }
            // std::cout << "double_dice_values = " << std::endl << DiceUtil::print(double_dice_values);
            double_dice_values_list.push_back(double_dice_values);
        }
        std::vector<DoubleDiceSet> double_dice_sets;
        double_dice_sets.reserve(double_dice_values_list.size());
        for (int n = 0; n < double_dice_values_list.size(); n++)
        {
            std::vector<DoubleDie> double_dice;
            double_dice.reserve(double_dice_values_list[n].size());
            for (int m = 0; m < double_dice_values_list[n].size(); m++)
            {
                std::vector<DieValueT> values_die_A(double_dice_values_list[n][m].begin(), double_dice_values_list[n][m].begin() + 6);
                std::vector<DieValueT> values_die_B(double_dice_values_list[n][m].begin() + 6, double_dice_values_list[n][m].end());
                double_dice.push_back(DoubleDie({ values_die_A, values_die_B }));
            }
            double_dice_sets.push_back(DoubleDiceSet("Random dice " + std::to_string(n + 1) + " from intransitive 12x12 Munnoz-Perera matrix", double_dice));
        }
        return double_dice_sets;
    }

    // Tries to find a die die_j "between" two dice die_i and die_k with P(die_i>die_j) > 0.5 && P(die_j>die_k) > 0.5.
    // Let die_i and die_k be two N-sided dice with P(die_i>die_k) > 0.5, where die_i[n] is the n-th value of die_i and
    // die_k[n] is the n-th value of die_k, with 0 <= n < N for N-sided dice. This function uses a simple algorithm:
    // If die_i[n] > die_k[n], die_j[n] can be smaller than die_i[n] to ensure that die_i beats die_j. We choose die_j[n] = (die_i[n] + die_k[n]) / 2.
    // If die_i[n] < die_k[n], die_j[n] can be greater than die_k[n] to ensure that die_j beats die_k. We choose die_j[n] = die_k[n] + 1.
    Die find_die_between_two_others(Die& die_i, Die& die_k)
    {
        const std::vector<DieValueT>& die_i_vec = die_i.values();
        const std::vector<DieValueT>& die_k_vec = die_k.values();
        assert(die_i_vec.size() == die_k_vec.size());
        double pDiDk = die_i.probability_to_beat(die_k);
        if (pDiDk > 0.5) // P(die_i>die_k) > 0.5: find die_j with P(die_i>die_j) > 0.5 and P(die_j>die_k) > 0.5
        {
            std::vector<DieValueT> die_j_vec(die_i_vec.size());
            for (int n = 0; n < die_i_vec.size(); n++)
            {
                if (die_i_vec[n] > die_k_vec[n])
                    die_j_vec[n] = (die_i_vec[n] + die_k_vec[n]) / 2;
                else if (die_i_vec[n] < die_k_vec[n])
                    die_j_vec[n] = die_k_vec[n] + 1;
                else
                    die_j_vec[n] = die_i_vec[n];
            }
            Die die_j(die_j_vec);
            double pDiDj = die_i.probability_to_beat(die_j);
            double pDjDk = die_j.probability_to_beat(die_k);
            if (pDiDj > 0.5 && pDjDk > 0.5)
                return die_j; // success: P(die_i>die_k) > 0.5, P(die_i>die_j) > 0.5, P(die_j>die_k) > 0.5
        }
        else if (pDiDk < 0.5) // P(die_i>die_k) < 0.5: find die_j with P(die_i>die_j) < 0.5 and P(die_j>die_k) < 0.5
        {
            std::vector<DieValueT> die_j_vec(die_k_vec.size());
            for (int n = 0; n < die_k_vec.size(); n++)
            {
                if (die_k_vec[n] > die_i_vec[n])
                    die_j_vec[n] = (die_i_vec[n] + die_k_vec[n]) / 2;
                else if (die_k_vec[n] < die_i_vec[n])
                    die_j_vec[n] = die_i_vec[n] + 1;
                else
                    die_j_vec[n] = die_k_vec[n];
            }
            Die die_j(die_j_vec);
            double pDiDj = die_i.probability_to_beat(die_j);
            double pDjDk = die_j.probability_to_beat(die_k);
            if (pDiDj < 0.5 && pDjDk < 0.5)
                return die_j; // success: P(die_i>die_k) < 0.5, P(die_i>die_j) < 0.5, P(die_j>die_k) < 0.5
        }
        else // fallback: die_j[n] = (die_i[n] + die_k[n]) / 2, if P(die_i>die_k) == 0.5
        {
            std::vector<DieValueT> die_j_vec(die_k_vec.size());
            for (int n = 0; n < die_k_vec.size(); n++)
                die_j_vec[n] = (die_i_vec[n] + die_k_vec[n]) / 2;
            Die die_j(die_j_vec);
            double pDiDj = die_i.probability_to_beat(die_j);
            double pDjDk = die_j.probability_to_beat(die_k);
            if (std::abs(pDiDj - 0.5) < FLT_EPSILON && std::abs(pDjDk - 0.5) < FLT_EPSILON)
                return die_j; // success: P(die_i>die_k) == 0.5, P(die_i>die_j) == 0.5, P(die_j>die_k) == 0.5
        }
        return Die(); // no die found
    }


    // Iteratively insert new dice D_j between D_i and D_(i+1), such that P(D_i > D_j) > 0.5 and P(D_j > D_(i+1)) > 0.5.
    // Start with a given set of dice and a given intransitive path, and repeat the process until
    // the given max. number of dice are reached, or no new dice D_j can be found.
    // If initial_dice_path is intransitive, extended_dice_path will also be intransitive
    bool extend_set_by_intransitive_dice_insertion(const DiceSet& initial_dice_set, const DicePath& initial_dice_path, size_t max_num_dice, DiceSet& extended_dice_set, DicePath& extended_dice_path, DiceLogger* logger)
    {
        std::string extended_dice_name = extended_dice_set.name().empty() ? initial_dice_set.name() : extended_dice_set.name();
        extended_dice_set = DiceSet(extended_dice_name, initial_dice_set.dice());
        extended_dice_path = initial_dice_path;
        if (initial_dice_set.size() >= max_num_dice)
            return true;
        DieValueT min_die_value = extended_dice_set.min_die_value();
        if (min_die_value < 1) // norm min. die value to >= 1
            extended_dice_set.mul_add_values(1, min_die_value + 1);
        while (extended_dice_set.size() < max_num_dice)
        {
            DiceSet work_dice_set;
            DicePath work_dice_path;
            if (extend_set_by_intransitive_dice_insertion_once(extended_dice_set, extended_dice_path, max_num_dice, work_dice_set, work_dice_path))
            {
                // New dice found => iterate with extended dice set
                extended_dice_set = work_dice_set;
                extended_dice_path = work_dice_path;
                if (logger)
                    logger->cout() << extended_dice_set.print_path_probabilities_x(extended_dice_path, true) << std::endl;
            }
            else
            {
                // No new dice found => multiply all values of all dice by factor 2 and retry
                bool success = false;
                DiceSet input_dice_set = extended_dice_set;
                DicePath input_dice_path = extended_dice_path;
                for (int retry_cnt = 0; !success && extended_dice_set.size() < max_num_dice && retry_cnt < 2; retry_cnt++)
                {
                    input_dice_set.mul_add_values(2, -1);
                    if (extend_set_by_intransitive_dice_insertion_once(input_dice_set, input_dice_path, max_num_dice, work_dice_set, work_dice_path))
                    {
                        // New dice found => iterate with extended dice set
                        extended_dice_set = work_dice_set;
                        extended_dice_path = work_dice_path;
                        success = true;
                        if (logger)
                            logger->cout() << extended_dice_set.print_path_probabilities_x(extended_dice_path, true) << std::endl;
                    }
                }
                if (!success)
                    break;
            }
        }
        return extended_dice_set.size() > initial_dice_set.size() && extended_dice_path.size() > initial_dice_path.size();
    }

    // Insert new dice D_j between D_i and D_(i+1), such that P(D_i > D_j) > 0.5 and P(D_j > D_(i+1)) > 0.5, 
    // once with all dice in the dice set, until the given max. number of dice are reached, or no new dice D_j can be found.
    // If initial_dice_path is intransitive, extended_dice_path will also be intransitive
    bool extend_set_by_intransitive_dice_insertion_once(DiceSet& initial_dice_set, DicePath& initial_dice_path, size_t max_num_dice, DiceSet& extended_dice_set, DicePath& extended_dice_path)
    {
        assert(initial_dice_set.size() > 1 && initial_dice_path.size() > 1);
        if (initial_dice_set.size() >= max_num_dice)
        {
            extended_dice_set = initial_dice_set;
            extended_dice_path = initial_dice_path;
            return false;
        }
        std::vector<Die> extended_dice_vec;
        extended_dice_vec.reserve(2 * initial_dice_path.size());
        for (size_t dice_cnt = 1; dice_cnt < initial_dice_path.size(); dice_cnt++)
        {
            int dice_from = initial_dice_path.at(dice_cnt - 1);
            int dice_to = initial_dice_path.at(dice_cnt);
            Die die_i = initial_dice_set.at(dice_from);
            Die die_k = initial_dice_set.at(dice_to);
            Die die_j = DiceGenerator::find_die_between_two_others(die_i, die_k);
            extended_dice_vec.push_back(die_i);
            if (die_j.num_values() == die_i.num_values()) // success: P(D_i > D_j) > 0.5 and P(D_j > D_(i+1)) > 0.5 => Insert new die_j
                extended_dice_vec.push_back(die_j);
            if (extended_dice_vec.size() + ((int)initial_dice_path.size() - (int)dice_cnt) >= max_num_dice) // max number of dice reached => just copy the remaining dice and finish
            {
                for (size_t n = dice_cnt + 1; n < initial_dice_path.size(); n++)
                    extended_dice_vec.push_back(initial_dice_set.at(initial_dice_path.at(n - 1)));
                break;
            }
        }
        if (extended_dice_vec.size() > initial_dice_set.size()) // success: new dice found
        {
            extended_dice_set = DiceSet(initial_dice_set.name(), extended_dice_vec);
            std::vector<int> extended_dice_idx;
            extended_dice_idx.reserve(extended_dice_set.size());
            for (size_t dice_cnt = 0; dice_cnt < extended_dice_set.size(); dice_cnt++)
                extended_dice_idx.push_back(dice_cnt);
            if (initial_dice_path.front() == initial_dice_path.back())
                extended_dice_idx.push_back(extended_dice_idx.front());
            extended_dice_path = DicePath(extended_dice_idx);
            return true;
        }
        else // no success, no dice found
        {
            extended_dice_set = initial_dice_set;
            extended_dice_path = initial_dice_path;
            return false;
        }
    }

} // namespace DiceGenerator