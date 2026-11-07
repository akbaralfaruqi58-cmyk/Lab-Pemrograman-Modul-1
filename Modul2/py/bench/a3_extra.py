"""A3: titik persilangan Python (list linear scan vs dict) untuk n kecil.

Jalankan dari folder Module 2:  python py/bench/a3_extra.py
"""
from __future__ import annotations

import timeit

LOOKUPS = 20_000


def tag_for(i: int) -> str:
    return f"LP-{i:06d}"


def catalog_tags(path: str) -> list[str]:
    out: list[str] = []
    with open(path, encoding="utf-8") as f:
        next(f)                                   # header
        for line in f:
            t = line.split(",", 1)[0].strip()
            if t and t not in out:
                out.append(t)
    return out


def median_ns(fn, lookups: int, repeats: int = 7) -> float:
    fn()                                          # warm-up, dibuang
    samples = timeit.repeat(fn, repeat=repeats, number=1)
    samples.sort()
    return samples[len(samples) // 2] / lookups * 1e9


def run(label: str, flat: list[str]) -> None:
    n = len(flat)
    table = {t: i for i, t in enumerate(flat)}
    probes = [flat[(k * 7919) % n] for k in range(LOOKUPS)]

    def linear() -> int:
        acc = 0
        for p in probes:
            for i, v in enumerate(flat):
                if v == p:
                    acc += i
                    break
        return acc

    def hashed() -> int:
        acc = 0
        for p in probes:
            acc += table[p]
        return acc

    print(f"crossover_py,{label},{n},{median_ns(linear, LOOKUPS):.1f},{median_ns(hashed, LOOKUPS):.1f}")


def main() -> None:
    print("experiment,label,n,list_ns,dict_ns")
    for n in (1, 2, 3, 4, 6, 8, 10, 12, 16, 20, 25, 32, 64, 128, 256):
        run("synthetic", [tag_for(i) for i in range(n)])
    run("my_catalog", catalog_tags("sample-data/my-catalog.csv"))


if __name__ == "__main__":
    main()