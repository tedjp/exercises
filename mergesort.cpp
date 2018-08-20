#include <array>
#include <iostream>
#include <iterator>
#include <vector>

// This merge is destructive to the source container;
// after merging, the source will only contain moved-from objects,
// a new container (usually vector) is returned with the objects
// in order.
template <
    typename CIterator,
    typename T = typename std::iterator_traits<CIterator>::value_type,
    typename Container = std::vector<T>
    >
static Container mergesort(CIterator a, CIterator b) {
    auto dist = std::distance(a, b);
    if (dist == 1)
        return {a,b};

    // split/recurse
    auto mid = a + dist / 2;
    auto left = mergesort(a, mid);
    auto right = mergesort(mid, b);

    // merge
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

template <typename It1, typename It2>
bool matches(It1 lbegin, It1 lend, It2 rbegin, It2 rend) {
    while (lbegin != lend && rbegin != rend) {
        if (*lbegin++ != *rbegin++)
            return false;
    }

    return lbegin == lend && rbegin == rend;
}

int main(void) {
    const std::array<char, 9> input = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'};
    const std::array<char, 9> expect = {'a', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 's'};

    auto sorted = mergesort(input.begin(), input.end());

    if (!matches(sorted.begin(), sorted.end(), expect.begin(), expect.end())) {
        std::cerr << "Mismatch\n";

        for (const auto& element: sorted)
            std::cerr << element << '\n';

        return 1;
    }

    return 0;
}
