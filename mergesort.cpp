#include <array>
#include <iostream>
#include <iterator>
#include <vector>

template <
    typename CIterator,
    typename T = typename std::iterator_traits<CIterator>::value_type,
    typename Container = std::vector<T>
    >
static Container mergesort(CIterator a, CIterator b) {
    // 1. split
    auto dist = std::distance(a, b);
    if (dist == 1)
        return {a,b};

    // 2. recurse
    auto mid = a + dist / 2;
    auto left = mergesort(a, mid);
    auto right = mergesort(mid, b);

    // 3. merge
    typename Container::size_type li = 0, ri = 0;
    Container out;
    out.reserve(left.size() + right.size());

    while (li < left.size() && ri < right.size()) {
        if (left[li] < right[ri])
            out.emplace_back(std::move(left[li++]));
        else
            out.emplace_back(std::move(right[ri++]));
    }

    while (li < left.size())
        out.emplace_back(std::move(left[li++]));

    while (ri < right.size())
        out.emplace_back(std::move(right[ri++]));

    return out;
}

int main(void) {
    const std::array<char, 9> input = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'};

    for (const auto& element: mergesort(input.begin(), input.end()))
        std::cout << element << '\n';

    return 0;
}
