#ifndef CONST_H
#define CONST_H

#include <iterator>
#include <unordered_map>

namespace OTUI {
    enum Pivot {
        NoPivot = 0,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    const std::unordered_map<std::string, int> WidgetProperties = {
        {"sword", 0},
        {"club", 1},
        {"axe", 2},
        {"shield", 3},
        {"distance", 4},
        {"wand", 5},
        {"ammunition", 6},
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
