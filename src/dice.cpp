// Intransitive dice and tuple of dice
#include "dice.h"

// Optional low-level debugging for path search
#define DICE_DEBUG_PATH_SEARCH 0 // 1
#if DICE_DEBUG_PATH_SEARCH
#define PRINT_DBG_PATH_SEARCH(x) do{ std::cout << x; } while(0)
#else
#define PRINT_DBG_PATH_SEARCH(x)
#endif

// Counts the number of eyes beating another die, i.e. returns the number of (*this > other) and (other > *this) sides.
// Probability P(*this > other) = count_beats(other)[0] / (double)(NumSides * NumSides).
// Probability P(other > *this) = count_beats(other)[1] / (double)(NumSides * NumSides).
std::array<size_t, 2> Die::count_beats(const Die& other) const
{
    const std::vector<int>& this_values = values();
    const std::vector<int>& other_values = other.values();
    assert(this_values.size() == other_values.size());
    std::array<size_t, 2> beat_cnt = { 0, 0 };
    for (size_t n = 0; n < this_values.size(); n++)
    {
        for (size_t m = 0; m < other_values.size(); m++)
        {
            if (this_values[n] > other_values[m])
                beat_cnt[0]++; // this die side beats the others die side
            else if (other_values[m] > this_values[n])
                beat_cnt[1]++; // the others die side beats this die side
        }
    }
    return beat_cnt;
}

// Computes and returns the probability of beating another die, i.e. returns P(*this > other).
double Die::probability_to_beat(Die& other)
{
    assert(values().size() == other.values().size());
    sort_values();
    other.sort_values();
    const std::vector<int>& this_values = values();
    const std::vector<int>& other_values = other.values();
    assert(this_values.size() == other_values.size());
    size_t beat_cnt = 0;
    for (size_t n = 0; n < this_values.size(); n++)
        for (size_t m = 0; m < other_values.size() && this_values[n] > other_values[m]; m++)
            beat_cnt++; // This die side beats the others die side. Note: values are sorted in ascending order, so we can break at first this_values[n] <= other_values[m]
    return (double)beat_cnt / (double)(this_values.size() * other_values.size());
}

// Sorts the values of this die in descending order
void Die::sort_values()
{
    if (!values_sorted)
    {
        std::sort(values().begin(), values().end(), std::less<int>());
        values_sorted = true;
    }
}

// Initializing constructor given the values of two single dice by two vectors
MultiDie::MultiDie(const std::vector<std::vector<int>>& die_values)
{
    assert(die_values.size() > 0);
    for (int n = 1; n < die_values.size(); n++)
    {
        assert(die_values[n].size() > 0 && die_values[n - 1].size() == die_values[n].size());
    }
    init(die_values);
}

// Print the values of all single dice of this dice tuple
std::string MultiDie::print(void) const
{
    std::stringstream str;
    for (int n = 0; n < m_die_values.size(); n++)
        str << (n > 0 ? ", " : "") << "(" << DiceUtil::print(m_die_values[n], 3) << " )";
    return str.str();
}

// Initializes the values of this multi die given the values (eyes) of each single die
void MultiDie::init(const std::vector<std::vector<int>> die_values)
{
    m_die_values = die_values;
    DiceUtil::generate_sums_recursively(m_die_values, 0, 0, m_values);
}

// Searches and returns a list of intransitive paths, each path contains a list of intransitive dice.
// Example: const DicePathList& paths = search_intransitive_paths();
// paths[0] = (D0, D1, D2, D3, D0) means: P(D0>D1) > 0.5, P(D1>D2) > 0.5, P(D2>D3) > 0.5, P(D3>D0) > 0.5.
// paths[1] = (D0, D2, D3, D1, D0) means: P(D0>D2) > 0.5, P(D2>D3) > 0.5, P(D3>D1) > 0.5, P(D1>D0) > 0.5.
template <typename DieType> const DicePathList& DiceSetT<DieType>::search_intransitive_paths(size_t max_final_paths, size_t max_open_paths)
{
    if (!m_intransitive_paths_computed)
    {
        // Create a NxN-matrix (N = number of dice) with matrix(i,j) = 1, if P(D_i>D_j) > 0.5 and i != j, or 0 otherwise.
        assert(m_dice.size() >= 2); // at least 2 dice required
        size_t num_die_values = m_dice[0].num_values();
        size_t beat_cnt_threash = num_die_values * num_die_values / 2; // i.e. P(D_i>D_j) > 0.5, if beat cnt > beat_cnt_threash
        size_t lose_cnt_threash = num_die_values * num_die_values / 2; // i.e. P(D_i>D_j) < 0.5, if beat cnt < lose_cnt_threash
        if (((num_die_values * num_die_values) % 2) != 0)
            lose_cnt_threash++; // example: 9-sided die => 9*9*0.5 = 40.5 => P(D_i>D_j) > 0.5, if beat cnt > 40, P(D_i>D_j) < 0.5, if beat cnt < 41
        SqrMatrix<int> beat_matrix(m_dice.size(), 0); // NxN-matrix with matrix(i,j) = 1 if P(D_i>D_j) > 0.5 for all 0<=i,j<N and i!=j, initialized with 0
        SqrMatrix<int> lose_matrix(m_dice.size(), 0); // NxN-matrix with matrix(i,j) = 1 if P(D_i>D_j) < 0.5 for all 0<=i,j<N and i!=j, initialized with 0
        for (int i = 0; i < (int)m_dice.size(); i++)
            for (int j = i + 1; j < (int)m_dice.size(); j++)
            {
                assert(num_die_values == m_dice[i].num_values() && num_die_values == m_dice[j].num_values());
                std::array<size_t, 2> beats = m_dice[i].count_beats(m_dice[j]);
                if (beats[0] > beat_cnt_threash) // i.e. P(D_i>D_j) > 0.5
                    beat_matrix.at(i, j) = 1;
                if (beats[0] < lose_cnt_threash) // i.e. P(D_i>D_j) < 0.5
                    lose_matrix.at(i, j) = 1;
                if (beats[1] > beat_cnt_threash) // i.e. P(D_j>D_i) > 0.5
                    beat_matrix.at(j, i) = 1;
                if (beats[1] < lose_cnt_threash) // i.e. P(D_j>D_i) > 0.5
                    lose_matrix.at(j, i) = 1;
            }
        // Recursive search of closed cycles in the matrix, which contain all dice exactly once
        DicePathList final_beat_paths, final_lose_paths;
        if (precheck_intransitive_is_possible(beat_matrix)) // Fast pre-check for intransitive chains: All columns of a beat-matrix must have at least one "1", otherwise the dice are not intransitive.
        {
            PRINT_DBG_PATH_SEARCH(std::endl << "search_intransitive_paths(" << m_name << "): beat_matrix =" << std::endl << beat_matrix.print() << "search_intransitive_paths(" << m_name << "): pre-check for beat matrix passed" << std::endl);
            final_beat_paths = search_closed_cycle(beat_matrix, max_final_paths, max_open_paths); // intransitive chains in beat matrix (P(D_i>D_j) > 0.5 for all 0<=i,j<N, i!=j)
            PRINT_DBG_PATH_SEARCH("search_intransitive_paths(" << m_name << "): final beat paths = " << final_beat_paths.print() << std::endl);
        }
        else
            PRINT_DBG_PATH_SEARCH(std::endl << "search_intransitive_paths(" << m_name << "): beat_matrix =" << std::endl << beat_matrix.print() << "search_intransitive_paths(" << m_name << "): pre-check for beat matrix NOT passed" << std::endl);
        if (final_beat_paths.size() < max_final_paths)
        {
            if (precheck_intransitive_is_possible(lose_matrix)) // Fast pre-check for intransitive chains: All columns of a lose-matrix must have at least one "1", otherwise the dice are not intransitive.
            {
                PRINT_DBG_PATH_SEARCH(std::endl << "search_intransitive_paths(" << m_name << "): lose_matrix =" << std::endl << lose_matrix.print() << "search_intransitive_paths(" << m_name << "): pre-check for lose matrix passed" << std::endl);
                final_lose_paths = search_closed_cycle(lose_matrix, max_final_paths - final_beat_paths.size(), max_open_paths); // intransitive chains in lose matrix (P(D_i>D_j) < 0.5 for all 0<=i,j<N, i!=j)
                PRINT_DBG_PATH_SEARCH("search_intransitive_paths(" << m_name << "): final lose paths = " << final_lose_paths.print() << std::endl);
            }
            else
                PRINT_DBG_PATH_SEARCH(std::endl << "search_intransitive_paths(" << m_name << "): lose_matrix =" << std::endl << lose_matrix.print() << "search_intransitive_paths(" << m_name << "): pre-check for lose matrix NOT passed" << std::endl);
        }
        m_intransitive_paths = DicePathList(); // list of all transitive paths (which is empty if the dice are not intransitive)
        m_intransitive_paths.splice(final_beat_paths);
        m_intransitive_paths.splice(final_lose_paths);
        m_intransitive_paths_computed = true;
    }
    return m_intransitive_paths;
}

// Returns true, if this set of dice is intransitive, i.e. there is at least one intransitive path.
template <typename DieType> bool DiceSetT<DieType>::is_intransitive()
{
    if (!m_intransitive_paths_computed)
        search_intransitive_paths();
    return m_intransitive_paths.size() > 0;
}

// Prints the matrix of beat probabilities P(D_i>D_j) for all i, j.
template <typename DieType> void DiceSetT<DieType>::print_probability_matrix()
{
    for (size_t i = 0; i < m_dice.size(); i++)
    {
        for (size_t j = 0; j < m_dice.size(); j++)
            std::cout << " " << std::fixed << std::setprecision(3) << m_dice[i].probability_to_beat(m_dice[j]);
        std::cout << std::endl;
    }
}

// Prints the beat probabilities P(D_i>D_j) for all i, j in a given path.
template <typename DieType> std::string DiceSetT<DieType>::print_path_probabilities(const DicePath& path, bool& path_is_intransitive, bool warn_if_path_is_not_intransitive)
{
    std::stringstream str;
    int probability_flag_sum = 0; // probability_flag_sum += { +1 if P(D_i>D_j) > 0.5, -1 if P(D_i>D_j) < 0.5, 0 otherwise } => probability_flag_sum must be equal to (path.size()-1) or -(path.size()-1) for intransitive paths, otherwise the path is not intransitive
    for (size_t dice_cnt = 1; dice_cnt < path.size(); dice_cnt++)
    {
        int dice_from = path.at(dice_cnt - 1);
        int dice_to = path.at(dice_cnt);
        double probability_to_beat = m_dice[dice_from].probability_to_beat(m_dice[dice_to]);
        assert(probability_to_beat >= 0.0 && probability_to_beat <= 1.0);
        if (probability_to_beat > 0.5)
            probability_flag_sum += 1;
        else if (probability_to_beat < 0.5)
            probability_flag_sum -= 1;
        str << ((dice_cnt > 1) ? ", " : "") << "P(D" << dice_from << ">D" << dice_to << ") = " << std::fixed << std::setprecision(3) << probability_to_beat;
    }
    path_is_intransitive = (probability_flag_sum == ((int)path.size() - 1) || probability_flag_sum == (1 - (int)path.size()));
    str << " (" << (path_is_intransitive ? "" : "NOT an ") << "intransitive path" << path.print_bonus(", ") << ")";

    if (warn_if_path_is_not_intransitive)
    {
        assert(path_is_intransitive); // debug version (assertion)
        if (!path_is_intransitive)   // release version
        {
            std::cerr << std::endl << "## WARNING Path " << path.print() << " in " << name() << " is NOT intransitive, expected an intransitive path!" << std::endl;
            std::cerr << "## Press ENTER to continue..." << std::endl;
            getchar();
        }

    }

    return str.str();
}

// Prints the list of probabilities P(D_i>D_j) for all D_i, D_j in a given path and
// checks their intransitivity, i.e. checks that all P(D_i>D_j) > 0.5 resp. all P(D_i>D_j) < 0.5.
template <typename DieType> std::string DiceSetT<DieType>::print_intransitive_path_probabilities(const DicePath& path)
{
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRTSUVWXYZ";
    std::stringstream str;
    int probability_flag_sum = 0; // probability_flag_sum += { +1 if P(D_i>D_j) > 0.5, -1 if P(D_i>D_j) < 0.5, 0 otherwise } => probability_flag_sum must be equal to (path.size()-1) or -(path.size()-1), otherwise the path is not intransitive
    for (size_t dice_cnt = 1; dice_cnt < path.size(); dice_cnt++)
    {
        int dice_from = path.at(dice_cnt - 1);
        int dice_to = path.at(dice_cnt);
        double probability_to_beat = m_dice[dice_from].probability_to_beat(m_dice[dice_to]);
        if (probability_to_beat > 0.5)
            probability_flag_sum += 1;
        else if (probability_to_beat < 0.5)
            probability_flag_sum -= 1;
        str << ((dice_cnt > 1) ? ", " : "") << "P(" << alphabet[dice_from] << ">" << alphabet[dice_to] << ") = " << std::fixed << std::setprecision(3) << probability_to_beat;
    }
    if (probability_flag_sum != ((int)path.size() - 1) && probability_flag_sum != (1 - (int)path.size()))
    {
        std::cerr << "## WARNING in print_intransitive_path_probabilities(): path " << path.print() << " is NOT intransitive";
        str << "## WARNING in print_intransitive_path_probabilities(): path " << path.print() << " is NOT intransitive";
    }
    return str.str();
}

// Print the dice of this set
template <typename DieType> std::string DiceSetT<DieType>::print_dice(void) const
{
    std::stringstream str;
    for (size_t n = 0; n < m_dice.size(); n++)
        str << " D" << std::setfill('0') << std::setw(2) << n << ": ( " << m_dice[n].print() << " )" << std::endl;
    return str.str();
}

// Searches and returns a list of closed cycles (i.e. an intransitive chain of dice), which contain all dice exactly once.
template <typename DieType> DicePathList DiceSetT<DieType>::search_closed_cycle(const SqrMatrix<int>& matrix, size_t max_final_paths, size_t max_open_paths)
{
    DicePathList open_path_list, closed_path_list;
    std::vector<std::vector<int>> col_indices_per_row = matrix.create_col_indices_foreach_row();
    PRINT_DBG_PATH_SEARCH("search_closed_cycle(" << m_name << "): col_indices_per_row = " << std::endl << DiceUtil::print(col_indices_per_row));
    const std::vector<int>& candidates = col_indices_per_row[0]; // successors of the first die D_0 with P(D_i > D_n) > 0.5, a cycle starts here
    for (size_t n = 0; n < candidates.size(); n++)
        open_path_list.append(DicePath({ 0, candidates[n] }));
    for (size_t n = 0; n < candidates.size() && closed_path_list.size() < max_final_paths; n++)
        search_closed_cycle_recursive(col_indices_per_row, open_path_list, closed_path_list, candidates[n], max_final_paths, max_open_paths);
    // closed_path_list.remove_duplicate_paths(); // test and debugging only (list should not have duplicates)
    PRINT_DBG_PATH_SEARCH("search_closed_cycle(" << m_name << "): final closed_path_list = " << closed_path_list.print() << std::endl);
    return closed_path_list;
}

// Recursive search of a closed cycle, which contains all dice exactly once.
template <typename DieType> void DiceSetT<DieType>::search_closed_cycle_recursive(const std::vector<std::vector<int>>& col_indices_per_row, DicePathList& open_path_list, DicePathList& closed_path_list, int row_idx, size_t max_final_paths, size_t max_open_paths)
{
    PRINT_DBG_PATH_SEARCH("search_closed_cycle_recursive(row_idx=" << row_idx << "): input open_path_list = " << open_path_list.print() << std::endl);
    PRINT_DBG_PATH_SEARCH("search_closed_cycle_recursive(row_idx=" << row_idx << "): input closed_path_list = " << closed_path_list.print() << std::endl);
    std::vector<int> candidates = col_indices_per_row[row_idx]; // all successors of the die D_i with P(D_i > D_n) > 0.5
    PRINT_DBG_PATH_SEARCH("search_closed_cycle_recursive(row_idx=" << row_idx << "): candidates = " << DiceUtil::print(candidates) << std::endl);
    std::vector<int> dice_indices_to_investigate;
    dice_indices_to_investigate.reserve(candidates.size());
    // Sort candidates by row_idx successors to increase the chance for a cycle, such that candidates[0] > row_idx, candidates[1] >  candidates[0], ...<wrap-around>..., candidates[i] < row_idx after sort
    std::sort(candidates.begin(), candidates.end(), std::less<int>());
    if (candidates.size() > 1 && row_idx < candidates.back())
    {
        std::vector<int> candidates_sorted;
        candidates_sorted.reserve(candidates.size());
        std::vector<int>::iterator candidates_sorted_start = candidates.begin();
        while (*candidates_sorted_start < row_idx)
            candidates_sorted_start++;
        std::copy(candidates_sorted_start, candidates.end(), std::back_inserter(candidates_sorted));
        std::copy(candidates.begin(), candidates_sorted_start, std::back_inserter(candidates_sorted));
        candidates = candidates_sorted;
    }
    // We search (and possibly extend) all open paths, which end in dice D_<row_idx>. To speed up iteration over open_path_list to get those paths with back() == row_idx, 
    // we create a list of pointer to the paths with open_path_list[i].back() == row_idx
    std::vector<DicePath*> open_path_list_back_is_row_idx;
    open_path_list_back_is_row_idx.reserve(open_path_list.size());
    for (std::list<DicePath>::iterator path_iter = open_path_list.begin(); path_iter != open_path_list.end(); path_iter++)
        if (path_iter->back() == row_idx)
            open_path_list_back_is_row_idx.push_back(&(*path_iter));
    // Append die candidates to the list of open paths and continue search on all non-cyclic paths
    for (size_t n = 0; n < candidates.size() && closed_path_list.size() < max_final_paths; n++)
    {
        std::vector<DicePath> paths_to_duplicate; // For each path with last die D_<row_idx>, we duplicate this path after appending a new die, such that all paths ending with D_<row_idx> can be extended with all candidates
        int die_idx = candidates[n]; // Die D_<die_idx> is a successor of die D_<row_idx> => Append D_<die_idx> to all paths ending with D_<row_idx>
        if (die_idx == 0) // We started with die D_0 and we landed at die D_0 => We have a cycle, which may or may not contain all dice
        {
            // Append <die_idx> to all paths ending at <row_idx>, but do not further investigate, since the path is a (possibly incomplete) cycle
            PRINT_DBG_PATH_SEARCH("search_closed_cycle_recursive(row_idx=" << row_idx << "): Cycle found, appending " << row_idx << " -> " << die_idx << std::endl);
            for (int open_path_idx = 0; open_path_idx < open_path_list_back_is_row_idx.size() && closed_path_list.size() < max_final_paths; open_path_idx++)
            {
                DicePath* path_iter = open_path_list_back_is_row_idx[open_path_idx];
                assert(path_iter == 0 || path_iter->back() == row_idx);
                if (path_iter)
                {
                    // cycle found, move path to success list
                    DicePath success_path = *path_iter;
                    success_path.append(die_idx);
                    if (success_path.contains_all_dices(col_indices_per_row.size())) // otherwise it's a partial cycle, which we are not interested in
                        closed_path_list.append(success_path);
                }
            }
        }
        else
        {
            // Append die D_<die_idx> to all paths ending with die D_<row_idx> and continue search
            PRINT_DBG_PATH_SEARCH("search_closed_cycle_recursive(row_idx=" << row_idx << "): check " << row_idx << " -> " << die_idx << std::endl);
            bool die_idx_appended = false;
            for (int open_path_idx = 0; open_path_idx < open_path_list_back_is_row_idx.size(); open_path_idx++)
            {
                DicePath* path_iter = open_path_list_back_is_row_idx[open_path_idx];
                assert(path_iter == 0 || path_iter->back() == row_idx);
                if (path_iter && !path_iter->is_in_list(die_idx)) // otherwise it's a partial cycle, which we are not interested in
                {
                    // Duplicate current path for possible other candidates and append the current candidate
                    paths_to_duplicate.push_back(*path_iter);
                    path_iter->append(die_idx);
                    open_path_list_back_is_row_idx[open_path_idx] = 0;
                    die_idx_appended = true;
                }
            }
            if (die_idx_appended) // Die D_<die_idx> appended => continue search with die D_<die_idx>
            {
                PRINT_DBG_PATH_SEARCH("search_closed_cycle_recursive(row_idx=" << row_idx << "): appended " << row_idx << " -> " << die_idx << std::endl);
                dice_indices_to_investigate.push_back(die_idx);
            }
        }
        // Append all pending paths
        for (size_t n = 0; n < paths_to_duplicate.size(); n++)
        {
            open_path_list.append(paths_to_duplicate[n]);
            open_path_list_back_is_row_idx.push_back(&(open_path_list.back()));
        }
    }
    // Remove dead ends, i.e. paths still ending with D_<row_idx>
    open_path_list.remove_paths_by_back_idx(row_idx);
    // open_path_list.remove_duplicate_paths(); // test and debugging only (list should not have duplicates)
    PRINT_DBG_PATH_SEARCH("search_closed_cycle_recursive(row_idx=" << row_idx << "): output open_path_list = " << open_path_list.print() << std::endl);
    PRINT_DBG_PATH_SEARCH("search_closed_cycle_recursive(row_idx=" << row_idx << "): output closed_path_list = " << closed_path_list.print() << std::endl);
    // Continue search recursively
    open_path_list.sort_by_path_length(); // Sort open_path_list by descending path length, such that long paths (with a higher chance to complete) are prefered
    for (size_t n = 0; n < dice_indices_to_investigate.size() && open_path_list.size() < max_open_paths && closed_path_list.size() < max_final_paths; n++)
    {
        search_closed_cycle_recursive(col_indices_per_row, open_path_list, closed_path_list, dice_indices_to_investigate[n], max_final_paths, max_open_paths);
    }
}

// Fast pre-check for intransitive chains: All columns of a beat/lose-matrix must have at least one "1", otherwise the dice are not intransitive.
template <typename DieType> bool DiceSetT<DieType>::precheck_intransitive_is_possible(const SqrMatrix<int>& matrix)
{
    for (int i = 0; i < (int)matrix.size(); i++)
    {
        bool col_has_one = false;
        for (int j = 0; j < (int)matrix.size() && !col_has_one; j++)
            col_has_one = (matrix.at(i, j) == 1);
        if (!col_has_one)
            return false;
    }
    return true;
}

// Explicit template instantiation
template class DiceSetT<Die>;
template class DiceSetT<MultiDie>;
