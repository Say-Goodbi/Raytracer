/**
 * @file Matrix.hpp
 * @brief Fixed-size generic matrix template used across geometry types.
 *
 * The Matrix template provides a small, stack-allocated, row-major storage
 * for floating point values and implements common matrix operations used by
 * the geometry primitives. It is intentionally lightweight and header-only
 * so it can be used as a base class for `Point` and `Vector` specializations.
 *
 * Template parameters:
 * - Rows: number of rows (compile-time constant)
 * - Cols: number of columns (compile-time constant)
 *
 * Notes:
 * - Storage is a plain C-style array of `float` in row-major order.
 * - Default constructor zero-initializes the contents.
 * - A copy constructor is defaulted to allow trivial copying.
 * - `identity()` is only available for square matrices (Rows == Cols).
 */

#pragma once

namespace Geometry
{
    template<unsigned int Rows, unsigned int Cols>
    class Matrix
    {
        protected:
            /**
             * Row-major storage of matrix elements.
             * Access with `data[row][col]` where 0 <= row < Rows and 0 <= col < Cols.
             */
            float data[Rows][Cols];

        public:

            /** Defaulted copy constructor (trivial copy of storage). */
            Matrix(const Matrix<Rows, Cols>& other) = default;

            /**
             * Construct from a C-style array. Copies Rows*Cols floats into storage.
             * The parameter uses the same row-major layout as the internal storage.
             */
            Matrix(float data[Rows][Cols]) 
            {
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        this->data[i][j] = data[i][j];
            }

            /**
             * Default constructor: zero-initialize all elements.
             */
            Matrix()
            {
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        data[i][j] = 0.0f;
            }

            ~Matrix() = default;

            /**
             * Create an identity matrix. Only valid when Rows == Cols.
             * The diagonal elements are set to 1.0f, others remain 0.0f.
             */
            static Matrix<Rows, Cols> identity() {
                static_assert(Rows == Cols, "Identity matrix must be square");
                Matrix<Rows, Cols> result;
                for (int i = 0; i < Rows && i < Cols; ++i)
                    result.data[i][i] = 1.0f;
                return result;
            }

            /** Read-only element access. No bounds-checking. */
            float operator()(unsigned int row, unsigned int col) const {
                return data[row][col];
            }

            /** Mutable element access. No bounds-checking. */
            float& operator()(unsigned int row, unsigned int col) {
                return data[row][col];
            }

            /** Element-wise addition. Returns a new matrix. */
            Matrix<Rows, Cols> operator+(const Matrix<Rows, Cols>& other) const {
                Matrix<Rows, Cols> result;
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        result.data[i][j] = data[i][j] + other.data[i][j];
                return result;
            }

            /** In-place element-wise addition. */
            Matrix<Rows, Cols>& operator+=(const Matrix<Rows, Cols>& other) {
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        data[i][j] += other.data[i][j];
                return *this;
            }

            /** Element-wise subtraction. Returns a new matrix. */
            Matrix<Rows, Cols> operator-(const Matrix<Rows, Cols>& other) const {
                Matrix<Rows, Cols> result;
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        result.data[i][j] = data[i][j] - other.data[i][j];
                return result;
            }

            /** In-place element-wise subtraction. */
            Matrix<Rows, Cols>& operator-=(const Matrix<Rows, Cols>& other) {
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        data[i][j] -= other.data[i][j];
                return *this;
            }

            /**
             * Matrix multiplication. Multiplies this (Rows x Cols) by
             * `other` (Cols x OtherCols) to produce a (Rows x OtherCols) matrix.
             */
            template<unsigned int OtherCols>
            Matrix<Rows, OtherCols> operator*(const Matrix<Cols, OtherCols>& other) const {
                Matrix<Rows, OtherCols> result;
                for (int i = 0; i < Rows; ++i) {
                    for (int j = 0; j < OtherCols; ++j) {
                        float sum = 0.0f;
                        for (int k = 0; k < Cols; ++k) {
                            sum += data[i][k] * other(k,j);
                        }
                        result(i,j) = sum;
                    }
                }
                return result;
            }

            /** In-place matrix multiplication (assigns result). */
            template<unsigned int OtherCols>
            Matrix<Rows, OtherCols>& operator*=(const Matrix<Cols, OtherCols>& other) {
                *this = (*this) * other;
                return *this;
            }

            /** Scalar multiplication. Returns a new matrix with every element scaled. */
            Matrix<Rows, Cols> operator*(float scalar) const {
                Matrix<Rows, Cols> result;
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        result.data[i][j] = data[i][j] * scalar;
                return result;
            }

            /** In-place scalar multiplication. */
            Matrix<Rows, Cols>& operator*=(float scalar) {
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        data[i][j] *= scalar;
                return *this;
            }
    };
}