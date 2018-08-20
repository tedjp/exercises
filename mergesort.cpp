#include <iostream>
#include <vector>

static std::vector<char> mergesort(std::vector<char>::const_iterator a, typename std::vector<char>::const_iterator b) {
    // 1. split
    auto dist = std::distance(a, b);
    if (dist == 1)
        return std::vector<char>(a, b);

    // 2. recurse
    auto mid = a + dist / 2;
    auto left = mergesort(a, mid);
    auto right = mergesort(mid, b);

    // 3. merge
    size_t li = 0;
    size_t ri = 0;
    std::vector<char> out;
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

    std::vector<char> sorted = mergesort(input.begin(), input.end());

    for (const auto& element: sorted)
        std::cout << element << '\n';

    return 0;
}
