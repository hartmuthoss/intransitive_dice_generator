// Matrices for dice
#pragma once
#include <sstream>
#include <string>
#include <vector>

// NxN-matrix stored in a flat memory model
template <typename MatType> class SqrMatrix
{
public:

  SqrMatrix(size_t N = 0, MatType initial_value = 0) : m_matrix(N* N, initial_value), m_stride(N)
  {
  }

  inline MatType& at(int i, int j)
  { 
    return at(j * m_stride + i);
  }
  
  inline const MatType& at(int i, int j) const
  { 
    return at(j * m_stride + i); 
  }
  
  inline MatType& at(size_t n)
  {
    assert(n >= 0 && n < m_matrix.size());
    return m_matrix[n];
  }

  inline const MatType& at(size_t n) const
  {
    assert(n >= 0 && n < m_matrix.size());
    return m_matrix[n];
  }

  inline size_t size(void) const
  { 
    return m_stride; 
  }

  // Returns a list of column indices for each row, which have a value > 0.
  // Example: std::vector<std::vector<int>> col_indices_per_row = create_col_indices_foreach_row()
  // Then n = col_indices_per_row[i][0] is the first dice with P(D_i > D_n) > 0.5 (if it exists, i.e.  col_indices_per_row[i].size() > 0).
  // m = col_indices_per_row[i][1] is another dice with P(D_i > D_m) > 0.5 (if it exists, i.e.  col_indices_per_row[i].size() > 1).
  std::vector<std::vector<int>> create_col_indices_foreach_row(void) const
  {
    std::vector<std::vector<int>> indices_per_row(m_stride);
    for (int i = 0; i < (int)m_stride; i++)
    {
      indices_per_row[i].reserve(m_stride);
      for (int j = 0; j < (int)m_stride; j++)
      {
        if (at(i, j) > 0)
          indices_per_row[i].push_back(j);
      }
    }
    return indices_per_row;
  }

  // Formats the values of this matrix into a string
  std::string print(void) const
  {
    std::stringstream str;
    for (int i = 0; i < (int)m_stride; i++)
    {
      for (int j = 0; j < (int)m_stride; j++)
        str << " " << at(i, j);
      str << std::endl;
    }
    return str.str();
  }

protected:

  std::vector<MatType> m_matrix; // flat memory
  size_t m_stride = 0; // i.e. N
};

