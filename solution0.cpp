#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <omp.h>
#include <cmath>

std::istream& operator>>(std::istream& in, __int128& value) {
    std::string s;
    in >> s;
    value = 0;
    bool negative = false;
    size_t i = 0;
    if (s[0] == '-') {
        negative = true;
        i = 1;
    }
    for (; i < s.size(); ++i) {
        value = value * 10 + (s[i] - '0');
    }
    if (negative) value = -value;
    return in;
}

std::ostream& operator<<(std::ostream& out, __int128 value) {
    if (value == 0) {
        out << '0';
        return out;
    }
    std::string s;
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }
    while (value > 0) {
        s += '0' + static_cast<int>(value % 10);
        value /= 10;
    }
    if (negative) s += '-';
    std::reverse(s.begin(), s.end());
    out << s;
    return out;
}

int main() {
    __int128 n;
    std::cin >> n;
    if (n <= 1) {
        return 0;
    }

    std::vector<__int128> factors;
    __int128 num = n;
    
    while (num % 2 == 0) {
        factors.push_back(2);
        num /= 2;
    }
    
    __int128 sqrt_num = static_cast<__int128>(std::sqrt(static_cast<long double>(num))) + 1;
    
    std::vector<std::vector<__int128>> thread_factors(omp_get_max_threads());
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        std::vector<__int128> local_factors;
        
        #pragma omp for schedule(dynamic, 1000)
        for (__int128 p = 3; p <= sqrt_num; p += 2) {
            if (num % p == 0) {
                local_factors.push_back(p);
            }
        }
        
        thread_factors[thread_id] = std::move(local_factors);
    }
    
    std::vector<__int128> potential_factors;
    for (const auto& tf : thread_factors) {
        potential_factors.insert(potential_factors.end(), tf.begin(), tf.end());
    }
    std::sort(potential_factors.begin(), potential_factors.end());
    potential_factors.erase(std::unique(potential_factors.begin(), potential_factors.end()), potential_factors.end());
    
    for (__int128 p : potential_factors) {
        while (num % p == 0) {
            factors.push_back(p);
            num /= p;
        }
    }
    
    if (num > 1) {
        factors.push_back(num);
    }
    
    std::sort(factors.begin(), factors.end());
    
    for (const auto& factor : factors) {
        std::cout << factor << ' ';
    }
    std::cout << '\n';

    return 0;
}
