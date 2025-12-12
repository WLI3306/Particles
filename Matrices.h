#ifndef MATRICES_H
#define MATRICES_H

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

namespace Matrices
{
    // basic matrix
    class Matrix
    {
    protected:
        int rows;
        int cols;
        std::vector<std::vector<double>> a;

    public:
        Matrix(int _rows, int _cols);

        double& operator()(int i, int j);
        const double& operator()(int i, int j) const;

        int getRows() const
        {
            return rows;
        }

        int getCols() const
        {
            return cols;
        }
    };

    // rotate points
    class RotationMatrix : public Matrix
    {
    public:
        RotationMatrix(double theta);
    };

    // scale points
    class ScalingMatrix : public Matrix
    {
    public:
        ScalingMatrix(double scale);
    };

    // move points
    class TranslationMatrix : public Matrix
    {
    public:
        TranslationMatrix(double xShift, double yShift, int nCols);
    };

    // matrix ops
    Matrix operator+(const Matrix& a, const Matrix& b);
    Matrix operator*(const Matrix& a, const Matrix& b);
    bool operator==(const Matrix& a, const Matrix& b);
    bool operator!=(const Matrix& a, const Matrix& b);

    std::ostream& operator<<(std::ostream& os, const Matrix& m);
}

#endif
