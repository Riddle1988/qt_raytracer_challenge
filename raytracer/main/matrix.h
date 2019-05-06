#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

template <size_t rows, size_t cols>
class Matrix
{
public:

    // 2x2
    Matrix(
            double m00,
            double m01,
            double m10,
            double m11
          )
    {
        m_data[0][0] = m00;
        m_data[0][1] = m01;

        m_data[1][0] = m10;
        m_data[1][1] = m11;
    }

    // 3x3
    Matrix(
            double m00,
            double m01,
            double m02,
            double m10,
            double m11,
            double m12,
            double m20,
            double m21,
            double m22
          )
    {
        m_data[0][0] = m00;
        m_data[0][1] = m01;
        m_data[0][2] = m02;

        m_data[1][0] = m10;
        m_data[1][1] = m11;
        m_data[1][2] = m12;

        m_data[2][0] = m20;
        m_data[2][1] = m21;
        m_data[2][2] = m22;
    }

    // 4x4
    Matrix(
            double m00,
            double m01,
            double m02,
            double m03,
            double m10,
            double m11,
            double m12,
            double m13,
            double m20,
            double m21,
            double m22,
            double m23,
            double m30,
            double m31,
            double m32,
            double m33
          )
    {
        m_data[0][0] = m00;
        m_data[0][1] = m01;
        m_data[0][2] = m02;
        m_data[0][3] = m03;

        m_data[1][0] = m10;
        m_data[1][1] = m11;
        m_data[1][2] = m12;
        m_data[1][3] = m13;

        m_data[2][0] = m20;
        m_data[2][1] = m21;
        m_data[2][2] = m22;
        m_data[2][3] = m23;

        m_data[3][0] = m30;
        m_data[3][1] = m31;
        m_data[3][2] = m32;
        m_data[3][3] = m33;
    }

    inline double get(int row, int col) const { return m_data[row][col]; }
    inline size_t getRowCount() const { return m_rows; }
    inline size_t getColCount() const { return m_cols; }

    bool operator==(const Matrix<rows, cols>& matrix) const {

        if (m_rows != matrix.getRowCount()) return false;
        if (m_cols != matrix.getColCount()) return false;

        for (size_t i = 0; i < m_rows; i++) {
            for (size_t j = 0; j < m_cols; j++) {
                if (m_data[i][j] != matrix.get(i, j)) return false;
            }
        }

        return true;
    }

    bool operator!=(const Matrix<rows, cols>& matrix) const {
        return !(*this == matrix);
    }


private:

    size_t m_rows = rows;
    size_t m_cols = cols;

    double m_data[rows][cols];
};

#endif // MATRIX_H