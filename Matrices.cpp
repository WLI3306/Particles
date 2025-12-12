#include "Matrices.h"
#include <stdexcept>
#include <cmath>
#include <iomanip>

namespace Matrices
{
    // make empty matrix
    Matrix::Matrix(int _rows, int _cols)
    {
        rows = _rows;
        cols = _cols;
        a.assign(rows, std::vector<double>(cols, 0.0));
    }

    // index access
    double& Matrix::operator()(int i, int j)
    {
        if (i < 0 || i >= rows || j < 0 || j >= cols)
            throw std::out_of_range("bad index");
        return a[i][j];
    }

    const double& Matrix::operator()(int i, int j) const
    {
        if (i < 0 || i >= rows || j < 0 || j >= cols)
            throw std::out_of_range("bad index");
        return a[i][j];
    }

    // add matrices
    Matrix operator+(const Matrix& left, const Matrix& right)
    {
        if (left.getRows() != right.getRows() || left.getCols() != right.getCols())
            throw std::runtime_error("size mismatch");

        Matrix r(left.getRows(), left.getCols());
        for (int i = 0; i < r.getRows(); ++i)
            for (int j = 0; j < r.getCols(); ++j)
                r(i, j) = left(i, j) + right(i, j);
        return r;
    }

    // multiply matrices
    Matrix operator*(const Matrix& left, const Matrix& right)
    {
        if (left.getCols() != right.getRows())
            throw std::runtime_error("size mismatch");

        Matrix r(left.getRows(), right.getCols());
        for (int i = 0; i < r.getRows(); ++i)
            for (int k = 0; k < r.getCols(); ++k)
            {
                double sum = 0;
                for (int j = 0; j < left.getCols(); ++j)
                    sum += left(i, j) * right(j, k);
                r(i, k) = sum;
            }
        return r;
    }

    // check equal
    bool operator==(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
            return false;

        for (int i = 0; i < a.getRows(); ++i)
            for (int j = 0; j < a.getCols(); ++j)
                if (std::fabs(a(i, j) - b(i, j)) >= 0.001)
                    return false;
        return true;
    }

    bool operator!=(const Matrix& a, const Matrix& b)
    {
        return !(a == b);
    }

    // print matrix
    std::ostream& operator<<(std::ostream& os, const Matrix& m)
    {
        for (int i = 0; i < m.getRows(); ++i)
        {
            for (int j = 0; j < m.getCols(); ++j)
                os << std::setw(10) << m(i, j) << " ";
            os << "\n";
        }
        os << "\n";
        return os;
    }

    // rotation matrix
    RotationMatrix::RotationMatrix(double t)
        : Matrix(2, 2)
    {
        (*this)(0,0)=cos(t);  (*this)(0,1)=-sin(t);
        (*this)(1,0)=sin(t);  (*this)(1,1)= cos(t);
    }

    // scale matrix
    ScalingMatrix::ScalingMatrix(double s)
        : Matrix(2, 2)
    {
        (*this)(0,0)=s; (*this)(1,1)=s;
    }

    // translation matrix
    TranslationMatrix::TranslationMatrix(double x, double y, int n)
        : Matrix(2, n)
    {
        for (int j = 0; j < n; ++j)
        {
            (*this)(0,j)=x;
            (*this)(1,j)=y;
        }
    }
}
