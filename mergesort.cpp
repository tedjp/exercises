#include <iostream>
#include <vector>

template <typename T, typename CIterator = typename std::vector<T>::const_iterator>
static std::vector<T> mergesort(CIterator a, CIterator b) {
    // 1. split
    auto dist = std::distance(a, b);
    if (dist == 1)
        return {a,b};

    // 2. recurse
    auto mid = a + dist / 2;
    auto left = mergesort<T>(a, mid);
    auto right = mergesort<T>(mid, b);

    // 3. merge
    size_t li = 0;
    size_t ri = 0;
    std::vector<T> out;
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
    const std::vector<char> input = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'};

    for (const auto& element: mergesort<char>(input.begin(), input.end()))
        std::cout << element << '\n';

    return 0;
}
