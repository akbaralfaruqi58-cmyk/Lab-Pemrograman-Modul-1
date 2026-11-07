"""A6/2.4 Cacat A (Python): waktu satu panggilan search("") pada n baris."""
import sys
import time

sys.path.insert(0, "py")
from pinjamlab.catalog_service import CatalogService

SIZES = (100, 1000, 10000, 30000, 100000, 1000000)
prev = None                                   # (n, ms) dari ukuran sebelumnya
for n in SIZES:
    # kalau tebakan O(n^2) dari ukuran sebelumnya sudah > 20 detik, jangan dijalankan
    if prev and prev[1] * (n / prev[0]) ** 2 > 20000:
        print(f"berhenti: n={n} diperkirakan > 20 detik (dari n={prev[0]}: {prev[1]:.1f} ms)")
        break
    with open("bin/_gen.csv", "w", encoding="utf-8") as f:
        f.write("tag,model,category,daily_fine,condition\n")
        for i in range(n):
            f.write(f"LP-{i},Model {i},Cat{i % 40},1000,GOOD\n")
    svc = CatalogService()
    svc.load("bin/_gen.csv")
    t0 = time.perf_counter()
    hits = svc.search("")
    ms = (time.perf_counter() - t0) * 1000
    print(f"n={n},search_ms={ms:.1f},hits={len(hits)}", flush=True)
    prev = (n, ms)