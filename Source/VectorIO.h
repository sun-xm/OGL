#ifndef _VECTOR_IO_H_
#define _VECTOR_IO_H_

#include <iostream>
#include "VectorX.h"

#ifdef VECTORX_NS
#define VXNS VECTORX_NS
namespace VECTORX_NS {
#else
#define VXNS
#endif

namespace details
{
    struct Bracket
    {
        char bracket;
    };

    struct Indent
    {
        int indent;
    };

    struct SingleLine
    {
        int singleline;
    };

    inline int bracket_index()
    {
        static const int index = std::ios_base::xalloc();
        return index;
    }

    inline int compact_index()
    {
        static const int index = std::ios_base::xalloc();
        return index;
    }

    inline int indent_index()
    {
        static const int index = std::ios_base::xalloc();
        return index;
    }

    inline int singleline_index()
    {
        static const int index = std::ios_base::xalloc();
        return index;
    }

    inline std::ostream& set_singleline(std::ostream& os)
    {
        os.iword(singleline_index()) = 1;
        return os;
    }

    inline std::ostream& set_multiline(std::ostream& os)
    {
        os.iword(singleline_index()) = 0;
        return os;
    }

    inline std::ostream& set_space(std::ostream& os)
    {
        os.iword(compact_index()) = 0;
        return os;
    }

    inline std::ostream& set_nospace(std::ostream& os)
    {
        os.iword(compact_index()) = 1;
        return os;
    }

    inline void get_bracket(std::ostream& os, char& opening, char& closing)
    {
        switch (os.iword(bracket_index()))
        {
            case '[':
            case ']':
            {
                opening = '[';
                closing = ']';
                break;
            }

            case '(':
            case ')':
            {
                opening = '(';
                closing = ')';
                break;
            }

            case '<':
            case '>':
            {
                opening = '<';
                closing = '>';
                break;
            }

            default:
            {
                opening = '{';
                closing = '}';
                break;
            }
        }
    }

    inline int get_indent(std::ostream& os)
    {
        auto indent = os.iword(indent_index());
        return indent >= 1 ? indent - 1 : 4;
    }

    inline char read(std::istream& is)
    {
        char c;
        while ([&]{ is >> c; return c == ' ' || c == '\n' || c == '\r' || c == '\t'; }());
        return is ? c : 0;
    }

    inline char peek(std::istream& is)
    {
        char c;
        while ([&]{ c = (char)is.peek(); return c == ' ' || c == '\n' || c == '\r' || c == '\t'; }())
        {
            is.get();
        }
        return is ? c : 0;
    }
}

std::ostream&(*const singleline)(std::ostream&)  = VXNS::details::set_singleline;
std::ostream&(*const multiline)(std::ostream&) = VXNS::details::set_multiline;
std::ostream&(*const space)(std::ostream&) = VXNS::details::set_space;
std::ostream&(*const nospace)(std::ostream&) = VXNS::details::set_nospace;

inline VXNS::details::Bracket bracket(char bracket)
{
    VXNS::details::Bracket b;
    b.bracket = bracket;
    return b;
}

inline VXNS::details::Indent indent(int indent)
{
    VXNS::details::Indent i;
    i.indent = indent < 1 ? 1 : indent + 1;
    return i;
}

inline std::ostream& operator<<(std::ostream& os, const VXNS::details::Bracket& bracket)
{
    os.iword(VXNS::details::bracket_index()) = bracket.bracket;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const VXNS::details::Indent& indent)
{
    os.iword(VXNS::details::indent_index()) = indent.indent;
    return os;
}

template<size_t Dimensions, typename Scalar>
inline std::ostream& operator<<(std::ostream& os, const Vector<Dimensions, Scalar>& v)
{
    auto compact = os.iword(VXNS::details::compact_index());

    char opening;
    char closing;
    VXNS::details::get_bracket(os, opening, closing);

    os << opening;

    if (Dimensions > 0)
    {
        os << v.s[0];

        for (size_t i = 1; i < Dimensions; i++)
        {
            os << ',' << (compact ? "" : " ") << v.s[i];
        }
    }

    return os << closing;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline std::ostream& operator<<(std::ostream& os, const Matrix<MRows, MCols, Scalar>& m)
{
    char opening;
    char closing;
    VXNS::details::get_bracket(os, opening, closing);

    int compact = os.iword(VXNS::details::compact_index());
    int singleline = os.iword(VXNS::details::singleline_index());

    os << opening;

    if (!singleline)
    {
        os << std::endl;
        for (int i = 0; i < VXNS::details::get_indent(os); i++)
        {
            os << ' ';
        }
    }

    if (MRows > 0)
    {
        os << m[0];

        for (size_t i = 1; i < MRows; i++)
        {
            os << ',';
            if (singleline && !compact)
            {
                os << ' ';
            }

            if (!singleline)
            {
                os << std::endl;
                for (int i = 0; i < VXNS::details::get_indent(os); i++)
                {
                    os << ' ';
                }
            }
            os << m[i];
        }
    }

    if (!singleline)
    {
        os << std::endl;
    }

    return os << closing;
}

template<size_t Dimensions, typename Scalar>
inline std::istream& operator>>(std::istream& is, Vector<Dimensions, Scalar>& v)
{
    char closing = 0;

    switch (VXNS::details::read(is))
    {
        case '{':
        {
            closing = '}';
            break;
        }

        case '[':
        {
            closing = ']';
            break;
        }

        case '(':
        {
            closing = ')';
            break;
        }

        case '<':
        {
            closing = '<';
            break;
        }

        default:
        {
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    if (Dimensions > 0)
    {
        is >> v.s[0];

        for (size_t i = 1; i < Dimensions; i++)
        {
            char c = VXNS::details::read(is);
            if (',' != c)
            {
                is.setstate(std::ios::failbit);
                return is;
            }
            is >> v.s[i];
        }
    }

    if (closing != VXNS::details::read(is))
    {
        is.setstate(std::ios::failbit);
    }

    return is;
}

template<size_t MRows, size_t MCols, typename Scalar>
inline std::istream& operator>>(std::istream& is, Matrix<MRows, MCols, Scalar>& m)
{
    char closing = 0;

    switch (VXNS::details::read(is))
    {
        case '{':
        {
            closing = '}';
            break;
        }

        case '[':
        {
            closing = ']';
            break;
        }

        case '(':
        {
            closing = ')';
            break;
        }

        case '<':
        {
            closing = '<';
            break;
        }

        default:
        {
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    if (MRows > 0)
    {
        is >> m.v[0];

        for (size_t i = 1; i < MRows; i++)
        {
            char c = VXNS::details::read(is);
            if (',' != c)
            {
                is.setstate(std::ios::failbit);
                return is;
            }
            is >> m.v[i];
        }
    }

    if (closing != VXNS::details::read(is))
    {
        is.setstate(std::ios::failbit);
    }

    return is;
}

#ifdef VECTORX_NS
}
#endif
#undef VXNS

#endif // _VECTOR_IO_H_