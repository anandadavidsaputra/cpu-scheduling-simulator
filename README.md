# Simulator Antrean Proses CPU (C++)

Aplikasi simulator berbasis *Command Line Interface* (CLI) untuk mensimulasikan bagaimana CPU mengeksekusi antrean proses menggunakan algoritma **FCFS (First-Come, First-Served)** dan **Priority Scheduling (Non-Preemptive)**. Proyek ini dibuat menggunakan bahasa pemrograman C++ untuk memenuhi tugas Project-Based Learning (PBL) UAS Mata Kuliah Algoritma dan Pemrograman.

---

## 📌 Penjelasan Singkat Proyek

Proyek ini mengumpamakan CPU sebagai sebuah loket pelayanan tunggal dan aplikasi/proses sebagai nasabah yang mengantre. Aplikasi ini menguji dua aturan antrean:
1. **FCFS (Urutan Kedatangan):** Proses yang datang paling awal akan dilayani terlebih dahulu (*Siapa Cepat, Dia Dapat*).
2. **Priority Non-Preemptive (Tingkat Prioritas):** CPU akan memilih proses yang memiliki tingkat kepentingan/prioritas tertinggi (ditandai dengan angka prioritas terkecil) dari seluruh proses yang sudah tiba di antrean.

Aplikasi ini membaca data input dari file teks luar, melakukan simulasi, lalu mengeluarkan hasil metrik berupa *Waiting Time* (Waktu Tunggu), *Turnaround Time* (Total Waktu), serta urutan eksekusi (*Gantt Chart*) ke dalam file log teks otomatis.

---

## 📂 Format File Input (`input.txt`)

Sebelum menjalankan program, pastikan kamu membuat file bernama `input.txt` di dalam folder yang sama dengan file aplikasi. Isi file tersebut dengan angka yang dipisahkan oleh spasi dengan format:

`ID_Proses` `Arrival_Time` `Burst_Time` `Priority`

### Contoh Isi File:
```text
1 0 8 3
2 1 4 1
3 2 2 5
4 3 5 2
5 4 3 4
