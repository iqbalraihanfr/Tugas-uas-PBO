#ifndef ANGGOTA_H
#define ANGGOTA_H

#include "Pengguna.h"
#include "DatabaseManager.h"
#include <vector>
#include <string>
#include "Buku.h"
#include "Peminjaman.h"

class Anggota : public Pengguna {
private:
    DatabaseManager* db;
    std::vector<Buku> wishlist;
public:
    Anggota();
    Anggota(const std::string& id, const std::string& nama, const std::string& username, const std::string& password, DatabaseManager* db = nullptr);
    void setDatabase(DatabaseManager* db);
    std::vector<Buku> getWishlist() const;
    void tambahKeWishlist();
    void cariBuku();
    bool pinjamBuku();
    bool kembalikanBuku();
};

#endif // ANGGOTA_H 