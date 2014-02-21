/*
 * Math.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: jarl
 */

#include "Math.hpp"

//const jarl::Scalar jarl::ScalarMax = DBL_MAX;
//const jarl::Scalar jarl::PI = M_PI;

jarl::Vector::Vector() :
    x(jarl::ScalarMax), y(jarl::ScalarMax)
{
}
jarl::Vector::Vector(const jarl::Vector& vector) :
    x(vector.x), y(vector.y)
{
}
jarl::Vector::Vector(jarl::Scalar x, jarl::Scalar y) :
    x(x), y(y)
{
}
bool jarl::Vector::valid()
{
    return x != jarl::ScalarMax && y != jarl::ScalarMax;
}
jarl::Vector jarl::Vector::quarterLeft()
{
    return Vector(-y, x);
}
jarl::Vector jarl::Vector::quarterRight()
{
    return Vector(y, -x);
}
jarl::Scalar jarl::Vector::length()
{
    return sqrt(*this * *this);
}
jarl::Scalar jarl::Vector::cross(const jarl::Vector& that)
{
    return x * that.y - y * that.x;
}
jarl::Vector jarl::Vector::unit()
{
    return *this / length();
}
jarl::Vector& jarl::Vector::operator=(const jarl::Vector& that)
{
    x = that.x;
    y = that.y;
    return *this;
}
jarl::Vector& jarl::Vector::operator+=(const jarl::Vector& that)
{
    x += that.x;
    y += that.y;
    return *this;
}
jarl::Vector jarl::Vector::operator+(const jarl::Vector& that)
{
    jarl::Vector result = *this;
    result += that;
    return result;
}
jarl::Vector& jarl::Vector::operator-=(const jarl::Vector& that)
{
    x -= that.x;
    y -= that.y;
    return *this;
}
jarl::Vector jarl::Vector::operator-(const jarl::Vector& that)
{
    jarl::Vector result = *this;
    result -= that;
    return result;
}
jarl::Vector jarl::Vector::operator-()
{
    jarl::Vector result;
    result.x = -x;
    result.x = -x;
    return result;
}
jarl::Vector& jarl::Vector::operator*=(const jarl::Scalar& that)
{
    x *= that;
    y *= that;
    return *this;
}
jarl::Scalar jarl::Vector::operator*(const jarl::Vector& that)
{
    return x * that.x + y * that.y;
}
jarl::Vector& jarl::Vector::operator/=(const jarl::Scalar& that)
{
    x /= that;
    y /= that;
    return *this;
}
jarl::Vector jarl::Vector::operator/(const jarl::Scalar& that)
{
    jarl::Vector result = *this;
    result /= that;
    return result;
}

jarl::Vector operator*(const jarl::Scalar& scalar,
                          const jarl::Vector& vector)
{
    jarl::Vector result = vector;
    result *= scalar;
    return result;
}
jarl::Vector operator*(const jarl::Vector& vector,
                          const jarl::Scalar& scalar)
{
    jarl::Vector result = vector;
    result *= scalar;
    return result;
}

jarl::Matrix::Matrix() :
    ul(jarl::ScalarMax), ll(jarl::ScalarMax), lr(jarl::ScalarMax), ur(jarl::ScalarMax)
{
}
bool jarl::Matrix::valid()
{
    return ul != jarl::ScalarMax && ll != jarl::ScalarMax && lr != jarl::ScalarMax && ur != jarl::ScalarMax;
}
jarl::Matrix::Matrix(jarl::Scalar ul, jarl::Scalar ll,
                        jarl::Scalar ur, jarl::Scalar lr) :
    ul(ul), ll(ll), lr(lr), ur(ur)
{
}
const jarl::Scalar& jarl::Matrix::operator()(const int& row,
        const int& column) const
{
    if (row == 0)
    {
        if (column == 0)
        {
            return ul;
        }
        else
        {
            return ur;
        }
    }
    else
    {
        if (column == 0)
        {
            return ll;
        }
        else
        {
            return lr;
        }
    }
}
jarl::Scalar& jarl::Matrix::operator()(const int& row, const int& column)
{
    if (row == 0)
    {
        if (column == 0)
        {
            return ul;
        }
        else
        {
            return ur;
        }
    }
    else
    {
        if (column == 0)
        {
            return ll;
        }
        else
        {
            return lr;
        }
    }
}
jarl::Matrix& jarl::Matrix::operator=(const jarl::Matrix& that)
{
    ul = that.ul;
    ll = that.ll;
    lr = that.lr;
    ur = that.ur;
    return *this;
}
jarl::Matrix& jarl::Matrix::operator+=(const jarl::Matrix& that)
{
    ul += that.ul;
    ll += that.ll;
    lr += that.lr;
    ur += that.ur;
    return *this;
}
jarl::Matrix jarl::Matrix::operator+(const jarl::Matrix& that)
{
    jarl::Matrix result = *this;
    result += that;
    return result;
}
jarl::Matrix& jarl::Matrix::operator-=(const jarl::Matrix& that)
{
    ul -= that.ul;
    ll -= that.ll;
    lr -= that.lr;
    ur -= that.ur;
    return *this;
}
jarl::Matrix jarl::Matrix::operator-(const jarl::Matrix& that)
{
    jarl::Matrix result = *this;
    result -= that;
    return result;
}
jarl::Matrix jarl::Matrix::operator-()
{
    jarl::Matrix result;
    ul = -ul;
    ll = -ll;
    lr = -lr;
    ur = -ur;
    return result;
}
jarl::Matrix& jarl::Matrix::operator*=(const jarl::Scalar& that)
{
    ul *= that;
    ll *= that;
    lr *= that;
    ur *= that;
    return *this;
}
jarl::Matrix& jarl::Matrix::operator*=(const jarl::Matrix& that)
{
    jarl::Matrix old = *this;
    ul = old.ul * that.ul + old.ur * that.ll;
    ll = old.ll * that.ul + old.lr * that.ll;
    ur = old.ul * that.ur + old.ur * that.lr;
    lr = old.ll * that.ur + old.lr * that.lr;
    return *this;
}
jarl::Vector jarl::Matrix::operator*(const jarl::Vector& that)
{
    jarl::Vector result;
    result.x = ul * that.x + ur * that.y;
    result.y = ll * that.x + lr * that.y;
    return result;
}
jarl::Matrix jarl::Matrix::operator*(const jarl::Matrix& that)
{
    jarl::Matrix result = *this;
    result *= that;
    return result;
}
jarl::Matrix& jarl::Matrix::operator/=(const jarl::Scalar& that)
{
    ul /= that;
    ll /= that;
    lr /= that;
    ur /= that;
    return *this;
}
jarl::Matrix& jarl::Matrix::operator/=(const jarl::Matrix& that)
{
    jarl::Matrix inverted = that.inverse();
    jarl::Matrix old = *this;
    ul = old.ul * inverted.ul + old.ur * inverted.ll;
    ll = old.ll * inverted.ul + old.lr * inverted.ll;
    ur = old.ul * inverted.ur + old.ur * inverted.lr;
    lr = old.ll * inverted.ur + old.lr * inverted.lr;
    return *this;
}
jarl::Scalar jarl::Matrix::determinate() const
{
    return ul * lr - ll * ur;
}
jarl::Matrix jarl::Matrix::inverse() const
{
    jarl::Matrix inverted(lr, -ll, -ur, ul);
    inverted /= determinate();
    return inverted;
}
jarl::Matrix jarl::Matrix::operator/(const jarl::Matrix& that)
{
    jarl::Matrix result = *this;
    result /= that;
    return result;
}

jarl::Matrix operator*(const jarl::Scalar& scalar,
                          const jarl::Matrix& matrix)
{
    jarl::Matrix result = matrix;
    result *= scalar;
    return result;
}
jarl::Matrix operator*(const jarl::Matrix& matrix,
                          const jarl::Scalar& scalar)
{
    jarl::Matrix result = matrix;
    result *= scalar;
    return result;
}
jarl::Matrix operator/(const jarl::Scalar& scalar,
                          const jarl::Matrix& matrix)
{
    jarl::Matrix result = matrix.inverse();
    result *= scalar;
    return result;
}
jarl::Matrix operator/(const jarl::Matrix& matrix,
                          const jarl::Scalar& scalar)
{
    jarl::Matrix result = matrix;
    result *= scalar;
    return result;
}

jarl::Transform::Transform()
{
}

jarl::Transform::Transform(jarl::Matrix rotation,
                              jarl::Vector translation) :
    rotation(rotation), translation(translation)
{
}

bool jarl::Transform::valid()
{
    return rotation.valid() && translation.valid();
}

jarl::Transform jarl::Transform::inverse() const
{
    jarl::Matrix inverse = rotation.inverse();
    return jarl::Transform(inverse, -inverse * translation);
}
jarl::Transform& jarl::Transform::operator=(
    const jarl::Transform& transform)
{
    rotation = transform.rotation;
    translation = transform.translation;
    return *this;
}
jarl::Vector jarl::Transform::operator()(const jarl::Vector& vector)
{
    return rotation * vector + translation;
}
jarl::Transform jarl::Transform::operator*(
    const jarl::Transform& transform)
{
    jarl::Transform result = *this;
    result *= transform;
    return result;
}
jarl::Transform& jarl::Transform::operator*=(
    const jarl::Transform& transform)
{
    rotation *= transform.rotation;
    translation += transform.translation;
    return *this;
}
jarl::Transform jarl::Transform::operator/(
    const jarl::Transform& transform)
{
    jarl::Transform result = *this;
    result /= transform;
    return result;
}
jarl::Transform& jarl::Transform::operator/=(
    const jarl::Transform& transform)
{
    rotation /= transform.rotation;
    translation -= transform.translation;
    return *this;
}

std::ostream & operator<<(std::ostream & stream,
                          jarl::Vector const & vector)
{
    stream << "(" << vector.x << "," << vector.y << ")";
    return stream;
}
std::ostream & operator<<(std::ostream & stream,
                          jarl::Matrix const & matrix)
{
    stream << "[" << jarl::Vector(matrix.ul, matrix.ll) << ","
    << jarl::Vector(matrix.ur, matrix.lr) << "]";
    return stream;
}
std::ostream & operator<<(std::ostream & stream,
                          jarl::Transform const & transform)
{
    stream << "<" << transform.rotation << "," << transform.translation << ">";
    return stream;
}
