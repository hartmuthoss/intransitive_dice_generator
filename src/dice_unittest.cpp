// Unittests for dice generation
#include <assert.h>
#include <dice_generator.h>
#include "dice_unittest.h"

namespace DiceUnittest
{
    // Run a unit test for 3-level insertion algorithm
    bool insertion_test(const std::vector<DieValueT>& A, const std::vector<DieValueT>& B, DiceLogger& logger, int max_iterations)
    {
        Die die_i(A), die_j, die_k(B);
        double pDiDk = die_i.probability_to_beat(die_k), pDiDj = 0, pDjDk = 0;
        bool success = false;
        for (int retry_cnt = 0; retry_cnt < max_iterations && !success; retry_cnt++)
        {
            die_j = DiceGenerator::find_die_between_two_others(die_i, die_k);
            if (die_j.num_values() > 0 && (pDiDj = die_i.probability_to_beat(die_j)) > 0.5 && (pDjDk = die_j.probability_to_beat(die_k)) > 0.5)
            {
                // Success: die D_j with D_i > D_j > D_k found
                logger.cout() << "Unittest: D_i = (" << die_i.print() << " )" << std::endl;
                logger.cout() << "Unittest: D_j = (" << die_j.print() << " )" << std::endl;
                logger.cout() << "Unittest: D_k = (" << die_k.print() << " )" << std::endl;
                logger.cout() << std::fixed << std::setprecision(3) << "Unittest passed: P(D_i>D_j) = " << pDiDj << ", P(D_j>D_k) = " << pDjDk << ", P(D_i>D_k) = " << pDiDk << std::endl;
                success = true;
            }
            else
            {
                // Enlarge value range by factor two and normalize to min dice value 1
                DieValueT dice_min_value = std::min(die_i.values().front(), die_k.values().front());
                die_i.mul_add_values(2, 1 - 2 * dice_min_value);
                die_k.mul_add_values(2, 1 - 2 * dice_min_value);
            }
        }
        if (!success)
        {
            logger.cerr() << "## ERROR dice_insertion_unittest with D_i = (" << die_i.print() << " ), D_k = (" << die_k.print() << " ): D_j with D_i > D_j > D_k not found." << std::endl << std::endl;
            logger.cerr() << "## Press ENTER to continue..." << std::endl;
            getchar();
        }
        logger.cout() << std::endl;
        return success;
    }

    // Run basic unit tests
    bool run(DiceLogger& logger)
    {
        bool success = true;
        success = insertion_test({ 2, 19, 22, 23, 29, 30 }, { 9, 16, 16, 19, 31, 38 }, logger) && success;
        success = insertion_test({ 6, 11, 16, 21, 26, 31 }, { 5, 10, 15, 20, 25, 36 }, logger) && success;
        success = insertion_test({ 5, 10, 15, 20, 25, 36 }, { 4,  9, 14, 19, 30, 35 }, logger) && success;
        success = insertion_test({ 4,  9, 14, 19, 30, 35 }, { 3,  8, 13, 24, 29, 34 }, logger) && success;
        success = insertion_test({ 3,  8, 13, 24, 29, 34 }, { 2,  7, 18, 23, 28, 33 }, logger) && success;
        success = insertion_test({ 2,  7, 18, 23, 28, 33 }, { 1, 12, 17, 22, 27, 32 }, logger) && success;
        success = insertion_test({ 1, 12, 17, 22, 27, 32 }, { 6, 11, 16, 21, 26, 31 }, logger) && success;
        return success;
    }

} // namespace DiceUnittest
