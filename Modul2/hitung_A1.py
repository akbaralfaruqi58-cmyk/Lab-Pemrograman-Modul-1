import csv

unique = []
cmp_scan = 0
cmp_hash = 0
with open("sample-data/my-catalog.csv", newline="") as f:
    for row in csv.DictReader(f):
        tag = row["tag"]
        dup = False
        for kept in unique:
            cmp_scan += 1
            if kept == tag:
                dup = True
                break
        if not dup:
            unique.append(tag)
        cmp_hash += 1
print(cmp_scan, cmp_hash, len(unique))