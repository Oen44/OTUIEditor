#ifndef CONST_H
#define CONST_H

#include <iterator>
#include <unordered_map>

namespace OTUI {
    enum Pivot {
        NoPivot = 0,
        TopLeft,
        Top,
        TopRight,
        Left,
        Right,
        BottomLeft,
        Bottom,
        BottomRight
    };

    enum AlignmentFlag {
        AlignNone = 0,
        AlignLeft = 1,
        AlignRight = 2,
        AlignTop = 4,
        AlignBottom = 8,
        AlignHorizontalCenter = 16,
        AlignVerticalCenter = 32,
        AlignTopLeft = AlignTop | AlignLeft, // 5
        AlignTopRight = AlignTop | AlignRight, // 6
        AlignBottomLeft = AlignBottom | AlignLeft, // 9
        AlignBottomRight = AlignBottom | AlignRight, // 10
        AlignLeftCenter = AlignLeft | AlignVerticalCenter, // 33
        AlignRightCenter = AlignRight | AlignVerticalCenter, // 34
        AlignTopCenter = AlignTop | AlignHorizontalCenter, // 20
        AlignBottomCenter = AlignBottom | AlignHorizontalCenter, // 24
        AlignCenter = AlignVerticalCenter | AlignHorizontalCenter // 48
    };

    template <typename T>
    struct reversion_wrapper { T &iterable; };

    template <typename T>
    auto begin (reversion_wrapper<T> w) { return std::rbegin(w.iterable); }

    template <typename T>
    auto end (reversion_wrapper<T> w) { return std::rend(w.iterable); }

    template <typename T>
    reversion_wrapper<T> reverse (T &iterable) { return { iterable }; }
}

#endif // CONST_H
