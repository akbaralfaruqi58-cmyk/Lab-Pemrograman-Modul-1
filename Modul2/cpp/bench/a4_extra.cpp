// A4: empat kondisi di mana struktur hash berhenti menang. Semua diukur.
//   E1 kunci berkolisi (hash buruk)      -> pencarian jatuh ke O(n)
//   E2 kunci panjang (hash mahal)        -> scan menang pada n lebih besar
//   E3 jeda rehash pada satu insert      -> latensi puncak, bukan rata-rata
//   E4 query rentang                     -> hash harus menelusuri SEMUA elemen
#include "pinjamlab/structures/hash_map.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <map>
#include <string>
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

struct ZeroHash {                                      // semua kunci masuk bucket 0
    std::size_t operator()(const std::string&) const noexcept { return 0; }
};

static long long scanAll(const std::vector<std::string>& flat,
                         const std::vector<std::string>& probes) {
    long long acc = 0;
    for (const auto& p : probes)
        for (std::size_t i = 0; i < flat.size(); ++i)
            if (flat[i] == p) { acc += static_cast<long long>(i); break; }
    return acc;
}

static void e1() {
    constexpr int kLook = 2000;
    for (const int n : {100, 1000, 10000}) {
        std::vector<std::string> flat;
        for (int i = 0; i < n; ++i) flat.push_back(tagFor(i));
        std::vector<std::string> probes;
        for (int k = 0; k < kLook; ++k)
            probes.push_back(flat[(static_cast<std::size_t>(k) * 7919) % flat.size()]);

        HashMap<std::string, int> good(static_cast<std::size_t>(n) * 2);
        HashMap<std::string, int, ZeroHash> bad;
        for (int i = 0; i < n; ++i) {
            good.insert(flat[static_cast<std::size_t>(i)], i);
            bad.insert(flat[static_cast<std::size_t>(i)], i);
        }
        const double tg = medianMicros([&] { long long a = 0; for (const auto& p : probes) if (const auto v = good.find(p)) a += *v; return a; });
        const double tb = medianMicros([&] { long long a = 0; for (const auto& p : probes) if (const auto v = bad.find(p)) a += *v; return a; });
        const double ts = medianMicros([&] { return scanAll(flat, probes); });
        std::printf("E1_collision,n=%d,hash_normal_ns=%.1f,hash_all_collide_ns=%.1f,vector_scan_ns=%.1f,longest_chain=%zu\n",
                    n, tg * 1000.0 / kLook, tb * 1000.0 / kLook, ts * 1000.0 / kLook, bad.bucketSize(0));
    }
}

static std::string longKey(int i, int len) {           // beda di AWAL, sisanya 'x'
    std::string k = tagFor(i);
    k.resize(static_cast<std::size_t>(len), 'x');
    return k;
}

static void e2() {
    constexpr int kLook = 20000;
    for (const int len : {16, 1024, 8192}) {
        for (const int n : {16, 64, 256, 1024}) {
            std::vector<std::string> flat;
            for (int i = 0; i < n; ++i) flat.push_back(longKey(i, len));
            std::vector<std::string> probes;
            for (int k = 0; k < kLook; ++k)
                probes.push_back(flat[(static_cast<std::size_t>(k) * 7919) % flat.size()]);
            HashMap<std::string, int> m(static_cast<std::size_t>(n) * 2);
            for (int i = 0; i < n; ++i) m.insert(flat[static_cast<std::size_t>(i)], i);
            const double th = medianMicros([&] { long long a = 0; for (const auto& p : probes) if (const auto v = m.find(p)) a += *v; return a; });
            const double ts = medianMicros([&] { return scanAll(flat, probes); });
            std::printf("E2_longkey,key_len=%d,n=%d,hash_ns=%.1f,scan_ns=%.1f\n",
                        len, n, th * 1000.0 / kLook, ts * 1000.0 / kLook);
        }
    }
}

static void e3() {
    constexpr int n = 1000000;
    std::vector<std::string> flat;
    flat.reserve(n);
    for (int i = 0; i < n; ++i) flat.push_back(tagFor(i));
    HashMap<std::string, int> m;                       // 8 bucket awal
    std::vector<double> t;
    t.reserve(n);
    for (int i = 0; i < n; ++i) {
        const auto t0 = Clock::now();
        m.insert(flat[static_cast<std::size_t>(i)], i);
        const auto t1 = Clock::now();
        t.push_back(std::chrono::duration<double, std::micro>(t1 - t0).count());
    }
    const auto mx = std::max_element(t.begin(), t.end());
    std::vector<double> sorted = t;
    std::sort(sorted.begin(), sorted.end());
    int over1ms = 0;
    for (const double x : t) if (x > 1000.0) ++over1ms;
    std::printf("E3_pause,n=%d,median_insert_ns=%.0f,p99_insert_ns=%.0f,max_insert_ms=%.2f,max_at_insert=%td,inserts_over_1ms=%d,rehashes=%zu\n",
                n, sorted[sorted.size() / 2] * 1000.0, sorted[sorted.size() * 99 / 100] * 1000.0,
                *mx / 1000.0, mx - t.begin() + 1, over1ms, m.rehashCount());
}

static void e4() {
    constexpr int n = 100000, kQ = 200, kWidth = 100;
    std::map<std::string, int> ordered;
    std::unordered_map<std::string, int> hashed;
    for (int i = 0; i < n; ++i) {
        const auto t = tagFor(i);
        ordered.emplace(t, i);
        hashed.emplace(t, i);
    }
    const std::string lo = tagFor(50000), hi = tagFor(50000 + kWidth);
    const double tm = medianMicros([&] {
        long long c = 0;
        for (int q = 0; q < kQ; ++q)
            for (auto it = ordered.lower_bound(lo); it != ordered.end() && it->first < hi; ++it) ++c;
        return c;
    });
    const double th = medianMicros([&] {
        long long c = 0;
        for (int q = 0; q < kQ; ++q)
            for (const auto& kv : hashed) if (kv.first >= lo && kv.first < hi) ++c;
        return c;
    }, 3);
    std::printf("E4_range,n=%d,hits=%d,std_map_us_per_query=%.2f,unordered_map_us_per_query=%.2f\n",
                n, kWidth, tm / kQ, th / kQ);
}

int main() {
    e1(); e2(); e3(); e4();
    return 0;
}