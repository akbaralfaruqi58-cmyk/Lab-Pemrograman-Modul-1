import csv, sys
from collections import Counter

MY = sys.argv[1] if len(sys.argv) > 1 else "sample-data/my-catalog.csv"
SAMPLE = "sample-data/sample-catalog.csv"

def baca(path):
    with open(path, newline="", encoding="utf-8") as f:
        return list(csv.reader(f))

rows = baca(MY)
header, data = rows[0], rows[1:]
masalah = []

if header != ["tag", "model", "category", "daily_fine", "condition"]:
    masalah.append(f"header salah: {header}")
for no, r in enumerate(data, start=2):
    if len(r) != 5:
        masalah.append(f"baris {no}: jumlah kolom {len(r)} (harus 5; jangan pakai koma di dalam isi)")
        continue
    if not r[3].isdigit():
        masalah.append(f"baris {no}: daily_fine bukan angka: {r[3]!r}")

tags = Counter(r[0] for r in data if len(r) == 5)
dobel = {t: c for t, c in tags.items() if c > 1}
kat_sampel = {r[2] for r in baca(SAMPLE)[1:]}
kat_saya = {r[2] for r in data if len(r) == 5}
kat_baru = kat_saya - kat_sampel

print(f"jumlah baris data      : {len(data)}   (minimal 50)")
print(f"tag yang muncul >1 kali: {len(dobel)}   (minimal 3) -> {dobel}")
print(f"kategori baru          : {len(kat_baru)}   (minimal 2) -> {sorted(kat_baru)}")
print(f"tag unik               : {len(tags)}")
if len(data) < 50: masalah.append("kurang dari 50 baris")
if len(dobel) < 3: masalah.append("kurang dari 3 tag duplikat")
if len(kat_baru) < 2: masalah.append("kurang dari 2 kategori yang tidak ada di sampel")
print("\nMASALAH:" if masalah else "\nOK: memenuhi syarat Praktikum 1.2")
for m in masalah: print(" -", m)