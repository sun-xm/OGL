#pragma once

#include <initializer_list>
#include <cfloat>
#include <cmath>
#include <cstdint>

template<typename Scalar>
inline Scalar sqrtx(Scalar s)
{
    return sqrt(s);
}

template<>
inline float sqrtx(float s)
{
    return sqrtf(s);
}

template<>
inline long double sqrtx(long double s)
{
    return sqrtl(s);
}

template<typename Scalar>
inline Scalar ToRadian(Scalar degree)
{
    return degree * (Scalar)3.14159265358979323846264338327950288419716939937510l / (Scalar)180.0;
}

template<typename Scalar>
inline Scalar ToDegree(Scalar radian)
{
    return radian / (Scalar)3.14159265358979323846264338327950288419716939937510l * (Scalar)180.0;
}

template<typename Scalar, size_t Dimensions>
struct Vector
{
    Scalar v[Dimensions];

    Vector() = default;
    Vector(Scalar v)
    {
        for (size_t i = 0; i < Dimensions; i++)
        {
            this->v[i] = v;
        }
    }
    Vector(const Scalar* v)
    {
        for (size_t i = 0; i < Dimensions; i++)
        {
            this->v[i] = v[i];
        }
    }
    Vector(const std::initializer_list<Scalar>& list)
    {
        auto l = list.begin();
        for (size_t i = 0; i < Dimensions; i++)
        {
            this->v[i] = (list.end() == l) ? 0 : *l++;
        }
    }

    bool IsNaN() const
    {
        for (Scalar v : this->v)
        {
            if (isnan(v))
            {
                return true;
            }
        }

        return false;
    }

    Scalar& operator[](size_t index)
    {
        return this->v[index];
    }
    const Scalar& operator[](size_t index) const
    {
        return this->v[index];
    }
    operator Scalar*()
    {
        return this->v;
    }
    operator const Scalar*() const
    {
        return this->v;
    }
};

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions> operator+(const Vector<Scalar, Dimensions>& first, const Vector<Scalar, Dimensions>& second)
{
    Vector<Scalar, Dimensions> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.v[i] = first.v[i] + second.v[i];
    }

    return result;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions> operator-(const Vector<Scalar, Dimensions>& first, const Vector<Scalar, Dimensions>& second)
{
    Vector<Scalar, Dimensions> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.v[i] = first.v[i] - second.v[i];
    }

    return result;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions> operator*(const Vector<Scalar, Dimensions>& vector, Scalar scale)
{
    Vector<Scalar, Dimensions> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.v[i] = vector.v[i] * scale;
    }

    return result;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions> operator*(Scalar scale, const Vector<Scalar, Dimensions>& vector)
{
    return vector * scale;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions> operator/(const Vector<Scalar, Dimensions>& vector, Scalar scale)
{
    Vector<Scalar, Dimensions> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.v[i] = vector.v[i] / scale;
    }

    return result;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions> operator-(const Vector<Scalar, Dimensions>& vector)
{
    Vector<Scalar, Dimensions> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.v[i] = -vector.v[i];
    }

    return result;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions>& operator+=(Vector<Scalar, Dimensions>& first, const Vector<Scalar, Dimensions>& second)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        first.v[i] += second.v[i];
    }

    return first;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions>& operator-=(Vector<Scalar, Dimensions>& first, const Vector<Scalar, Dimensions>& second)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        first.v[i] -= second.v[i];
    }

    return first;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions>& operator+=(Vector<Scalar, Dimensions>& first, Scalar second)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        first.v[i] += second;
    }

    return first;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions>& operator-=(Vector<Scalar, Dimensions>& first, Scalar second)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        first.v[i] -= second;
    }

    return first;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions>& operator*=(Vector<Scalar, Dimensions>& vector, Scalar scale)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        vector.v[i] *= scale;
    }

    return vector;
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions>& operator/=(Vector<Scalar, Dimensions>& vector, Scalar scale)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        vector.v[i] /= scale;
    }

    return vector;
}

template<typename Scalar, size_t Dimensions>
inline Scalar Dot(const Vector<Scalar, Dimensions>& v0, const Vector<Scalar, Dimensions>& v1)
{
    Scalar product = 0;
    for (size_t i = 0; i < Dimensions; i++)
    {
        product += v0.v[i] * v1.v[i];
    }
    return product;
}

template<typename Scalar, size_t Dimensions>
inline Scalar Length(const Vector<Scalar, Dimensions>& v)
{
    return sqrtx(Dot(v, v));
}

template<typename Scalar, size_t Dimensions>
inline Vector<Scalar, Dimensions> Normalize(const Vector<Scalar, Dimensions>& v)
{
    return v / Length(v);
}

template<typename Scalar>
inline Scalar Dot(const Vector<Scalar, 2>& v0, const Vector<Scalar, 2>& v1)
{
    return v0.v[0] * v1.v[0] + v0.v[1] * v1.v[1];
}

template<typename Scalar>
inline Scalar Dot(const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1)
{
    return v0.v[0] * v1.v[0] + v0.v[1] * v1.v[1] + v0.v[2] * v1.v[2];
}

template<typename Scalar>
inline Vector<Scalar, 3> Cross(const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1)
{
    return Vector<Scalar, 3>{ v0.v[1] * v1.v[2] - v0.v[2] * v1.v[1],
                              v0.v[2] * v1.v[0] - v0.v[0] * v1.v[2],
                              v0.v[0] * v1.v[1] - v0.v[1] * v1.v[0] };
}

template<typename Scalar>
inline Vector<Scalar, 3> Project(const Vector<Scalar, 3>& vector, const Vector<Scalar, 4>& plane)
{
    auto a = plane.X;
    auto b = plane.Y;
    auto c = plane.Z;
    auto d = plane.W;

    auto a2 = a * a;
    auto b2 = b * b;
    auto c2 = c * c;

    auto x = ((b2 + c2) * vector.X - a * (b * vector.Y + c * vector.Z + d)) / (a2 + b2 + c2);
    auto y = ((a2 + c2) * vector.Y - b * (a * vector.X + c * vector.Z + d)) / (a2 + b2 + c2);
    auto z = ((a2 + b2) * vector.Z - c * (a * vector.X + b * vector.Y + d)) / (a2 + b2 + c2);

    return Vector<Scalar, 3>{ x, y, z };
}

template<typename Scalar>
inline Scalar CosOfVectors(const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1)
{
    return Dot(Normalize(v0), Normalize(v1));
}

template<typename Scalar>
struct Vector<Scalar, 2>
{
    union
    {
        Scalar v[2];
        struct
        {
            Scalar X;
            Scalar Y;
        };
    };

    Vector() = default;
    Vector(const Scalar* v)
    {
        for (size_t i = 0; i < 2; i++)
        {
            this->v[i] = v[i];
        }
    }
    Vector(const std::initializer_list<Scalar>& list)
    {
        auto l = list.begin();

        for (size_t i = 0; i < 2; i++)
        {
            this->v[i] = (list.end() == l) ? 0 : *l++;
        }
    }

    bool IsNaN() const
    {
        for (Scalar v : this->v)
        {
            if (isnan(v))
            {
                return true;
            }
        }

        return false;
    }

    Scalar& operator[](size_t index)
    {
        return this->v[index];
    }
    const Scalar& operator[](size_t index) const
    {
        return this->v[index];
    }
    operator Scalar*()
    {
        return this->v;
    }
    operator const Scalar*() const
    {
        return this->v;
    }

    Scalar Dot() const
    {
        return ::Dot(*this, *this);
    }
    Scalar Dot(const Vector<Scalar, 3>& other) const
    {
        return ::Dot(*this, other);
    }
    Scalar Length() const
    {
        return ::Length(*this);
    }
    Vector<Scalar, 2> Normalize() const
    {
        return ::Normalize(*this);
    }

    static const Vector<Scalar, 2> NaN, Zero, XAxis, YAxis;
};
template<typename Scalar>
const Vector<Scalar, 2> Vector<Scalar, 2>::NaN   = { std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN() };
template<typename Scalar>
const Vector<Scalar, 2> Vector<Scalar, 2>::Zero  = { 0, 0 };
template<typename Scalar>
const Vector<Scalar, 2> Vector<Scalar, 2>::XAxis = { 1, 0 };
template<typename Scalar>
const Vector<Scalar, 2> Vector<Scalar, 2>::YAxis = { 0, 1 };

template<typename Scalar>
struct Vector<Scalar, 3>
{
    union
    {
        Scalar v[3];
        struct
        {
            Scalar X;
            Scalar Y;
            Scalar Z;
        };
    };

    Vector() = default;
    Vector(Scalar v)
    {
        for (size_t i = 0; i < 3; i++)
        {
            this->v[i] = v;
        }
    }
    Vector(const Scalar* v)
    {
        for (size_t i = 0; i < 3; i++)
        {
            this->v[i] = v[i];
        }
    }
    Vector(const std::initializer_list<Scalar>& list)
    {
        auto l = list.begin();

        for (size_t i = 0; i < 3; i++)
        {
            this->v[i] = (list.end() == l) ? 0 : *l++;
        }
    }

    bool IsNaN() const
    {
        for (Scalar v : this->v)
        {
            if (isnan(v))
            {
                return true;
            }
        }

        return false;
    }

    Scalar& operator[](size_t index)
    {
        return this->v[index];
    }
    const Scalar& operator[](size_t index) const
    {
        return this->v[index];
    }
    operator Scalar*()
    {
        return this->v;
    }
    operator const Scalar*() const
    {
        return this->v;
    }

    Scalar Dot() const
    {
        return ::Dot(*this, *this);
    }
    Scalar Dot(const Vector<Scalar, 3>& other) const
    {
        return ::Dot(*this, other);
    }
    Scalar Length() const
    {
        return ::Length(*this);
    }
    Vector<Scalar, 3> Cross(const Vector<Scalar, 3>& other) const
    {
        return ::Cross(*this, other);
    }
    Vector<Scalar, 3> Normalize() const
    {
        return ::Normalize(*this);
    }

    static const Vector<Scalar, 3> NaN, Zero, XAxis, YAxis, ZAxis;
};
template<typename Scalar>
const Vector<Scalar, 3> Vector<Scalar, 3>::NaN   = { std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN() };
template<typename Scalar>
const Vector<Scalar, 3> Vector<Scalar, 3>::Zero  = { 0, 0, 0 };
template<typename Scalar>
const Vector<Scalar, 3> Vector<Scalar, 3>::XAxis = { 1, 0, 0 };
template<typename Scalar>
const Vector<Scalar, 3> Vector<Scalar, 3>::YAxis = { 0, 1, 0 };
template<typename Scalar>
const Vector<Scalar, 3> Vector<Scalar, 3>::ZAxis = { 0, 0, 1 };

template<typename Scalar>
struct Vector<Scalar, 4>
{
    union
    {
        Scalar v[4];
        struct
        {
            Scalar X;
            Scalar Y;
            Scalar Z;
            Scalar W;
        };
    };

    Vector() = default;
    Vector(Scalar v)
    {
        for (size_t i = 0; i < 4; i++)
        {
            this->v[i] = v;
        }
    }
    Vector(const Scalar* v)
    {
        for (size_t i = 0; i < 4; i++)
        {
            this->v[i] = v[i];
        }
    }
    Vector(const std::initializer_list<Scalar>& list)
    {
        auto l = list.begin();

        for (size_t i = 0; i < 4; i++)
        {
            this->v[i] = (list.end() == l) ? 0 : *l++;
        }
    }

    bool IsNaN() const
    {
        for (Scalar v : this->v)
        {
            if (isnan(v))
            {
                return true;
            }
        }

        return false;
    }

    Scalar& operator[](size_t index)
    {
        return this->v[index];
    }
    const Scalar& operator[](size_t index) const
    {
        return this->v[index];
    }
    operator Scalar*()
    {
        return this->v;
    }
    operator const Scalar*() const
    {
        return this->v;
    }

    static const Vector<Scalar, 4> NaN, Zero;
};
template<typename Scalar>
const Vector<Scalar, 4> Vector<Scalar, 4>::NaN   = { std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN() };
template<typename Scalar>
const Vector<Scalar, 4> Vector<Scalar, 4>::Zero  = { 0, 0, 0, 0 };

template<typename Scalar>
struct Quaternion : public Vector<Scalar, 4>
{
    Quaternion() = default;
    Quaternion(const Scalar* v) : Vector<Scalar, 4>(v)
    {
    }
    Quaternion(const std::initializer_list<Scalar>& v) : Vector<Scalar, 4>(v)
    {
    }

    Vector<Scalar, 3> Rotate(const Vector<Scalar, 3>& v) const
    {
        auto uv = ((Vector<Scalar, 3>*)this)->Cross(v);
        uv += uv;
        return uv * this->v[3] + ((Vector<Scalar, 3>*)this)->Cross(uv) + v;
    }

    Quaternion<Scalar> Inverse() const
    {
        return Quaternion<Scalar>{ this->v[0], this->v[1], this->v[2], -this->v[3] };
    }

    Quaternion<Scalar> operator*(const Quaternion& other) const
    {
        auto& t = (Vector<Scalar, 3>&)*this;
        auto& o = (Vector<Scalar, 3>&)other;

        auto w = t[3] * o[3] - t.Dot(o);
        auto p = t.Cross(o) + o * t[3] + t * o[3];

        return Quaternion<Scalar>{ p.v[0], p.v[1], p.v[2], w };
    }

    Vector<Scalar, 3> ToEuler() const
    {
        auto x = std::atan2(2 * (this->v[0] * this->v[3] + this->v[1] * this->v[2]), 1 - 2 * (this->v[0] * this->v[0] + this->v[1] * this->v[1]));
        auto s = 2 * (this->v[1] * this->v[3] - this->v[0] * this->v[2]);
        auto y = std::abs(s) < 1 ? std::asin(s) : std::copysign((Scalar)3.14159265358979323846264338327950288419716939937510l / 2, s);
        auto z = std::atan2(2 * (this->v[2] * this->v[3] + this->v[0] * this->v[1]), 1 - 2 * (this->v[1] * this->v[1] + this->v[2] * this->v[2]));

        return Vector<Scalar, 3>{ x, y, z };
    }

    Vector<Scalar, 4> ToRotation() const
    {
        auto a = acos(this->v[3]);
        auto s = sin(a);

        if (abs(s) < FLT_EPSILON)
        {
            return Vector<Scalar, 4>{ 0, 0, 0, 0 };
        }

        return Vector<Scalar, 4>{ this->v[0] / s, this->v[1] / s, this->v[2] / s, ToDegree(a * 2) };
    }

    static Quaternion<Scalar> FromEuler(const Vector<Scalar, 3>& e)
    {
        auto cx = std::cos(e[0] / 2);
        auto sx = std::sin(e[0] / 2);
        auto cy = std::cos(e[1] / 2);
        auto sy = std::sin(e[1] / 2);
        auto cz = std::cos(e[2] / 2);
        auto sz = std::sin(e[2] / 2);

        return Quaternion<Scalar>{ cz * cy * sx - sz * sy * cx,
                                   sz * cy * sx + cz * sy * cx,
                                   sz * cy * cx - cz * sy * sx,
                                   cz * cy * cx + sz * sy * sx };
    }

    static Quaternion<Scalar> FromRotation(const Vector<Scalar, 4>& rotation)
    {
        auto& axis = (Vector<Scalar, 3>&)rotation;
        auto angle = rotation.v[3];

        if (axis.Dot() < FLT_EPSILON)
        {
            return Identity;
        }

        return FromAxisAngle(axis, ToRadian(angle));
    }

    static Quaternion<Scalar> From2Vectors(const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1)
    {
        auto n0 = v0.Normalize();
        auto n1 = v1.Normalize();

        if (std::fabs(n0[0] - n1[0]) < 0.00001 &&
            std::fabs(n0[1] - n1[1]) < 0.00001 &&
            std::fabs(n0[2] - n1[2]) < 0.00001)
        {
            return Identity;
        }

        auto h = (n0 + n1).Normalize();
        auto q = n0.Cross(h);

        return Quaternion<Scalar>{ q[0], q[1], q[2], n0.Dot(h) };
    }

    static Quaternion<Scalar> FromAxisAngle(const Vector<Scalar, 3>& axis, float radian)
    {
        auto a = radian / 2;
        auto c = cos(a);
        auto s = sin(a);
        auto n = axis.Normalize();

        return Quaternion<Scalar>{ s * n.v[0], s * n.v[1], s * n.v[2], c };
    }

    static const Quaternion<Scalar> Identity;
};
template<typename Scalar>
const Quaternion<Scalar> Quaternion<Scalar>::Identity = { 0, 0, 0, 1 };

typedef Vector<uint32_t, 3> Element;
typedef Vector<float, 3> Vertex;
typedef Vector<float, 3> Normal;
typedef Vector<float, 2> Coordinate;

template<typename Scalar, size_t MRows, size_t MCols = MRows>
struct Matrix
{
    Vector<Scalar, MCols> v[MRows];

    Matrix() = default;
    Matrix(Scalar v)
    {
        for (size_t i = 0; i < MRows; i++)
        {
            this->v[i] = v;
        }
    }
    Matrix(const Scalar* v)
    {
        for (size_t i = 0; i < MRows; i++)
        {
            this->v[i] = v + i * MCols;
        }
    }
    Matrix(const std::initializer_list<Scalar>& list)
    {
        auto l = list.begin();
        for (size_t i = 0; i < MRows; i++)
        {
            auto& v = this->v[i];
            for (size_t j = 0; j < MCols; j++)
            {
                v[j] = (list.end() == l) ? (Scalar)0 : *l++;
            }
        }
    }
    Matrix(const std::initializer_list<const Vector<Scalar, MCols>>& list)
    {
        auto l = list.begin();
        for (size_t i = 0; i < MRows; i++)
        {
            this->v[i] = (list.end() == l) ? Vector<Scalar, MCols>((Scalar)0) : *l++;
        }
    }

    Matrix<Scalar, MCols, MRows> Transpose() const
    {
        Matrix<Scalar, MCols, MRows> m;
        for (size_t i = 0; i < MRows; i++)
        {
            for (size_t j = 0; j < MCols; j++)
            {
                m[j][i] = this->v[i][j];
            }
        }

        return m;
    }

    Vector<Scalar, MCols>& operator[](size_t index)
    {
        return this->v[index];
    }
    const Vector<Scalar, MCols>& operator[](size_t index) const
    {
        return this->v[index];
    }
    operator Scalar*()
    {
        return this->v[0];
    }
    operator const Scalar*() const
    {
        return this->v[0];
    }
};

template<typename Scalar, size_t MRows, size_t MCols, size_t MCR>
inline Matrix<Scalar, MRows, MCols> operator*(const Matrix<Scalar, MRows, MCR>& m0, const Matrix<Scalar, MCR, MCols>& m1)
{
    Matrix<Scalar, MRows, MCols> m;
    for (size_t i = 0; i < MRows; i++)
    {
        for (size_t j = 0; j < MCols; j++)
        {
            auto& s = m[i][j];
            s = 0;

            for (size_t k = 0; k < MCR; k++)
            {
                s += m0[i][k] * m1[k][j];
            }
        }
    }
    return m;
}

template<typename Scalar, size_t MRows, size_t MCols>
inline Vector<Scalar, MCols> operator*(const Matrix<Scalar, MRows, MCols>& m, const Vector<Scalar, MCols>& v)
{
    Vector<Scalar, MCols> p;
    for (size_t i = 0; i < MCols; i++)
    {
        p[i] = Dot(m[i], v);
    }
    return p;
}

template<typename Scalar, size_t MRows, size_t MCols = MRows>
struct CMatrix
{
    Vector<Scalar, MRows> v[MCols];

    CMatrix() = default;
    CMatrix(const Scalar* v)
    {
        for (size_t i = 0; i < MCols; i++)
        {
            this->v[i] = v + i * MRows;
        }
    }
    CMatrix(const std::initializer_list<Scalar>& list)
    {
        auto l = list.begin();
        for (size_t i = 0; i < MCols; i++)
        {
            auto& v = this->v[i];
            for (size_t j = 0; j < MRows; j++)
            {
                v[j] = (list.end() == l) ? (Scalar)0 : *l++;
            }
        }
    }
    CMatrix(const std::initializer_list<const Vector<Scalar, MRows>>& list)
    {
        auto l = list.begin();
        for (size_t i = 0; i < MCols; i++)
        {
            this->v[i] = (list.end() == l) ? Vector<Scalar, MRows>((Scalar)0) : *l++;
        }
    }

    ::Matrix<Scalar, MRows, MCols> ToMatrix() const
    {
        ::Matrix<Scalar, MRows, MCols> m;
        for (size_t i = 0; i < MCols; i++)
        {
            for (size_t j = 0; j < MRows; j++)
            {
                m[j][i] = this->v[i][j];
            }
        }
        return m;
    }

    const ::Matrix<Scalar, MRows, MCols>& RawMatrix() const
    {
        return *(::Matrix<Scalar, MRows, MCols>*)this;
    }

    operator Scalar*()
    {
        return this->v[0];
    }
    operator const Scalar*() const
    {
        return this->v[0];
    }
};

template<typename T, size_t N>
char(&_ArraySizeHelper(T(&array)[N]))[N];
#define COUNTOF(array)(sizeof( _ArraySizeHelper(array)))