#include "../include/Admin.h"
#include <iostream>

Admin::Admin() : Pengguna(), db(nullptr) {}

Admin::Admin(const std::string& id, const std::string& nama, const std::string& username, const std::string& password, DatabaseManager* db)
    : Pengguna(id, nama, username, password), db(db) {}

void Admin::setDatabase(DatabaseManager* db) {
    this->db = db;
}

void Admin::tambahBuku() {
    // Cek apakah tabel buku kosong
    auto books = db->getAllBuku();
    if (books.empty()) {
        db->executeQuery("ALTER TABLE buku AUTO_INCREMENT = 1;");
    }
    std::string judul, penulis;
    std::cout << "Judul: "; std::getline(std::cin, judul);
    std::cout << "Penulis: "; std::getline(std::cin, penulis);
    std::string status = "tersedia";
    if (db->insertBuku(judul, penulis, status)) {
        std::cout << "Buku berhasil ditambahkan!\n";
    } else {
        std::cout << "Gagal menambah buku.\n";
    }
    std::cout << "\n";
}

void Admin::kelolaDataBuku() {
    std::cout << "=== Daftar Buku ===\n";
    auto books = db->getAllBuku();
    for (const auto& b : books) {
        std::cout << "ID: " << b[0] << ", Judul: " << b[1] << ", Penulis: " << b[2] << ", Status: " << b[3] << "\n";
    }
    std::cout << "\n1. Edit Buku\n2. Hapus Buku\n0. Kembali\n";
    int choice;
    std::cout << "Pilih menu: "; std::cin >> choice; std::cin.ignore();
    if (choice == 1) {
        int id; std::cout << "ID Buku: "; std::cin >> id; std::cin.ignore();
        std::string judul, penulis, status;
        std::cout << "Judul baru: "; std::getline(std::cin, judul);
        std::cout << "Penulis baru: "; std::getline(std::cin, penulis);
        while (true) {
            std::cout << "Status baru (tersedia/dipinjam): ";
            std::getline(std::cin, status);
            // Konversi ke lowercase
            std::string status_lc = status;
            for (auto& c : status_lc) c = std::tolower(c);
            if (status_lc == "tersedia" || status_lc == "dipinjam") {
                status = status_lc;
                break;
            } else {
                std::cout << "Input tidak valid. Masukkan hanya 'tersedia' atau 'dipinjam'.\n";
            }
        }
        if (db->updateBuku(id, judul, penulis, status)) std::cout << "Update berhasil!\n";
        else std::cout << "Update gagal!\n";
    } else if (choice == 2) {
        int id; std::cout << "ID Buku: "; std::cin >> id; std::cin.ignore();
        if (db->deleteBuku(id)) std::cout << "Buku dihapus!\n";
        else std::cout << "Gagal hapus buku!\n";
    }
}

void Admin::kelolaTransaksiPeminjaman() {
    std::cout << "=== Daftar Peminjaman ===\n";
    auto pinjam = db->getAllPeminjaman();
    for (const auto& p : pinjam) {
        std::cout << "ID: " << p[0] << ", Buku: " << p[1] << ", Anggota: " << p[2] << ", Pinjam: " << p[3] << ", Kembali: " << p[4] << ", Status: " << p[5] << "\n";
    }
    std::cout << "\n1. Update Status Peminjaman\n0. Kembali\n";
    int choice;
    std::cout << "Pilih menu: "; std::cin >> choice; std::cin.ignore();
    if (choice == 1) {
        int id; std::cout << "ID Peminjaman: "; std::cin >> id; std::cin.ignore();
        std::string status, tgl_kembali;
        std::cout << "Status baru (dipinjam/kembali): "; std::getline(std::cin, status);
        std::cout << "Tanggal kembali (YYYY-MM-DD): "; std::getline(std::cin, tgl_kembali);
        if (db->updatePeminjaman(id, status, tgl_kembali)) std::cout << "Update berhasil!\n";
        else std::cout << "Update gagal!\n";
    }
} 