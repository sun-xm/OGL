#pragma once

#include <initializer_list>
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <limits>

#define SCALAR_TYPE(v) decltype(ScalarHelper(v))

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

template<typename Vector, size_t Dimensions, typename Scalar>
struct VectorBase
{
    VectorBase() = default;
    explicit VectorBase(Scalar s)
    {
        for (size_t i = 0; i < Dimensions; i++)
        {
            ((Vector*)this)->s[i] = s;
        }
    }
    explicit VectorBase(const Scalar* s)
    {
        for (size_t i = 0; i < Dimensions; i++)
        {
            ((Vector*)this)->s[i] = s[i];
        }
    }
    VectorBase(const std::initializer_list<Scalar>& list)
    {
        auto l = list.begin();
        for (size_t i = 0; i < Dimensions; i++)
        {
            ((Vector*)this)->s[i] = (list.end() == l) ? 0 : *l++;
        }
    }
    template<template<size_t D, typename S> class VType, size_t Dims, typename SType>
    explicit VectorBase(const VType<Dims, SType>& other, const std::initializer_list<Scalar>& list = {})
    {
        for (size_t i = 0; i < (Dims < Dimensions ? Dims : Dimensions); i++)
        {
            ((Vector*)this)->s[i] = (Scalar)other.s[i];
        }
        auto l = list.begin();
        for (size_t i = Dims; i < Dimensions; i++)
        {
            ((Vector*)this)->s[i] = (list.end() == l) ? 0 : *l++;
        }
    }

    bool IsNaN() const
    {
        return Any(*(Vector*)this, [](Scalar s){ return isnan(s); });
    }

    Scalar& operator[](size_t index)
    {
        return ((Vector*)this)->s[index];
    }
    const Scalar& operator[](size_t index) const
    {
        return ((Vector*)this)->s[index];
    }

    operator Scalar*()
    {
        return ((Vector*)this)->s;
    }
    operator const Scalar*() const
    {
        return ((Vector*)this)->s;
    }
};

template<size_t Dimensions, typename Scalar = float>
struct Vector : VectorBase<Vector<Dimensions, Scalar>, Dimensions, Scalar>
{
    Scalar s[Dimensions];

    using Base = VectorBase<Vector, Dimensions, Scalar>;

    Vector() = default;
    explicit Vector(Scalar s) : Base(s) {}
    explicit Vector(const Scalar* s) : Base(s) {}
    Vector(const std::initializer_list<Scalar>& list) : Base(list) {}
    template<template<size_t D, typename S> class VType, size_t Dims, typename SType>
    explicit Vector(const VType<Dims, SType>& other, const std::initializer_list<Scalar>& list) : Base(other, list) {}
};

template<typename Scalar>
struct Vector<2, Scalar> : VectorBase<Vector<2, Scalar>, 2, Scalar>
{
    union
    {
        Scalar s[2];
        struct
        {
            Scalar X;
            Scalar Y;
        };
    };

    using Base = VectorBase<Vector, 2, Scalar>;

    Vector() = default;
    explicit Vector(Scalar v) : Base(v) {}
    explicit Vector(const Scalar* v) : Base(v) {}
    Vector(const std::initializer_list<Scalar>& list) : Base(list) {}
    template<template<size_t D, typename S> class VType, size_t Dims, typename SType>
    explicit Vector(const VType<Dims, SType>& other, const std::initializer_list<Scalar>& list = {}) : Base(other, list) {}

    Scalar Dot() const
    {
        return ::Dot(*this, *this);
    }
    Scalar Dot(const Vector& other) const
    {
        return ::Dot(*this, other);
    }
    Scalar Length() const
    {
        return ::Length(*this);
    }
    Vector Normalize() const
    {
        return ::Normalize(*this);
    }

    static const Vector NaN, Zero, XAxis, YAxis;
};
template<typename Scalar>
const Vector<2, Scalar> Vector<2, Scalar>::NaN   = { std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN() };
template<typename Scalar>
const Vector<2, Scalar> Vector<2, Scalar>::Zero  = { 0, 0 };
template<typename Scalar>
const Vector<2, Scalar> Vector<2, Scalar>::XAxis = { 1, 0 };
template<typename Scalar>
const Vector<2, Scalar> Vector<2, Scalar>::YAxis = { 0, 1 };

template<typename Scalar>
struct Vector<3, Scalar> : VectorBase<Vector<3, Scalar>, 3, Scalar>
{
    union
    {
        Scalar s[3];
        struct
        {
            Scalar X;
            Scalar Y;
            Scalar Z;
        };
    };

    using Base = VectorBase<Vector, 3, Scalar>;

    Vector() = default;
    explicit Vector(Scalar v) : Base(v) {}
    explicit Vector(const Scalar* v) : Base(v) {}
    Vector(const std::initializer_list<Scalar>& list) : Base(list) {}
    template<template<size_t D, typename S> class VType, size_t Dims, typename SType>
    explicit Vector(const VType<Dims, SType>& other, const std::initializer_list<Scalar>& list = {}) : Base(other, list) {}

    Scalar Dot() const
    {
        return ::Dot(*this, *this);
    }
    Scalar Dot(const Vector& other) const
    {
        return ::Dot(*this, other);
    }
    Scalar Length() const
    {
        return ::Length(*this);
    }
    Vector Cross(const Vector& other) const
    {
        return ::Cross(*this, other);
    }
    Vector Normalize() const
    {
        return ::Normalize(*this);
    }

    static const Vector<3, Scalar> NaN, Zero, XAxis, YAxis, ZAxis;
};
template<typename Scalar>
const Vector<3, Scalar> Vector<3, Scalar>::NaN   = { std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN() };
template<typename Scalar>
const Vector<3, Scalar> Vector<3, Scalar>::Zero  = { 0, 0, 0 };
template<typename Scalar>
const Vector<3, Scalar> Vector<3, Scalar>::XAxis = { 1, 0, 0 };
template<typename Scalar>
const Vector<3, Scalar> Vector<3, Scalar>::YAxis = { 0, 1, 0 };
template<typename Scalar>
const Vector<3, Scalar> Vector<3, Scalar>::ZAxis = { 0, 0, 1 };

template<typename Scalar>
struct Vector<4, Scalar> : VectorBase<Vector<4, Scalar>, 4, Scalar>
{
    union
    {
        Scalar s[4];
        struct
        {
            Scalar X;
            Scalar Y;
            Scalar Z;
            Scalar W;
        };
    };

    using Base = VectorBase<Vector, 4, Scalar>;

    Vector() = default;
    explicit Vector(Scalar v) : Base(v) {}
    explicit Vector(const Scalar* v) : Base(v) {}
    Vector(const std::initializer_list<Scalar>& list) : Base(list) {}
    template<template<size_t D, typename S> class VType, size_t Dims, typename SType>
    explicit Vector(const VType<Dims, SType>& other, const std::initializer_list<Scalar>& list = {}) : Base(other, list) {}

    static const Vector NaN, Zero, XYPlane, YZPlane, ZXPlane;
};
template<typename Scalar>
const Vector<4, Scalar> Vector<4, Scalar>::NaN   = { std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN(), std::numeric_limits<Scalar>::quiet_NaN() };
template<typename Scalar>
const Vector<4, Scalar> Vector<4, Scalar>::Zero  = { 0, 0, 0, 0 };
template<typename Scalar>
const Vector<4, Scalar> Vector<4, Scalar>::XYPlane = { 0, 0, 1, 0 };
template<typename Scalar>
const Vector<4, Scalar> Vector<4, Scalar>::YZPlane = { 1, 0, 0, 0 };
template<typename Scalar>
const Vector<4, Scalar> Vector<4, Scalar>::ZXPlane = { 0, 1, 0, 0 };

typedef Vector<3, uint32_t> Element;
typedef Vector<3> Vertex;
typedef Vector<3> Normal;
typedef Vector<4> Color;
typedef Vector<2> Coordinate;

template<size_t Dimensions, typename Scalar>
inline size_t Dims(const Vector<Dimensions, Scalar>&)
{
    return Dimensions;
}

template<size_t Dimensions, typename Scalar>
inline Scalar ScalarHelper(const Vector<Dimensions, Scalar>&)
{
    return *(Scalar*)nullptr;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> operator+(const Vector<Dimensions, Scalar>& first, const Vector<Dimensions, Scalar>& second)
{
    Vector<Dimensions, Scalar> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = first.s[i] + second.s[i];
    }

    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> operator+(const Vector<Dimensions, Scalar>& v, Scalar s)
{
    Vector<Dimensions, Scalar> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = v.s[i] + s;
    }

    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> operator+(Scalar s, const Vector<Dimensions, Scalar>& v)
{
    return v + s;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> operator-(const Vector<Dimensions, Scalar>& first, const Vector<Dimensions, Scalar>& second)
{
    Vector<Dimensions, Scalar> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = first.s[i] - second.s[i];
    }

    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> operator-(const Vector<Dimensions, Scalar>& v, Scalar s)
{
    Vector<Dimensions, Scalar> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = v.s[i] - s;
    }

    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> operator-(Scalar s, const Vector<Dimensions, Scalar>& v)
{
    Vector<Dimensions, Scalar> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = s - v.s[i];
    }

    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> operator*(const Vector<Dimensions, Scalar>& vector, Scalar scale)
{
    Vector<Dimensions, Scalar> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = vector.s[i] * scale;
    }

    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> operator*(Scalar scale, const Vector<Dimensions, Scalar>& vector)
{
    return vector * scale;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> operator/(const Vector<Dimensions, Scalar>& vector, Scalar scale)
{
    Vector<Dimensions, Scalar> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = vector.s[i] / scale;
    }

    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> operator-(const Vector<Dimensions, Scalar>& vector)
{
    Vector<Dimensions, Scalar> result;

    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = -vector.s[i];
    }

    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar>& operator+=(Vector<Dimensions, Scalar>& first, const Vector<Dimensions, Scalar>& second)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        first.s[i] += second.s[i];
    }

    return first;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar>& operator-=(Vector<Dimensions, Scalar>& first, const Vector<Dimensions, Scalar>& second)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        first.s[i] -= second.s[i];
    }

    return first;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar>& operator+=(Vector<Dimensions, Scalar>& first, Scalar second)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        first.s[i] += second;
    }

    return first;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar>& operator-=(Vector<Dimensions, Scalar>& first, Scalar second)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        first.s[i] -= second;
    }

    return first;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar>& operator*=(Vector<Dimensions, Scalar>& vector, Scalar scale)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        vector.s[i] *= scale;
    }

    return vector;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar>& operator/=(Vector<Dimensions, Scalar>& vector, Scalar scale)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        vector.s[i] /= scale;
    }

    return vector;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator==(const Vector<Dimensions, Scalar>& v0, const Vector<Dimensions, Scalar>& v1)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v0.s[i] == v1.s[i]);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator==(const Vector<Dimensions, Scalar>& v, Scalar s)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v.s[i] == s);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator==(Scalar s, const Vector<Dimensions, Scalar>& v)
{
    return v == s;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator!=(const Vector<Dimensions, Scalar>& v0, const Vector<Dimensions, Scalar>& v1)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v0.s[i] != v1.s[i]);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator!=(const Vector<Dimensions, Scalar>& v, Scalar s)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v.s[i] != s);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator!=(Scalar s, const Vector<Dimensions, Scalar>& v)
{
    return v != s;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator>(const Vector<Dimensions, Scalar>& v0, const Vector<Dimensions, Scalar>& v1)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v0.s[i] > v1.s[i]);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator>(const Vector<Dimensions, Scalar>& v, Scalar s)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v.s[i] > s);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator>(Scalar s, const Vector<Dimensions, Scalar>& v)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (s > v.s[i]);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator<(const Vector<Dimensions, Scalar>& v0, const Vector<Dimensions, Scalar>& v1)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v0.s[i] < v1.s[i]);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator<(const Vector<Dimensions, Scalar>& v, Scalar s)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v.s[i] < s);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator<(Scalar s, const Vector<Dimensions, Scalar>& v)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (s < v.s[i]);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator>=(const Vector<Dimensions, Scalar>& v0, const Vector<Dimensions, Scalar>& v1)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v0.s[i] >= v1.s[i]);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator>=(const Vector<Dimensions, Scalar>& v, Scalar s)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v.s[i] >= s);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator>=(Scalar s, const Vector<Dimensions, Scalar>& v)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (s >= v.s[i]);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator<=(const Vector<Dimensions, Scalar>& v0, const Vector<Dimensions, Scalar>& v1)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v0.s[i] <= v1.s[i]);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator<=(const Vector<Dimensions, Scalar>& v, Scalar s)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v.s[i] <= s);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, bool> operator<=(Scalar s, const Vector<Dimensions, Scalar>& v)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (s <= v.s[i]);
    }
    return result;
}

template<size_t Dimensions>
inline Vector<Dimensions, bool> operator&&(const Vector<Dimensions, bool>& v0, const Vector<Dimensions, bool>& v1)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v0.s[i] && v1.s[i]);
    }
    return result;
}

template<size_t Dimensions>
inline Vector<Dimensions, bool> operator||(const Vector<Dimensions, bool>& v0, const Vector<Dimensions, bool>& v1)
{
    Vector<Dimensions, bool> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.s[i] = (v0.s[i] || v1.s[i]);
    }
    return result;
}

template<size_t Dimensions>
inline bool Any(const Vector<Dimensions, bool>& booleans)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        if (booleans[i])
        {
            return true;
        }
    }
    return false;
}

template<size_t Dimensions, typename Scalar, typename Pred>
inline bool Any(const Vector<Dimensions, Scalar>& v, const Pred& p)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        if (p(v[i]))
        {
            return true;
        }
    }
    return false;
}

template<size_t Dimensions>
inline bool All(const Vector<Dimensions, bool>& booleans)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        if (!booleans[i])
        {
            return false;
        }
    }
    return true;
}

template<size_t Dimensions, typename Scalar, typename Pred>
inline bool All(const Vector<Dimensions, Scalar>& v, const Pred& p)
{
    for (size_t i = 0; i < Dimensions; i++)
    {
        if (!p(v[i]))
        {
            return false;
        }
    }
    return true;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> Abs(const Vector<Dimensions, Scalar>& v)
{
    Vector<Dimensions, Scalar> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.v[i] = std::abs(v.v[i]);
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> Min(const Vector<Dimensions, Scalar>& v0, const Vector<Dimensions, Scalar>& v1)
{
    Vector<Dimensions, Scalar> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.v[i] = v0.v[i] < v1.v[i] ? v0.v[i] : v1.v[i];
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> Max(const Vector<Dimensions, Scalar>& v0, const Vector<Dimensions, Scalar>& v1)
{
    Vector<Dimensions, Scalar> result;
    for (size_t i = 0; i < Dimensions; i++)
    {
        result.v[i] = v0.v[i] > v1.v[i] ? v0.v[i] : v1.v[i];
    }
    return result;
}

template<size_t Dimensions, typename Scalar>
inline Scalar Dot(const Vector<Dimensions, Scalar>& v0, const Vector<Dimensions, Scalar>& v1)
{
    Scalar product = 0;
    for (size_t i = 0; i < Dimensions; i++)
    {
        product += v0.s[i] * v1.s[i];
    }
    return product;
}

template<size_t Dimensions, typename Scalar>
inline Scalar Length(const Vector<Dimensions, Scalar>& v)
{
    return sqrtx(Dot(v, v));
}

template<size_t Dimensions, typename Scalar>
inline Vector<Dimensions, Scalar> Normalize(const Vector<Dimensions, Scalar>& v)
{
    return v / Length(v);
}

template<typename Scalar>
inline Vector<3, Scalar> Cross(const Vector<3, Scalar>& v0, const Vector<3, Scalar>& v1)
{
    return Vector<3, Scalar>{ v0.s[1] * v1.s[2] - v0.s[2] * v1.s[1],
                              v0.s[2] * v1.s[0] - v0.s[0] * v1.s[2],
                              v0.s[0] * v1.s[1] - v0.s[1] * v1.s[0] };
}

template<typename Scalar>
inline Vector<4, Scalar> Plane(const Vector<3, Scalar>& position, const Vector<3, Scalar>& normal)
{
    return { normal.X, normal.Y, normal.Z, -normal.X * position.X - normal.Y * position.Y - normal.Z * position.Z };
}

template<typename Scalar>
inline Vector<3, Scalar> Project(const Vector<3, Scalar>& position, const Vector<4, Scalar>& plane)
{
    return Intersect(position, (Vector<3, Scalar>&)plane, plane);
}

template<typename Scalar>
inline Vector<3, Scalar> Intersect(const Vector<3, Scalar>& position, const Vector<3, Scalar>& vector, const Vector<4, Scalar>& plane)
{
    auto t = -Distance(position, plane) / Dot(vector, (Vector<3, Scalar>&)plane);
    return position + t * vector;
}

template<typename Scalar>
inline Scalar Distance(const Vector<3, Scalar>& position, const Vector<4, Scalar>& plane)
{
    return position.X * plane.X + position.Y * plane.Y + position.Z * plane.Z + plane.W;
}

template<typename Scalar>
inline Scalar CosOfVectors(const Vector<3, Scalar>& v0, const Vector<3, Scalar>& v1)
{
    return Dot(Normalize(v0), Normalize(v1));
}

template<typename Scalar>
inline Scalar operator*(const Vector<2, Scalar>& v0, const Vector<2, Scalar>& v1)
{
    return Dot(v0, v1);
}

template<typename Scalar>
inline Scalar operator*(const Vector<3, Scalar>& v0, const Vector<3, Scalar>& v1)
{
    return Dot(v0, v1);
}

template<typename Scalar>
inline Vector<3, Scalar> operator^(const Vector<3, Scalar>& v0, const Vector<3, Scalar>& v1)
{
    return Cross(v0, v1);
}

template<typename Scalar>
inline Vector<3, Scalar>& operator^=(Vector<3, Scalar>& v0, const Vector<3, Scalar>& v1)
{
    v0 = Cross(v0, v1);
    return v0;
}

template<typename MTrans, size_t MRows, size_t MCols, typename Scalar>
struct MatrixBase
{
    Vector<MCols, Scalar> v[MRows];

    MatrixBase() = default;
    explicit MatrixBase(Scalar v)
    {
        for (size_t i = 0; i < MRows; i++)
        {
            this->v[i] = Vector<MCols, Scalar>(v);
        }
    }
    explicit MatrixBase(const Scalar* v)
    {
        for (size_t i = 0; i < MRows; i++)
        {
            this->v[i] = Vector<MCols, Scalar>(v + i * MCols);
        }
    }
    MatrixBase(const std::initializer_list<Scalar>& list)
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
    MatrixBase(const std::initializer_list<const Vector<MCols, Scalar>>& list)
    {
        static const Vector<MCols, Scalar> Zero = {0};
        auto l = list.begin();
        for (size_t i = 0; i < MRows; i++)
        {
            this->v[i] = (list.end() == l) ? Zero : *l++;
        }
    }
    template<template<size_t R, size_t C, typename S> class MType, typename SType>
    explicit MatrixBase(const MType<MRows, MCols, SType>& other)
    {
        for (size_t i = 0; i < MRows; i++)
        {
            auto& t = this->v[i];
            auto& o = other.v[i];
            for (size_t j = 0; j < MCols; j++)
            {
                t[j] = (Scalar)o[j];
            }
        }
    }

    bool IsNaN() const
    {
        for (size_t i = 0; i < MRows; i++)
        {
            if (this->v[i].IsNaN())
            {
                return true;
            }
        }
        return false;
    }

    MTrans Transpose() const
    {
        MTrans m;
        for (size_t i = 0; i < MRows; i++)
        {
            for (size_t j = 0; j < MCols; j++)
            {
                m[j][i] = this->v[i][j];
            }
        }
        return m;
    }

    const Vector<MRows * MCols, Scalar>& ToVector() const
    {
        return *(Vector<MRows * MCols, Scalar>*)this->v[0].v;
    }

    Vector<MCols, Scalar>& operator[](size_t index)
    {
        return this->v[index];
    }
    const Vector<MCols, Scalar>& operator[](size_t index) const
    {
        return this->v[index];
    }

    operator Scalar*()
    {
        return (Scalar*)this->v[0];
    }
    operator const Scalar*() const
    {
        return (const Scalar*)this->v[0];
    }
};

template<size_t MRows, size_t MCols = MRows, typename Scalar = float>
struct Matrix : MatrixBase<Matrix<MCols, MRows, Scalar>, MRows, MCols, Scalar>
{
    using Base = MatrixBase<Matrix<MCols, MRows, Scalar>, MRows, MCols, Scalar>;
    Matrix() = default;
    explicit Matrix(Scalar s) : Base(s) {}
    explicit Matrix(const Scalar* s) : Base(s) {}
    Matrix(const std::initializer_list<Scalar>& list) : Base(list) {}
    Matrix(const std::initializer_list<const Vector<MCols, Scalar>>& list) : Base(list) {}
    template<template<size_t R, size_t C, typename S> class MType, typename SType>
    explicit Matrix(const MType<MRows, MCols, SType>& other) : Base(other) {}
};

template<size_t Dims, typename Scalar>
struct Matrix<Dims, Dims, Scalar> : MatrixBase<Matrix<Dims, Dims, Scalar>, Dims, Dims, Scalar>
{
    using Base = MatrixBase<Matrix, Dims, Dims, Scalar>;
    Matrix() = default;
    explicit Matrix(Scalar v) : Base(v) {}
    explicit Matrix(const Scalar* v) : Base(v) {}
    Matrix(const std::initializer_list<Scalar>& list) : Base(list) {}
    Matrix(const std::initializer_list<const Vector<Dims, Scalar>>& list) : Base(list) {}
    template<template<size_t R, size_t C, typename S> class MType, typename SType>
    Matrix(const Matrix<Dims, Dims, SType>& other) : Base(other) {}

    Matrix Inverse() const
    {
        Matrix l, u;
        Vector<Dims, size_t> v;
        DecomposePLU(*this, l, u, v);

        Matrix p;
        for (size_t i = 0; i < Dims; i++)
        {
            p[i] = Matrix::Identity[v[i]];
        }

        auto iu = UpperInverse(u);
        auto il = LowerInverse(l);

        return iu * il * p;
    }

    static const Matrix<Dims, Dims, Scalar> Identity;
};

template<size_t Dims, typename Scalar>
inline Matrix<Dims, Dims, Scalar> MtxIdentity()
{
    Matrix<Dims, Dims, Scalar> id;
    for (size_t i = 0; i < Dims; i++)
    {
        for (size_t j = 0; j < Dims; j++)
        {
            id[i][j] = (Scalar)(i == j ? 1 : 0);
        }
    }
    return id;
}

template<size_t Dims, typename Scalar>
const Matrix<Dims, Dims, Scalar> Matrix<Dims, Dims, Scalar>::Identity = MtxIdentity<Dims, Scalar>();

template<size_t MRows, size_t MCols, typename Scalar>
inline size_t Rows(const Matrix<MRows, MCols, Scalar>&)
{
    return MRows;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline size_t Cols(const Matrix<MRows, MCols, Scalar>&)
{
    return MCols;
}

template<size_t MRows, typename Scalar>
inline void DecomposePLU(const Matrix<MRows, MRows, Scalar>& m, Matrix<MRows, MRows, Scalar>& l, Matrix<MRows, MRows, Scalar>& u, Vector<MRows, size_t>& p)
{
    for (size_t i = 0; i < MRows; i++) p[i] = i;

    Matrix<MRows, MRows, Scalar> L, U;
    U = m;

    for (size_t i = 0; i < MRows - 1; i++)
    {
        auto row = i;
        auto max = std::abs(U[p[i]][i]);

        for (size_t j = i + 1; j < MRows; j++)
        {
            auto uji = std::abs(U[p[j]][i]);
            if (uji > max)
            {
                max = uji;
                row = j;
            }
        }

        if (row != i)
        {
            auto x = p[i];
            p[i] = p[row];
            p[row] = x;
        }

        auto& ui = U[p[i]];
        for (size_t j = i + 1; j < MRows; j++)
        {
            auto& uj = U[p[j]];

            auto r = uj[i] / ui[i];
            L[p[j]][i] = r;

            for (size_t k = i + 1; k < MRows; k++)
            {
                uj[k] -= r * ui[k];
            }
            uj[i] = 0;
        }
    }

    for (size_t i = 0; i < MRows; i++)
    {
        u[i] = U[p[i]];

        auto& Li = L[p[i]];
        auto& li = l[i];
        for (size_t j = 0; j < MRows; j++)
        {
            li[j] = (j < i ? Li[j] : (j == i ? 1 : 0));
        }
    }
}

template<size_t MRows, typename Scalar>
inline Matrix<MRows, MRows, Scalar> UpperInverse(const Matrix<MRows, MRows, Scalar>& u)
{
    Matrix<MRows, MRows, Scalar> inv((Scalar)0);

    for (int n = (int)(MRows - 1); n >= 0; n--)
    {
        inv[n][n] = 1 / u[n][n];

        for (int i = n - 1; i >= 0; i--)
        {
            Scalar sum = 0;
            for (int j = i + 1; j <= n; j++)
            {
                sum += u[i][j] * inv[j][n];
            }

            inv[i][n] = -sum / u[i][i];
        }
    }

    return inv;
}

template<size_t MRows, typename Scalar>
inline Matrix<MRows, MRows, Scalar> LowerInverse(const Matrix<MRows, MRows, Scalar>& l)
{
    Matrix<MRows, MRows, Scalar> inv((Scalar)0);

    for (int n = 0; n < (int)MRows; n++)
    {
        inv[n][n] = 1 / l[n][n];

        for (int i = n + 1; i < MRows; i++)
        {
            Scalar sum = 0;
            for (int j = n; j < i; j++)
            {
                sum += l[i][j] * inv[j][n];
            }

            inv[i][n] = -sum / l[i][i];
        }
    }

    return inv;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline Matrix<MRows, MCols, Scalar> operator*(const Matrix<MRows, MCols, Scalar>& m, Scalar s)
{
    Matrix<MRows, MCols, Scalar> result;

    for (size_t i = 0; i < MRows; i++)
    {
        for (size_t j = 0; j < MCols; j++)
        {
            result[i][j] = m[i][j] * s;
        }
    }

    return result;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline Matrix<MRows, MCols, Scalar> operator*(Scalar s, const Matrix<MRows, MCols, Scalar>& m)
{
    return m * s;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline Matrix<MRows, MCols, Scalar> operator/(const Matrix<MRows, MCols, Scalar>& m, Scalar s)
{
    return m * (1 / s);
}

template<size_t MRows, size_t MCols, typename Scalar>
inline Matrix<MRows, MCols, Scalar> operator+(const Matrix<MRows, MCols, Scalar>& m0, const Matrix<MRows, MCols, Scalar>& m1)
{
    Matrix<MRows, MCols, Scalar> m;
    for (size_t i = 0; i < MRows; i++)
    {
        for (size_t j = 0; j < MCols; j++)
        {
            m[i][j] = m0[i][j] + m1[i][j];
        }
    }
    return m;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline Matrix<MRows, MCols, Scalar> operator-(const Matrix<MRows, MCols, Scalar>& m0, const Matrix<MRows, MCols, Scalar>& m1)
{
    Matrix<MRows, MCols, Scalar> m;
    for (size_t i = 0; i < MRows; i++)
    {
        for (size_t j = 0; j < MCols; j++)
        {
            m[i][j] = m0[i][j] - m1[i][j];
        }
    }
    return m;
}

template<size_t MRows, size_t MCols, size_t MCR, typename Scalar>
inline Matrix<MRows, MCols, Scalar> operator*(const Matrix<MRows, MCR, Scalar>& m0, const Matrix<MCR, MCols, Scalar>& m1)
{
    Matrix<MRows, MCols, Scalar> m;
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

template<size_t MRows, size_t MCols, typename Scalar>
inline Vector<MCols, Scalar> operator*(const Matrix<MRows, MCols, Scalar>& m, const Vector<MCols, Scalar>& v)
{
    Vector<MCols, Scalar> p;
    for (size_t i = 0; i < MCols; i++)
    {
        p[i] = Dot(m[i], v);
    }
    return p;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline Matrix<MRows, MCols, Scalar>& operator+=(Matrix<MRows, MCols, Scalar>& m0, const Matrix<MRows, MCols, Scalar>& m1)
{
    for (size_t i = 0; i < MRows; i++)
    {
        for (size_t j = 0; j < MCols; j++)
        {
            m0[i][j] += m1[i][j];
        }
    }
    return m0;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline Matrix<MRows, MCols, Scalar>& operator-=(Matrix<MRows, MCols, Scalar>& m0, const Matrix<MRows, MCols, Scalar>& m1)
{
    for (size_t i = 0; i < MRows; i++)
    {
        for (size_t j = 0; j < MCols; j++)
        {
            m0[i][j] -= m1[i][j];
        }
    }
    return m0;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline Matrix<MRows, MCols, Scalar>& operator*=(Matrix<MRows, MCols, Scalar>& m, Scalar s)
{
    for (size_t i = 0; i < MRows; i++)
    {
        for (size_t j = 0; j < MCols; j++)
        {
            m[i][j] *= s;
        }
    }
    return m;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline Matrix<MRows, MCols, Scalar>& operator/=(Matrix<MRows, MCols, Scalar>& m, Scalar s)
{
    auto r = 1 / s;
    return operator*=(m, r);
}

template<size_t MRows, size_t MCols, typename Scalar>
inline bool operator==(const Matrix<MRows, MCols, Scalar>& m0, const Matrix<MRows, MCols, Scalar>& m1)
{
    for (size_t i = 0; i < MRows; i++)
    {
        if (m0.v[i] != m1.v[i])
        {
            return false;
        }
    }
    return true;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline bool operator!=(const Matrix<MRows, MCols, Scalar>& m0, const Matrix<MRows, MCols, Scalar>& m1)
{
    return !(m0 == m1);
}

template<typename Scalar = float>
struct Quaternion : public Vector<4, Scalar>
{
    Quaternion() = default;
    Quaternion(const Scalar* v) : Vector<4, Scalar>(v)
    {
    }
    Quaternion(const std::initializer_list<Scalar>& v) : Vector<4, Scalar>(v)
    {
    }

    Vector<3, Scalar> Rotate(const Vector<3, Scalar>& v) const
    {
        auto uv = ((Vector<3, Scalar>*)this)->Cross(v);
        uv += uv;
        return uv * this->s[3] + ((Vector<3, Scalar>*)this)->Cross(uv) + v;
    }

    Quaternion<Scalar> Inverse() const
    {
        return Quaternion<Scalar>{ this->s[0], this->s[1], this->s[2], -this->s[3] };
    }

    Quaternion<Scalar> operator*(const Quaternion& other) const
    {
        auto& t = (Vector<3, Scalar>&)*this;
        auto& o = (Vector<3, Scalar>&)other;

        auto w = t[3] * o[3] - t.Dot(o);
        auto p = t.Cross(o) + o * t[3] + t * o[3];

        return Quaternion<Scalar>{ p.s[0], p.s[1], p.s[2], w };
    }

    Vector<3, Scalar> ToEuler() const
    {
        auto x = std::atan2(2 * (this->v[0] * this->v[3] + this->v[1] * this->v[2]), 1 - 2 * (this->v[0] * this->v[0] + this->v[1] * this->v[1]));
        auto s = 2 * (this->v[1] * this->v[3] - this->v[0] * this->v[2]);
        auto y = std::abs(s) < 1 ? std::asin(s) : std::copysign((Scalar)3.14159265358979323846264338327950288419716939937510l / 2, s);
        auto z = std::atan2(2 * (this->v[2] * this->v[3] + this->v[0] * this->v[1]), 1 - 2 * (this->v[1] * this->v[1] + this->v[2] * this->v[2]));

        return Vector<3, Scalar>{ x, y, z };
    }

    Vector<4, Scalar> ToRotation() const
    {
        auto a = std::acos(this->s[3]);
        auto s = std::sin(a);

        if (std::abs(s) < FLT_EPSILON)
        {
            return Vector<4, Scalar>{ 1, 0, 0, 0 };
        }

        return Vector<4, Scalar>{ this->s[0] / s, this->s[1] / s, this->s[2] / s, ToDegree(a * 2) };
    }

    Matrix<4, 4, Scalar> ToMatrix() const
    {
        auto xx2 = this->X * this->X * 2;
        auto yy2 = this->Y * this->Y * 2;
        auto zz2 = this->Z * this->Z * 2;
        auto xy2 = this->X * this->Y * 2;
        auto xz2 = this->X * this->Z * 2;
        auto yz2 = this->Y * this->Z * 2;
        auto xw2 = this->X * this->W * 2;
        auto yw2 = this->Y * this->W * 2;
        auto zw2 = this->Z * this->W * 2;

        return Matrix<4, 4, Scalar>{{ 1 - yy2 - zz2,     xy2 - zw2,     xz2 + yw2, 0 },
                                    {     xy2 + zw2, 1 - xx2 - zz2,     yz2 - xw2, 0 },
                                    {     xz2 - yw2,     yz2 + xw2, 1 - xx2 - yy2, 0 },
                                    {             0,             0,             0, 1 }};
    }

    static Quaternion<Scalar> FromEuler(const Vector<3, Scalar>& e)
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

    static Quaternion<Scalar> FromRotation(const Vector<4, Scalar>& rotation)
    {
        auto& axis = (Vector<3, Scalar>&)rotation;
        auto angle = rotation.s[3];

        if (axis.Dot() < FLT_EPSILON)
        {
            return Identity;
        }

        return FromAxisAngle(axis, ToRadian(angle));
    }

    static Quaternion<Scalar> From2Vectors(const Vector<3, Scalar>& v0, const Vector<3, Scalar>& v1)
    {
        const auto e = std::numeric_limits<Scalar>::epsilon();

        auto n0 = v0.Normalize();
        auto n1 = v1.Normalize();

        if ((n0 - n1).Length() < e)
        {
            return Identity;
        }
        else if ((n0 + n1).Length() < e)
        {
            auto cx = std::abs(Dot(n0, Vector<3, Scalar>::XAxis));
            auto cy = std::abs(Dot(n0, Vector<3, Scalar>::YAxis));
            auto cz = std::abs(Dot(n0, Vector<3, Scalar>::ZAxis));

            auto axis = Vector<3, Scalar>::XAxis;
            auto minc = cx;

            if (cy < minc)
            {
                minc = cy;
                axis = Vector<3, Scalar>::YAxis;
            }
            if (cz < minc)
            {
                axis = Vector<3, Scalar>::ZAxis;
            }

            auto v = n0.Cross(axis);
            return Quaternion<Scalar>{ v[0], v[1], v[2], 0 };
        }

        auto h = (n0 + n1).Normalize();
        auto q = n0.Cross(h);

        return Quaternion<Scalar>{ q[0], q[1], q[2], n0.Dot(h) };
    }

    static Quaternion<Scalar> FromAxisAngle(const Vector<3, Scalar>& axis, float radian)
    {
        auto a = radian / 2;
        auto c = std::cos(a);
        auto s = std::sin(a);
        auto n = axis.Normalize();

        return Quaternion<Scalar>{ s * n.s[0], s * n.s[1], s * n.s[2], c };
    }

    static const Quaternion<Scalar> Identity;
};

template<typename Scalar>
const Quaternion<Scalar> Quaternion<Scalar>::Identity = { 0, 0, 0, 1 };

template<typename Scalar = float>
class Transform2D
{
public:
    static const Matrix<3, 3, Scalar>& Identity()
    {
        return Matrix<3, 3, Scalar>::Identity;
    }

    static Matrix<3, 3, Scalar> Shift(Scalar x, Scalar y)
    {
        return Matrix<3, 3, Scalar>{{ 1, 0, x },
                                    { 0, 1, y },
                                    { 0, 0, 1 }};
    }

    static Matrix<3, 3, Scalar> Scale(Scalar x, Scalar y)
    {
        return Matrix<3, 3, Scalar>{{ x, 0, 0 },
                                    { 0, y, 0 },
                                    { 0, 0, 1 }};
    }

    static Matrix<3, 3, Scalar> Rotate(Scalar x, Scalar y, Scalar radian)
    {
        auto s = std::sin(radian);
        auto c = std::cos(radian);
        auto m = Matrix<3, 3, Scalar>{{ c, -s, 0 },
                                      { s,  c, 0 },
                                      { 0,  0, 1 }};
        return Shift(x, y) * m * Shift(-x, -y);
    }
};

template<typename Scalar = float>
class Transform3D
{
public:
    static const Matrix<4, 4, Scalar>& Identity()
    {
        return Matrix<4, 4, Scalar>::Identity;
    }

    static Matrix<4, 4, Scalar> Shift(Scalar x, Scalar y, Scalar z)
    {
        return Matrix<4, 4, Scalar>{{ 1, 0, 0, x },
                                    { 0, 1, 0, y },
                                    { 0, 0, 1, z },
                                    { 0, 0, 0, 1 }};
    }

    static Matrix<4, 4, Scalar> Shift(const Vector<3, Scalar>& v)
    {
        return Shift(v[0], v[1], v[2]);
    }

    static Matrix<4, 4, Scalar> Scale(Scalar x, Scalar y, Scalar z)
    {
        return Matrix<4, 4, Scalar>{{ x, 0, 0, 0},
                                    { 0, y, 0, 0},
                                    { 0, 0, z, 0},
                                    { 0, 0, 0, 1}};
    }

    static Matrix<4, 4, Scalar> Scale(const Vector<3, Scalar>& v)
    {
        return Scale(v[0], v[1], v[2]);
    }

    static Matrix<4, 4, Scalar> From2Vectors(const Vector<3, Scalar>& v0, const Vector<3, Scalar>& v1)
    {
        auto n0 = v0.Normalize();
        auto n1 = v1.Normalize();
        auto n = (n0 ^ n1).Normalize();
        auto c = n0 * n1;
        auto s = sqrtx(1 - c * c);

        auto x2 = n.X * n.X;
        auto y2 = n.Y * n.Y;
        auto z2 = n.Z * n.Z;
        auto xy = n.X * n.Y;
        auto yz = n.Y * n.Z;
        auto zx = n.Z * n.X;
        auto cc = 1 - c;

        return Matrix<4, 4, Scalar>
        {
            {       c + x2 * cc,  xy * cc - n.Z * s,  zx * cc + n.Y * s,  0 },
            { xy * cc + n.Z * s,        c + y2 * cc,  yz * cc - n.X * s,  0 },
            { zx * cc - n.Y * s,  yz * cc + n.X * s,        c + z2 * cc,  0 },
            {                 0,                  0,                  0,  1 }
        };
    }

    static Matrix<4, 4, Scalar> FromAxisAngle(const Vector<3, Scalar>& axis, Scalar radian)
    {
        auto c = std::cos(radian);
        auto s = std::sin(radian);
        auto n = axis.Normalize();

        auto cc = 1 - c;
        auto x2 = n.X * n.X;
        auto y2 = n.Y * n.Y;
        auto z2 = n.Z * n.Z;
        auto xy = n.X * n.Y;
        auto yz = n.Y * n.Z;
        auto zx = n.Z * n.X;

        return Matrix<4, 4, Scalar>
        {
            {       c + x2 * cc,  xy * cc - n.Z * s,  zx * cc + n.Y * s,  0 },
            { xy * cc + n.Z * s,        c + y2 * cc,  yz * cc - n.X * s,  0 },
            { zx * cc - n.Y * s,  yz * cc + n.X * s,        c + z2 * cc,  0 },
            {                 0,                  0,                  0,  1 }
        };
    }

    static Matrix<4, 4, Scalar> Perspective(Scalar vfov /*in radian*/, Scalar aspect, Scalar _near, Scalar _far)
    {
        auto f = 1 / tan(vfov / 2);
        auto i = 1 / (_near - _far);
        return Matrix<4, 4, Scalar>{{ f / aspect,  0,                  0,                    0 },
                                    {          0,  f,                  0,                    0 },
                                    {          0,  0, (_near + _far) * i, _near * _far * i * 2 },
                                    {          0,  0,                 -1,                    0 }};
    }

    static Matrix<4, 4, Scalar> LookAt(const Vector<3, Scalar>& eye, const Vector<3, Scalar>& center, Scalar rotation /*in radian*/)
    {
        auto s = Shift(Vector<3, Scalar>::Zero - eye);
        auto q = Quaternion<Scalar>::From2Vectors(center - eye, -Vector<3, Scalar>::ZAxis);
        auto r = Quaternion<Scalar>::FromAxisAngle(q.Rotate(Vector<3, Scalar>::ZAxis), rotation);
        return r.ToMatrix() * q.ToMatrix() * s;
    }

    static void GetTranslation(const Matrix<4, 4, Scalar>& matrix, Vector<3, Scalar>& translation)
    {
        translation = { matrix[0][3], matrix[1][3], matrix[2][3] };
    }

    static void GetRotation(const Matrix<4, 4, Scalar>& matrix, Vector<3, Scalar>& axis, Scalar& radian)
    {
        auto trace = matrix[0][0] + matrix[1][1] + matrix[2][2];
        radian = std::acos((trace - 1) / 2);

        auto factor = 1 / (2 * sqrtx(1 + trace));
        axis = { factor * (matrix[2][1] - matrix[1][2]),
                 factor * (matrix[0][2] - matrix[2][0]),
                 factor * (matrix[1][0] - matrix[0][1]) };
    }
};

template<typename T, size_t N>
char(&_ArraySizeHelper(T(&array)[N]))[N];
#define COUNTOF(array)(sizeof( _ArraySizeHelper(array)))