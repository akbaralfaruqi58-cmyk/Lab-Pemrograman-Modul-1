"""A6/2.4 Cacat A: profil satu panggilan search("") pada n = 10.000 baris."""
import cProfile
import pstats
import sys

sys.path.insert(0, "py")
from pinjamlab.catalog_service import CatalogService

n = 10000
with open("bin/_gen.csv", "w", encoding="utf-8") as f:
    f.write("tag,model,category,daily_fine,condition\n")
    for i in range(n):
        f.write(f"LP-{i},Model {i},Cat{i % 40},1000,GOOD\n")
svc = CatalogService()
svc.load("bin/_gen.csv")

pr = cProfile.Profile()
pr.enable()
svc.search("")
pr.disable()
pstats.Stats(pr).sort_stats("tottime").print_stats(6)