// A5: kunci int vs kunci string pada unordered_map kecil
#include <chrono>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>
using Clock = std::chrono::steady_clock;

template <typename K, typename Make>
double nsPerFind(int n, Make make) {
    std::unordered_map<K, int> m;
    std::vector<K> keys;
    for (int i = 0; i < n; ++i) { keys.push_back(make(i)); m.emplace(keys.back(), i); }
    constexpr int kLook = 2000000;
    long long acc = 0;
    for (int k = 0; k < 200000; ++k) acc += m.find(keys[(k * 7919u) % n])->second;  // warm-up
    const auto t0 = Clock::now();
    for (int k = 0; k < kLook; ++k) acc += m.find(keys[(k * 7919u) % n])->second;
    const auto t1 = Clock::now();
    const volatile long long sink = acc; (void)sink;
    return std::chrono::duration<double, std::nano>(t1 - t0).count() / kLook;
}

int main() {
    std::printf("n,int_key_ns,string_key_ns\n");
    for (const int n : {1, 2, 4, 8, 12, 16, 20, 24, 32, 64}) {
        const double a = nsPerFind<int>(n, [](int i) { return i; });
        const double b = nsPerFind<std::string>(n, [](int i) {
            char buf[32]; std::snprintf(buf, 32, "LP-%06d", i); return std::string(buf); });
        std::printf("%d,%.1f,%.1f\n", n, a, b);
    }
}