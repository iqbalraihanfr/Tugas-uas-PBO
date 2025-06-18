#ifndef ADMIN_H
#define ADMIN_H

#include "Pengguna.h"
#include "Buku.h"
#include "Anggota.h"
#include "Peminjaman.h"
#include "DatabaseManager.h"
#include <vector>
#include <string>

class Admin : public Pengguna {
private:
    DatabaseManager* db;
public:
    Admin();
    Admin(const std::string& id, const std::string& nama, const std::string& username, const std::string& password, DatabaseManager* db = nullptr);
    void tambahBuku();
    void kelolaDataBuku();
    void kelolaTransaksiPeminjaman();
    void kelolaAkunPengguna();
    void setDatabase(DatabaseManager* db);
};

#endif // ADMIN_H 