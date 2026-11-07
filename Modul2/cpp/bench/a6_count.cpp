// A6/2.4 Cacat A: hitung perbandingan tag pada satu panggilan search("").
#include "pinjamlab/catalog_service.hpp"

#include <cstdio>
#include <fstream>

int main() {
    for (const int n : {1000, 10000}) {
        const char* path = "bin/_gen.csv";
        {
            std::ofstream out(path);
            out << "tag,model,category,daily_fine,condition\n";
            for (int i = 0; i < n; ++i)
                out << "LP-" << i << ",Model " << i << ",Cat" << (i % 40) << ",1000,GOOD\n";
        }
        pinjamlab::CatalogService svc;
        svc.load(path);
        pinjamlab::g_cmp = 0;
        const auto hits = svc.search("");
        const long long expected = static_cast<long long>(n) * (n - 1) / 2;
        std::printf("n=%d hits=%zu perbandingan=%lld  n(n-1)/2=%lld\n",
                    n, hits.size(), pinjamlab::g_cmp, expected);
    }
}