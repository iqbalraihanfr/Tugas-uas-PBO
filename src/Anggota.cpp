#include "../include/Anggota.h"
#include <iostream>

Anggota::Anggota() : Pengguna(), db(nullptr) {}

Anggota::Anggota(const std::string& id, const std::string& nama, const std::string& username, const std::string& password, DatabaseManager* db)
    : Pengguna(id, nama, username, password), db(db) {}

void Anggota::setDatabase(DatabaseManager* db) {
    this->db = db;
}

void Anggota::cariBuku() {
    std::string query;
    std::cout << "Masukkan judul/penulis: ";
    std::getline(std::cin, query);
    auto results = db->searchBuku(query);
    std::cout << "=== Hasil Pencarian ===\n";
    for (const auto& b : results) {
        std::cout << "ID: " << b[0] << ", Judul: " << b[1] << ", Penulis: " << b[2] << ", Status: " << b[3] << "\n";
    }
    if (results.empty()) std::cout << "Tidak ada buku ditemukan.\n";
}

void Anggota::tampilkanBukuTersedia() const {
    auto books = db->getAvailableBuku();
    std::cout << "\n=== Daftar Buku Tersedia ===\n";
    if (books.empty()) {
        std::cout << "Tidak ada buku yang tersedia saat ini.\n";
    } else {
        for (const auto& b : books) {
            std::cout << "ID: " << b[0] << ", Judul: " << b[1] << ", Penulis: " << b[2] << "\n";
        }
    }
    std::cout << std::endl;
}

bool Anggota::pinjamBuku() {
    tampilkanBukuTersedia(); // Tampilkan buku sebelum input
    std::string judul;
    std::cout << "Masukkan judul buku yang ingin dipinjam: ";
    std::getline(std::cin, judul);
    auto results = db->searchBuku(judul);
    int found_id = -1;
    std::string found_judul;
    for (const auto& b : results) {
        std::string status_lc = b[3];
        for (auto& c : status_lc) c = std::tolower(c);
        if (status_lc == "tersedia") {
            found_id = std::stoi(b[0]);
            found_judul = b[1];
            break;
        }
    }
    if (found_id == -1) {
        std::cout << "Buku tidak ditemukan atau sedang dipinjam.\n";
        return false;
    }
    std::string id_anggota = this->getIdPengguna();
    bool result = db->pinjamBuku(found_id, id_anggota);
    if (result) {
        std::cout << "Berhasil meminjam buku '" << found_judul << "'. Harap dikembalikan tepat waktu.\n";
        return true;
    } else {
        std::cout << "Gagal meminjam buku. (Cek status buku dan koneksi database)\n";
        return false;
    }
}

bool Anggota::kembalikanBuku() {
    int id_peminjaman;
    std::cout << "ID Peminjaman: ";
    std::cin >> id_peminjaman; std::cin.ignore();
    if (db->kembalikanBuku(id_peminjaman)) {
        std::cout << "Buku berhasil dikembalikan!\n";
        return true;
    } else {
        std::cout << "Gagal mengembalikan buku.\n";
        return false;
    }
}

void Anggota::tambahKeWishlist() {
    std::string judul;
    std::cout << "Masukkan judul buku yang ingin ditambah ke wishlist: ";
    std::getline(std::cin, judul);
    auto results = db->searchBuku(judul);
    int found_id = -1;
    for (const auto& b : results) {
        found_id = std::stoi(b[0]);
        std::cout << "Buku ditemukan: ID: " << b[0] << ", Judul: " << b[1] << ", Penulis: " << b[2] << ", Status: " << b[3] << "\n";
        break;
    }
    if (found_id == -1) {
        std::cout << "Buku tidak ditemukan.\n";
        return;
    }
    std::string id_anggota = this->getIdPengguna();
    if (db->tambahWishlist(id_anggota, found_id)) {
        std::cout << "Buku berhasil ditambahkan ke wishlist!\n";
    } else {
        std::cout << "Buku sudah ada di wishlist atau terjadi kesalahan.\n";
    }
}

void Anggota::lihatWishlist() {
    std::string id_anggota = this->getIdPengguna();
    auto wishlist_ids = db->getWishlist(std::stoi(id_anggota));
    if (wishlist_ids.empty()) {
        std::cout << "Wishlist kosong.\n";
        return;
    }
    std::cout << "=== Wishlist Anda ===\n";
    for (int id_buku : wishlist_ids) {
        auto results = db->searchBuku(std::to_string(id_buku));
        for (const auto& b : results) {
            if (std::stoi(b[0]) == id_buku) {
                std::cout << "ID: " << b[0] << ", Judul: " << b[1] << ", Penulis: " << b[2] << ", Status: " << b[3] << "\n";
            }
        }
    }
}

std::vector<Buku> Anggota::getWishlist() const {
    return wishlist;
} 