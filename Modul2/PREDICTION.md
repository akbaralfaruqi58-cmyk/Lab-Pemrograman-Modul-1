# PREDICTION.md

Ditulis pada 7 November 2026, SETELAH tabel empat arah pertama (`bench-results.csv`)
diukur dan hasilnya sudah saya lihat, dan SEBELUM eksperimen di bawah dijalankan.
Karena itu tabel empat arah pertama tidak punya prediksi sebelum pengukuran. Prediksi di
bawah hanya untuk eksperimen yang belum saya ukur.

Data acuan dari pengukuran pertama (ns per lookup, mesin saya):

| Implementasi | n=100 | n=10^4 | n=10^6 |
|---|---:|---:|---:|
| C++ linear scan | 520,0 | 48.411,0 | 4.842.832,0 |
| C++ std::unordered_map | 59,8 | 120,2 | 368,3 |
| C++ HashMap buatan | 52,2 | 75,7 | 234,1 |
| Python list linear scan | 4.960,5 | 864.550,0 | 62.159.798,5 |
| Python dict | 206,0 | 308,4 | 703,4 |

## 1. Titik persilangan C++ (linear scan vs std::unordered_map)

**Prediksi: n ≈ 13.**

Linear scan berhenti saat kunci ditemukan, jadi rata-rata memeriksa sekitar n/2 elemen.
Dari data saya, biaya per elemen yang diperiksa (c1) kira-kira 10 ns (520/50 = 10,4 ns
pada n=100; 9,7 ns pada n=10^4 dan 10^6). Satu lookup hash (c2) sekitar 52-60 ns.
Titik silang mendekati c1 x n/2 = c2, sehingga n ≈ 2 x c2 / c1 ≈ 10-12.
Saya menaruh prediksi sedikit di atas hasil rumus (13), karena pada n kecil seluruh data
dan seluruh loop muat di cache L1 dan branch prediction akurat, sehingga biaya per elemen
bisa sedikit lebih rendah dari 10 ns, dan itu menggeser titik silang naik sedikit.
Selisihnya dari rumus kecil, jadi saya cukup yakin dengan orde besarnya (belasan elemen).

## 2. Titik persilangan Python (list vs dict)

**Prediksi: n ≈ 7.**

Loop Python `for i, v in enumerate(flat)` memakan sekitar 99 ns per elemen pada n=100
(4.960,5 / 50), sedangkan satu lookup `dict` sekitar 206 ns. Rumus 2 x c2 / c1 memberi
n ≈ 4. Biaya per elemen pada n kecil kemungkinan lebih rendah daripada 99 ns, karena
datanya muat di cache (biaya per elemen yang saya ukur naik jadi sekitar 173 ns pada
n=10^4 saat data menyebar di memori). Biaya per elemen yang lebih rendah menggeser titik
silang ke atas, sehingga prediksi saya 7, sedikit di atas hasil rumus. Persilangan
Python lebih awal daripada C++ karena loop Python mahal per elemen (interpreter), sedangkan
`dict` dikerjakan di dalam C.

## 3. HashMap 8 bucket awal vs di-presize n*2 (n = 10^6)

Ukuran bucket yang dibandingkan: tumbuh dari 8 bucket (rehash otomatis, `kMaxLoad = 0,75`)
melawan dibuat langsung dengan 2n bucket seperti di harness benchmark.

Hasil simulasi pertumbuhan dari 8 bucket sampai n = 10^6: jumlah bucket akhir 2.097.152,
faktor beban akhir sekitar 0,48 (versi presize: 0,5), terjadi 18 kali rehash, dan total
node yang dipindah ulang sekitar 1,57 x n.

**a) Waktu membangun (n insert): versi 8 bucket awal sekitar 1,7x lebih lambat.**

Membangun sudah mencakup satu alokasi node (`new`) dan satu hash per insert. Versi 8 bucket
awal menambah pekerjaan rehash: sekitar 1,57 x n node dipindah ulang, dan setiap node
dihitung ulang bucket-nya (`bucketOf` memanggil hash lagi) lalu ditulis ke array bucket
baru dengan akses memori yang acak. Jika memindah satu node memakan sekitar 45% dari biaya
satu insert, totalnya 1 + 1,57 x 0,45 ≈ 1,7x.

**b) Waktu pencarian: hampir sama, selisihnya kurang dari 10%.**

Panjang rantai rata-rata untuk pencarian yang berhasil sekitar 1 + λ/2. Versi 8 bucket
awal (λ ≈ 0,48) menghasilkan sekitar 1,24 dan versi presize (λ = 0,5) sekitar 1,25, selisih
sekitar 1%. Jumlah bucket akhirnya juga hampir sama (2.097.152 vs 2.000.000), sehingga
ukuran array bucket dan jumlah akses memori per lookup hampir sama. Selisih lain yang
mungkin muncul hanya dari urutan node dalam rantai setelah rehash (rehash mengubah
urutan node dalam rantai), yang saya perkirakan kecil. Karena itu saya membatasi
selisihnya di bawah 10%.

## 4. Apakah HashMap buatan saya masih mengalahkan std::unordered_map pada n = 10^6?

**Prediksi: ya, masih menang, dengan selisih lebih kecil dari sebelumnya.**

Pada pengukuran pertama HashMap saya sudah lebih cepat di ketiga n (234,1 vs 368,3 ns
pada n=10^6). Kedua struktur memakai fungsi hash yang sama (`std::hash<std::string>`),
sehingga biaya hashing tidak membedakan. Yang membedakan: faktor beban maksimum
`std::unordered_map` adalah 1,0 sedangkan HashMap saya ditahan di 0,75 (akhir sekitar
0,48 pada n=10^6), jadi rantai saya lebih pendek dan perbandingan kunci lebih sedikit.
Dengan 8 bucket awal faktor beban akhirnya tetap sekitar 0,48, jadi keunggulan itu
seharusnya bertahan. Selisihnya kemungkinan mengecil karena rehash membuat susunan
node di memori kurang rapi dibanding dibangun sekali.
