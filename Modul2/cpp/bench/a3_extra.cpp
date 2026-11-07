// A3: dua eksperimen tambahan, SETELAH PREDICTION.md di-commit.
//   Bagian 1: titik persilangan (n kecil), linear scan vs std::unordered_map vs HashMap
//   Bagian 2: HashMap tumbuh dari 8 bucket vs dibuat langsung 2n bucket, n = 10^6
#include "pinjamlab/structures/hash_map.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

using namespace pinjamlab::structures;
using Clock = std::chrono::steady_clock;

template <typename Fn>
double medianMicros(Fn&& op, int repeats = 7) {
    std::vector<double> s;
    op();                                              // warm-up, dibuang
    for (int r = 0; r < repeats; ++r) {
        const auto t0 = Clock::now();
        const volatile auto sink = op();
        (void)sink;
        const auto t1 = Clock::now();
        s.push_back(std::chrono::duration<double, std::micro>(t1 - t0).count());
    }
    std::sort(s.begin(), s.end());
    return s[s.size() / 2];
}

static std::string tagFor(int i) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "LP-%06d", i);
    return buf;
}

static std::vector<std::string> catalogTags(const char* path) {
    std::vector<std::string> out;
    std::ifstream in(path);
    std::string line;
    std::getline(in, line);
    while (std::getline(in, line)) {
        const std::string t = line.substr(0, line.find(','));
        if (!t.empty() && std::find(out.begin(), out.end(), t) == out.end()) out.push_back(t);
    }
    return out;
}

constexpr int kLookups = 200000;     // total pencarian per batch, sama untuk ketiganya

static void crossover(const char* label, const std::vector<std::string>& flat) {
    const std::size_t n = flat.size();
    std::unordered_map<std::string, int> stdmap;
    stdmap.reserve(n);
    HashMap<std::string, int> mine(n * 2);
    for (std::size_t i = 0; i < n; ++i) {
        stdmap.emplace(flat[i], static_cast<int>(i));
        mine.insert(flat[i], static_cast<int>(i));
    }
    std::vector<std::string> probes;
    probes.reserve(kLookups);
    for (int k = 0; k < kLookups; ++k)
        probes.push_back(flat[(static_cast<std::size_t>(k) * 7919) % n]);

    const double lin = medianMicros([&] {
        long long acc = 0;
        for (const auto& p : probes)
            for (std::size_t i = 0; i < flat.size(); ++i)
                if (flat[i] == p) { acc += static_cast<long long>(i); break; }
        return acc;
    });
    const double sh = medianMicros([&] {
        long long acc = 0;
        for (const auto& p : probes) {
            const auto it = stdmap.find(p);
            if (it != stdmap.end()) acc += it->second;
        }
        return acc;
    });
    const double mh = medianMicros([&] {
        long long acc = 0;
        for (const auto& p : probes)
            if (const auto v = mine.find(p)) acc += *v;
        return acc;
    });
    const double k = 1000.0 / kLookups;
    std::printf("crossover,%s,%zu,%.1f,%.1f,%.1f\n", label, n, lin * k, sh * k, mh * k);
}

template <typename Make>
static double buildMillis(Make&& make) {
    std::vector<double> s;
    { auto warm = make(); (void)warm.size(); }
    for (int r = 0; r < 7; ++r) {
        const auto t0 = Clock::now();
        auto c = make();
        const auto t1 = Clock::now();
        const volatile auto sink = c.size();
        (void)sink;
        s.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
    }                                                  // destruktor c jalan SETELAH t1
    std::sort(s.begin(), s.end());
    return s[s.size() / 2];
}

static void growth(int n) {
    std::vector<std::string> flat;
    flat.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) flat.push_back(tagFor(i));
    const std::size_t un = static_cast<std::size_t>(n);

    const double buildPre = buildMillis([&] {
        HashMap<std::string, int> m(un * 2);
        for (int i = 0; i < n; ++i) m.insert(flat[static_cast<std::size_t>(i)], i);
        return m;
    });
    const double buildGrown = buildMillis([&] {
        HashMap<std::string, int> m;                   // 8 bucket awal
        for (int i = 0; i < n; ++i) m.insert(flat[static_cast<std::size_t>(i)], i);
        return m;
    });
    std::printf("growth_build_ms,presized_2n,%d,%.1f\n", n, buildPre);
    std::printf("growth_build_ms,grown_from_8,%d,%.1f\n", n, buildGrown);

    HashMap<std::string, int> pre(un * 2), grown;
    std::unordered_map<std::string, int> stdmap;
    stdmap.reserve(un);
    for (int i = 0; i < n; ++i) {
        const auto& t = flat[static_cast<std::size_t>(i)];
        pre.insert(t, i);
        grown.insert(t, i);
        stdmap.emplace(t, i);
    }
    std::printf("growth_info,grown_from_8,%d,buckets=%zu,rehashes=%zu,load=%.3f\n", n,
                grown.bucketCount(), grown.rehashCount(), grown.loadFactor());
    std::printf("growth_info,presized_2n,%d,buckets=%zu,rehashes=%zu,load=%.3f\n", n,
                pre.bucketCount(), pre.rehashCount(), pre.loadFactor());

    std::vector<std::string> probes;
    probes.reserve(kLookups);
    for (int k = 0; k < kLookups; ++k) probes.push_back(tagFor((k * 7919) % n));
    const auto look = [&](auto& table) {
        return medianMicros([&] {
            long long acc = 0;
            for (const auto& p : probes) {
                if constexpr (std::is_same_v<std::decay_t<decltype(table)>,
                                             std::unordered_map<std::string, int>>) {
                    const auto it = table.find(p);
                    if (it != table.end()) acc += it->second;
                } else {
                    if (const auto v = table.find(p)) acc += *v;
                }
            }
            return acc;
        }) * 1000.0 / kLookups;
    };
    std::printf("growth_lookup_ns,presized_2n,%d,%.1f\n", n, look(pre));
    std::printf("growth_lookup_ns,grown_from_8,%d,%.1f\n", n, look(grown));
    std::printf("growth_lookup_ns,std_unordered_map,%d,%.1f\n", n, look(stdmap));
}

int main() {
    std::printf("experiment,label,n,linear_ns,std_ns,mine_ns\n");
    for (const int n : {1, 2, 3, 4, 6, 8, 10, 12, 16, 20, 25, 32, 64, 128, 256}) {
        std::vector<std::string> flat;
        for (int i = 0; i < n; ++i) flat.push_back(tagFor(i));
        crossover("synthetic", flat);
    }
    crossover("my_catalog", catalogTags("sample-data/my-catalog.csv"));
    std::printf("experiment,variant,n,value\n");
    growth(1000000);
    return 0;
}