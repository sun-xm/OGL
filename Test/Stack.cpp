#include <VectorX.h>

int main(int argc, char* argv[])
{
    auto v0 = Vector<2, int>{ 1, 2 };
    auto v1 = Vector<2, int>{ 3, 4 };

    auto m0 = v0 & v1;
    if (2 != Rows(m0) || 2 != Cols(m0))
    {
        return -1;
    }

    if (Any(m0.ToVector() != Vector<4, int>{ v0[0], v0[1], v1[0], v1[1] }))
    {
        return -1;
    }

    v0 = { 5, 7 };
    v1 = { 6, 8 };

    auto m1 = v0 | v1;
    if (2 != Rows(m1) || 2 != Cols(m1))
    {
        return -1;
    }

    if (Any(m1.ToVector() != Vector<4, int>{ v0[0], v1[0], v0[1], v1[1] }))
    {
        return false;
    }

    auto m24 = m0 | m1;
    if (2 != Rows(m24) || 4 != Cols(m24))
    {
        return -1;
    }

    if (Any(m24.ToVector() != Vector<8, int>{ m0[0][0], m0[0][1], m1[0][0], m1[0][1], m0[1][0], m0[1][1], m1[1][0], m1[1][1] }))
    {
        return -1;
    }

    auto m42 = m0 & m1;
    if (4 != Rows(m42) || 2 != Cols(m42))
    {
        return -1;
    }

    if (Any(m42.ToVector() != Vector<8, int>{ m0[0][0], m0[0][1], m0[1][0], m0[1][1], m1[0][0], m1[0][1], m1[1][0], m1[1][1] }))
    {
        return -1;
    }

    auto v = Vector<2, int>{ 9, 0 };

    auto m23 = m0 | v;
    if (2 != Rows(m23) || 3 != Cols(m23))
    {
        return -1;
    }

    if (Any(m23.ToVector() != Vector<6, int>{ m0[0][0], m0[0][1], v[0], m0[1][0], m0[1][1], v[1] }))
    {
        return -1;
    }

    m23 = v | m0;
    if (Any(m23.ToVector() != Vector<6, int>{ v[0], m0[0][0], m0[0][1], v[1], m0[1][0], m0[1][1] }))
    {
        return -1;
    }

    auto m32 = m0 & v;
    if (3 != Rows(m32) || 2 != Cols(m32))
    {
        return -1;
    }

    if (Any(m32.ToVector() != Vector<6, int>{ m0[0][0], m0[0][1], m0[1][0], m0[1][1], v[0], v[1] }))
    {
        return -1;
    }

    m32 = v & m0;
    if (Any(m32.ToVector() != Vector<6, int>{ v[0], v[1], m0[0][0], m0[0][1], m0[1][0], m0[1][1] }))
    {
        return -1;
    }

    return 0;
}