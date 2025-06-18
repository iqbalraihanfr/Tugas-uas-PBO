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

bool Anggota::pinjamBuku() {
    int id_buku;
    std::cout << "ID Buku yang ingin dipinjam: ";
    std::cin >> id_buku; std::cin.ignore();
    std::string id_anggota = this->getIdPengguna();
    if (db->pinjamBuku(id_buku, id_anggota)) {
        std::cout << "Buku berhasil dipinjam!\n";
        return true;
    } else {
        std::cout << "Gagal meminjam buku.\n";
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
    int id_buku;
    std::cout << "ID Buku yang ingin ditambah ke wishlist: ";
    std::cin >> id_buku; std::cin.ignore();
    std::string id_anggota = this->getIdPengguna();
    if (db->tambahWishlist(id_anggota, id_buku)) {
        std::cout << "Buku berhasil ditambahkan ke wishlist!\n";
    } else {
        std::cout << "Gagal menambah ke wishlist.\n";
    }
}

std::vector<Buku> Anggota::getWishlist() const {
    return wishlist;
} 