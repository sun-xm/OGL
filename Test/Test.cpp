#include <VectorX.h>

int main()
{
    auto v4i = Vector<3, int>::Zero | 1;
    if (Any(v4i != Vector<4, int>{ 0, 0, 0, 1 }))
    {
        return -1;
    }

    v4i = 1 | Vector<3, int>::Zero;
    if (Any(v4i != Vector<4, int>{ 1, 0, 0, 0 }))
    {
        return -1;
    }

    v4i = Vector<2, int>{ 1, 2 } | Vector<2, int>{ 3, 4 };
    if (Any(v4i != Vector<4, int>{ 1, 2, 3, 4 }))
    {
        return -1;
    }

    auto m3i = Matrix<3, 3, int>::Zero;
    m3i.Range(Matrix<2, 2, int>::Identity, 1, 1);
    if (Any(m3i.ToVector() != Vector<9, int>{ 0, 0, 0, 0, 1, 0, 0, 0, 1 }))
    {
        return -1;
    }

    m3i.SetCol(Vector<3, int>{ 1, 1, 1 }, 1);
    if (Any(m3i.ToVector() != Vector<9, int>{ 0, 1, 0, 0, 1, 0, 0, 1, 1 }))
    {
        return -1;
    }

    m3i.SetRow(Vector<3, int>{ 1, 1, 1 }, 1);
    if (Any(m3i.ToVector() != Vector<9, int>{ 0, 1, 0, 1, 1, 1, 0, 1, 1 }))
    {
        return -1;
    }

    return 0;
}