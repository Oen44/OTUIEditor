#ifndef CONST_H
#define CONST_H

#include <iterator>

namespace OTUI {
    enum Pivot {
        NoPivot = 0,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    template <typename T>
    struct reversion_wrapper { T& iterable; };

    template <typename T>
    auto begin (reversion_wrapper<T> w) { return std::rbegin(w.iterable); }

    template <typename T>
    auto end (reversion_wrapper<T> w) { return std::rend(w.iterable); }

    template <typename T>
    reversion_wrapper<T> reverse (T&& iterable) { return { iterable }; }
}

#endif // CONST_H
