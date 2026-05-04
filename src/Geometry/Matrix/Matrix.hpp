#pragma once

namespace Geometry
{
    template<unsigned int Rows, unsigned int Cols>
    class Matrix
    {
        protected:
            float data[Rows][Cols];

        public:
            Matrix(const Matrix<Rows, Cols>& other) = default;

            Matrix(float data[Rows][Cols]) 
            {
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        this->data[i][j] = data[i][j];
            }

            Matrix()
            {
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        data[i][j] = 0.0f;
            }

            ~Matrix() = default;

            static Matrix<Rows, Cols> identity() {
                static_assert(Rows == Cols, "Identity matrix must be square");
                Matrix<Rows, Cols> result;
                for (int i = 0; i < Rows && i < Cols; ++i)
                    result.data[i][i] = 1.0f;
                return result;
            }

            float operator()(unsigned int row, unsigned int col) const {
                return data[row][col];
            }

            float& operator()(unsigned int row, unsigned int col) {
                return data[row][col];
            }

            Matrix<Rows, Cols> operator+(const Matrix<Rows, Cols>& other) const {
                Matrix<Rows, Cols> result;
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        result.data[i][j] = data[i][j] + other.data[i][j];
                return result;
            }

            Matrix<Rows, Cols>& operator+=(const Matrix<Rows, Cols>& other) {
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        data[i][j] += other.data[i][j];
                return *this;
            }

            Matrix<Rows, Cols> operator-(const Matrix<Rows, Cols>& other) const {
                Matrix<Rows, Cols> result;
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        result.data[i][j] = data[i][j] - other.data[i][j];
                return result;
            }

            Matrix<Rows, Cols>& operator-=(const Matrix<Rows, Cols>& other) {
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        data[i][j] -= other.data[i][j];
                return *this;
            }

            template<unsigned int OtherCols>
            Matrix<Rows, OtherCols> operator*(const Matrix<Cols, OtherCols>& other) const {
                Matrix<Rows, OtherCols> result;
                for (int i = 0; i < Rows; ++i) {
                    for (int j = 0; j < OtherCols; ++j) {
                        float sum = 0.0f;
                        for (int k = 0; k < Cols; ++k) {
                            sum += data[i][k] * other.data[k][j];
                        }
                        result.data[i][j] = sum;
                    }
                }
                return result;
            }

            template<unsigned int OtherCols>
            Matrix<Rows, OtherCols>& operator*=(const Matrix<Cols, OtherCols>& other) {
                *this = (*this) * other;
                return *this;
            }

            Matrix<Rows, Cols> operator*(float scalar) const {
                Matrix<Rows, Cols> result;
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        result.data[i][j] = data[i][j] * scalar;
                return result;
            }

            Matrix<Rows, Cols>& operator*=(float scalar) {
                for (int i = 0; i < Rows; ++i)
                    for (int j = 0; j < Cols; ++j)
                        data[i][j] *= scalar;
                return *this;
            }
    };
}