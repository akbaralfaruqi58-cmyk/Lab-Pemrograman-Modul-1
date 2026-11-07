// A6/2.4 Cacat A: waktu satu panggilan search("") pada n baris (semua baris cocok).
#include "pinjamlab/catalog_service.hpp"

#include <chrono>
#include <cstdio>
#include <fstream>

int main() {
    const char* path = "bin/_gen.csv";
    for (const int n : {100, 1000, 10000, 30000, 100000, 1000000}) {
        {
            std::ofstream out(path);
            out << "tag,model,category,daily_fine,condition\n";
            for (int i = 0; i < n; ++i)
                out << "LP-" << i << ",Model " << i << ",Cat" << (i % 40) << ",1000,GOOD\n";
        }
        pinjamlab::CatalogService svc;
        svc.load(path);
        const auto t0 = std::chrono::steady_clock::now();
        const auto hits = svc.search("");
        const auto t1 = std::chrono::steady_clock::now();
        const double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::printf("n=%d,search_ms=%.1f,hits=%zu\n", n, ms, hits.size());
        std::fflush(stdout);
        if (ms > 20000.0) { std::printf("berhenti: n=%d sudah melewati 20 detik\n", n); break; }
    }
}