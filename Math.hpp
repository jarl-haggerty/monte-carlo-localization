/*
 * Math.hpp
 *
 *  Created on: Apr 18, 2012
 *      Author: jarl
 */

#ifndef MATH_HPP_
#define MATH_HPP_

#include <iostream>
#include <float.h>
#include <math.h>

namespace jarl
{
typedef double Scalar;
const Scalar ScalarMax = DBL_MAX;
const Scalar PI = M_PI;
class Vector
{
public:
    Scalar x, y;
    Vector();
    Vector(const Vector& vector);
    Vector(Scalar, Scalar);
    bool valid();
    Scalar length();
    Scalar cross(const Vector&);
    Vector unit();
    Vector quarterLeft();
    Vector quarterRight();
    Vector& operator=(const Vector&);
    Vector& operator+=(const Vector&);
    Vector operator+(const Vector&);
    Vector& operator-=(const Vector&);
    Vector operator-(const Vector&);
    Vector operator-();
    Scalar operator*(const Vector&);
    Vector& operator*=(const Scalar&);
    Vector operator/(const Scalar&);
    Vector& operator/=(const Scalar&);
};
class Matrix
{
public:
    Scalar ul, ll, lr, ur;
    Matrix();
    Matrix(Scalar, Scalar, Scalar, Scalar);
    bool valid();
    Scalar determinate() const;
    Matrix inverse() const;
    const Scalar& operator()(const int&, const int&) const;
    Scalar& operator()(const int&, const int&);
    Matrix& operator=(const Matrix&);
    Matrix& operator+=(const Matrix&);
    Matrix operator+(const Matrix&);
    Matrix& operator-=(const Matrix&);
    Matrix operator-(const Matrix&);
    Matrix operator-();
    Vector operator*(const Vector&);
    Matrix operator*(const Matrix&);
    Matrix& operator*=(const Matrix&);
    Matrix& operator*=(const Scalar&);
    Matrix operator/(const Matrix&);
    Matrix& operator/=(const Matrix&);
    Matrix& operator/=(const Scalar&);
};
class Transform
{
public:
    Matrix rotation;
    Vector translation;
    Transform();
    Transform(Matrix, Vector);
    bool valid();
    Transform inverse() const;
    Transform& operator=(const Transform&);
    Vector operator()(const Vector&);
    Transform operator*(const Transform&);
    Transform& operator*=(const Transform&);
    Transform operator/(const Transform&);
    Transform& operator/=(const Transform&);
};
}

jarl::Vector operator*(const jarl::Scalar&, const jarl::Vector&);
jarl::Vector operator*(const jarl::Vector&, const jarl::Scalar&);
jarl::Matrix operator*(const jarl::Scalar&, const jarl::Matrix&);
jarl::Matrix operator*(const jarl::Matrix&, const jarl::Scalar&);
jarl::Matrix operator/(const jarl::Scalar&, const jarl::Matrix&);
jarl::Matrix operator/(const jarl::Matrix&, const jarl::Scalar&);

std::ostream & operator<<(std::ostream &, jarl::Vector const &);
std::ostream & operator<<(std::ostream &, jarl::Matrix const &);
std::ostream & operator<<(std::ostream &, jarl::Transform const &);

#endif /* MATH_HPP_ */
