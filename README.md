# Sistem Manajemen Perpustakaan

## Ikhtisar

Ini adalah sistem manajemen perpustakaan sederhana yang dibangun menggunakan C++ dan MySQL untuk penyimpanan data. Aplikasi ini memungkinkan pustakawan untuk mengelola buku, anggota, dan transaksi.

## Prasyarat

- **C++ Compiler**: GCC atau Clang (untuk macOS/Linux) atau Visual Studio (untuk Windows)
- **CMake**: Versi 3.10 atau lebih tinggi
- **MySQL**: Versi 8.0 atau lebih tinggi
- **MySQL Connector/C**: Untuk integrasi MySQL dengan C++

## Instalasi

### macOS/Linux

1. **Instal MySQL**:

   ```bash
   brew install mysql
   ```

2. **Mulai Layanan MySQL**:

   ```bash
   brew services start mysql
   ```

3. **Instal MySQL Connector/C**:

   ```bash
   brew install mysql-connector-c
   ```

4. **Clone Repositori**:

   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

5. **Build Proyek**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

### Windows

1. **Instal MySQL**:

   - Unduh dan instal MySQL dari [MySQL Downloads](https://dev.mysql.com/downloads/installer/).
   - Selama instalasi, atur password root.

2. **Instal MySQL Connector/C**:

   - Unduh MySQL Connector/C dari [MySQL Connector/C Downloads](https://dev.mysql.com/downloads/connector/c/).
   - Ekstrak file ke lokasi yang diketahui.

3. **Atur Variabel Lingkungan**:

   - Tambahkan direktori `lib` dari MySQL Connector/C ke PATH sistem.
   - Contoh: `C:\Program Files\MySQL\MySQL Connector C 6.1\lib`

4. **Clone Repositori**:

   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

5. **Build Proyek**:
   - Buka CMake GUI dan atur direktori sumber ke direktori repositori.
   - Atur direktori build ke folder baru (misalnya, `build`).
   - Konfigurasi dan generate proyek.
   - Buka solusi yang dihasilkan di Visual Studio dan build proyek.

## Library yang Diperlukan

- **MySQL Connector/C**: Untuk koneksi ke database MySQL.
- **CMake**: Untuk mengelola build proyek.
- **Visual Studio**: Untuk Windows, pastikan Anda memiliki Visual Studio dengan dukungan C++.

## Penggunaan

1. **Mulai Aplikasi**:

   ```bash
   ./library_management
   ```

2. **Login**:

   - Username: `admin`
   - Password: `admin123`

3. **Menu Utama**:
   - **Manajemen Buku**: Tambah, update, hapus, dan cari buku.
   - **Manajemen Anggota**: Tambah, update, hapus, dan cari anggota.
   - **Manajemen Transaksi**: Pinjam dan kembalikan buku.
   - **Laporan**: Lihat laporan tentang buku dan transaksi.

## Pemecahan Masalah

- **Masalah Koneksi Database**: Pastikan MySQL berjalan dan database `library_system` ada.
- **Masalah Build**: Pastikan semua dependensi terinstal dengan benar dan variabel lingkungan diatur.

## Lisensi

Proyek ini dilisensikan di bawah Lisensi MIT.
