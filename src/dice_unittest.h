// Unittests for dice generation
#pragma once
#include <vector>
#include "dice.h"
#include "dice_logger.h"

namespace DiceUnittest
{
    // Run basic unit tests
    bool run(DiceLogger& logger);

    // Run a unit test for 3-level insertion algorithm
    bool insertion_test(const std::vector<DieValueT>& A, const std::vector<DieValueT>& B, DiceLogger& logger, int max_iterations = 2);

} // namespace DiceUnittest
