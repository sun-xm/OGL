#include <VectorX.h>

int main(int argc, char* argv[])
{
    auto m0 = Matrix<2, 2, int>{{ 1, 2 }, { 3, 4 }};
    auto m1 = Matrix<2, 2, int>{{ 5, 6 }, { 7, 8 }};

    // Horizontal stack
    auto mh = m0 | m1;
    if (2 != Rows(mh) || 4 != Cols(mh))
    {
        return -1;
    }

    if (Any(mh.ToVector() != Vector<8, int>{ 1, 2, 5, 6, 3, 4, 7, 8 }))
    {
        return -1;
    }

    // Vertical stack
    auto mv = m0 & m1;
    if (4 != Rows(mv) || 2 != Cols(mv))
    {
        return -1;
    }

    if (Any(mv.ToVector() != Vector<8, int>{ 1, 2, 3, 4, 5, 6, 7, 8 }))
    {
        return -1;
    }

    return 0;
}