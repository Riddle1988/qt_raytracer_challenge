#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include "tuple.h"

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

    Matrix<rows,cols>() {
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                m_data[i][j] = 0.0;
            }
        }
    }

    inline void set(int row, int col, double value) { m_data[row][col] = value; }
    inline double get(int row, int col) const { return m_data[row][col]; }
    inline size_t getRowCount() const { return m_rows; }
    inline size_t getColCount() const { return m_cols; }

    inline bool operator==(const Matrix<rows, cols>& matrix) const {

        if (m_rows != matrix.getRowCount()) return false;
        if (m_cols != matrix.getColCount()) return false;

        for (size_t i = 0; i < m_rows; i++) {
            for (size_t j = 0; j < m_cols; j++) {
                if (m_data[i][j] != matrix.get(i, j)) return false;
            }
        }

        return true;
    }

    inline bool operator!=(const Matrix<rows, cols>& matrix) const {
        return !(*this == matrix);
    }

    inline Matrix<4,4> operator*(const Matrix<4,4>& multiplier) const;
    inline Tuple operator*(const Tuple& tuple) const;

    inline Matrix<rows, cols> transpose() const {
        Matrix<rows, cols> result;

        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                int row = static_cast<int>(i);
                int col = static_cast<int>(j);

                result.set(col, row, m_data[row][col]);
            }
        }

        return result;
    }

    inline double determinant() const;

    inline Matrix<rows-1, cols-1> submatrix(int rowToRemove, int colToRemove) const {
        Matrix<rows-1, cols-1> result;

        int sourceRow = 0;
        for (size_t i = 0; i < rows-1; i++) {
            int row = static_cast<int>(i);

            // skip this row
            if (row == rowToRemove) { sourceRow++; }

            int sourceCol = 0;
            for (size_t j = 0; j < cols-1; j++) {
                int col = static_cast<int>(j);

                // skip this col
                if (col == colToRemove) { sourceCol++; }

                result.set(row, col, m_data[sourceRow][sourceCol]);
                sourceCol++;
            }
            sourceRow++;
        }
        return result;
    }

private:

    size_t m_rows = rows;
    size_t m_cols = cols;

    double m_data[rows][cols]{};

};

// 2x2 specializations
template<>
inline double Matrix<2, 2>::determinant() const {
    // [a b]
    // [c d]
    //
    // determinant = a*d - b*c

    double ad = m_data[0][0] * m_data[1][1];
    double bc = m_data[0][1] * m_data[1][0];

    double result = ad - bc;

    return result;
}

// 4x4 specializations
template<>
inline Tuple Matrix<4, 4>::operator*(const Tuple& tuple) const {
    double result[4];
    for (size_t i = 0; i < 4; i++) {
            result[i] = m_data[i][0] * tuple.x() +
                        m_data[i][1] * tuple.y() +
                        m_data[i][2] * tuple.z() +
                        m_data[i][3] * tuple.w();
        }

    return {result[0], result[1], result[2], result[3]};
}

template<>
inline Matrix<4, 4> Matrix<4,4>::operator*(const Matrix<4, 4>& multiplier) const {
    Matrix<4,4> result;
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            int row = static_cast<int>(i);
            int col = static_cast<int>(j);
            auto value = m_data[row][0] * multiplier.get(0, col) +
                         m_data[row][1] * multiplier.get(1, col) +
                         m_data[row][2] * multiplier.get(2, col) +
                         m_data[row][3] * multiplier.get(3, col);

            result.set(row, col, value);
        }
    }

    return result;
}

static const Matrix<4,4> identity_matrix = Matrix<4,4>(
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1);

#endif // MATRIX_H
