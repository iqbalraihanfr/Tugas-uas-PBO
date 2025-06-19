#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "Pengguna.h"
#include <mysql.h>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class DatabaseManager {
private:
    MYSQL* conn;
    bool connected;
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    unsigned int port;

public:
    // Constructor & Destructor
    DatabaseManager();
    ~DatabaseManager();

    // Connection management
    bool connect(const std::string& host, const std::string& user, 
                const std::string& password, const std::string& database);
    void disconnect();
    bool isConnected() const;

    // Database operations
    bool executeQuery(const std::string& query);
    MYSQL_RES* executeSelect(const std::string& query);
    int getLastInsertId();
    std::string escapeString(const std::string& str);
    
    // Table operations
    bool createTable(const std::string& tableName, const std::string& columns);
    bool insertRecord(const std::string& table, const std::string& columns, const std::string& values);
    bool updateRecord(const std::string& table, const std::string& setClause, const std::string& whereClause);
    bool deleteRecord(const std::string& table, const std::string& whereClause);
    MYSQL_RES* selectRecords(const std::string& table, const std::string& columns = "*", const std::string& whereClause = "");

    // Error handling
    std::string getLastError() const;

    // User authentication and role
    Pengguna* validateLogin(const std::string& username, const std::string& password);
    std::string getRoleByUsername(const std::string& username);

    // CRUD for buku
    bool insertBuku(const std::string& judul, const std::string& penulis, const std::string& status);
    bool updateBuku(int id_buku, const std::string& judul, const std::string& penulis, const std::string& status);
    bool deleteBuku(int id_buku);
    std::vector<std::vector<std::string>> searchBuku(const std::string& keyword);
    std::vector<std::vector<std::string>> getAllBuku();
    std::vector<std::vector<std::string>> getAvailableBuku();

    // CRUD for peminjaman
    bool insertPeminjaman(int id_buku, int id_anggota, const std::string& tanggal_pinjam, const std::string& tanggal_kembali, const std::string& status);
    bool updatePeminjaman(int id_peminjaman, const std::string& status, const std::string& tanggal_kembali);
    std::vector<std::vector<std::string>> getPeminjamanByAnggota(int id_anggota);
    std::vector<std::vector<std::string>> getAllPeminjaman();

    // CRUD for denda
    bool insertDenda(int id_peminjaman, float jumlah_denda, const std::string& status_pembayaran);
    bool updateDenda(int id_denda, const std::string& status_pembayaran);
    std::vector<std::vector<std::string>> getDendaByPeminjaman(int id_peminjaman);

    // CRUD for wishlist
    bool insertWishlist(int id_anggota, int id_buku);
    bool deleteWishlist(int id_anggota, int id_buku);
    std::vector<int> getWishlist(int id_anggota);

    // Anggota actions
    bool pinjamBuku(int id_buku, const std::string& id_anggota);
    bool kembalikanBuku(int id_peminjaman);
    bool tambahWishlist(const std::string& id_anggota, int id_buku);
};

#endif // DATABASE_MANAGER_H 