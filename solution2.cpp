#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <omp.h>

template<typename Iterator>
void parallel_quicksort(Iterator first, Iterator last) {
    if (first == last || std::next(first) == last) {
        return;
    }
    
    auto size = std::distance(first, last);
    if (size < 1000) {
        std::sort(first, last);
        return;
    }
    
    auto pivot = *std::next(first, size / 2);
    auto middle1 = std::partition(first, last, [pivot](const auto& elem) {
        return elem < pivot;
    });
    auto middle2 = std::partition(middle1, last, [pivot](const auto& elem) {
        return !(pivot < elem);
    });
    
    #pragma omp task
    parallel_quicksort(first, middle1);
    #pragma omp task
    parallel_quicksort(middle2, last);
    #pragma omp taskwait
}

std::vector<size_t> read_array() {
    size_t length, a, b, p;
    std::cin >> length >> a >> b >> p;
    std::vector<size_t> result(length);
    result[0] = a % p;
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = (result[i - 1] * a + b) % p;
    }
    
    return result;
}


int main() {
    auto array = read_array();

    #pragma omp parallel
    {
        #pragma omp single
        parallel_quicksort(array.begin(), array.end());
    }

    size_t k;
    std::cin >> k;
    for (size_t i = k - 1; i < array.size(); i += k) {
        std::cout << array[i] << ' ';
    }
    std::cout << "\n";

    return 0;
}

