#include "DatabaseManager.h"
#include <iostream>
#include "Pengguna.h"
#include "Admin.h"
#include "Anggota.h"

DatabaseManager::DatabaseManager() : conn(nullptr), connected(false) {
    conn = mysql_init(nullptr);
    if (!conn) {
        throw std::runtime_error("Failed to initialize MySQL connection");
    }
    
    // Set connection timeout
    int timeout = 10;
    mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
    
    // Enable auto-reconnect
    bool reconnect = true;
    mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect(const std::string& host, const std::string& user, 
                            const std::string& password, const std::string& database) {
    if (!conn) {
        std::cerr << "MySQL connection not initialized" << std::endl;
        return false;
    }
    
    // Try to connect with socket first (for local connections)
    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(),
                           database.c_str(), 0, "/tmp/mysql.sock", 0)) {
        // If socket connection fails, try TCP connection
        if (!mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(),
                               database.c_str(), 3306, nullptr, 0)) {
            std::cerr << "Failed to connect to MySQL: " << mysql_error(conn) << std::endl;
            return false;
        }
    }
    
    connected = true;
    std::cout << "Successfully connected to MySQL database: " << database << std::endl;
    return true;
}

void DatabaseManager::disconnect() {
    if (conn) {
        mysql_close(conn);
        conn = nullptr;
        connected = false;
    }
}

bool DatabaseManager::isConnected() const {
    return connected && conn != nullptr && mysql_ping(conn) == 0;
}

bool DatabaseManager::executeQuery(const std::string& query) {
    if (!isConnected()) {
        return false;
    }
    
    if (mysql_query(conn, query.c_str()) != 0) {
        return false;
    }
    
    return true;
}

MYSQL_RES* DatabaseManager::executeSelect(const std::string& query) {
    if (!isConnected()) {
        return nullptr;
    }
    
    if (mysql_query(conn, query.c_str()) != 0) {
        return nullptr;
    }
    
    return mysql_store_result(conn);
}

bool DatabaseManager::createTable(const std::string& tableName, const std::string& columns) {
    std::string query = "CREATE TABLE IF NOT EXISTS " + tableName + " (" + columns + ")";
    return executeQuery(query);
}

bool DatabaseManager::insertRecord(const std::string& table, const std::string& columns, const std::string& values) {
    std::string query = "INSERT INTO " + table + " (" + columns + ") VALUES (" + values + ")";
    return executeQuery(query);
}

bool DatabaseManager::updateRecord(const std::string& table, const std::string& setClause, const std::string& whereClause) {
    std::string query = "UPDATE " + table + " SET " + setClause;
    if (!whereClause.empty()) {
        query += " WHERE " + whereClause;
    }
    return executeQuery(query);
}

bool DatabaseManager::deleteRecord(const std::string& table, const std::string& whereClause) {
    std::string query = "DELETE FROM " + table;
    if (!whereClause.empty()) {
        query += " WHERE " + whereClause;
    }
    return executeQuery(query);
}

MYSQL_RES* DatabaseManager::selectRecords(const std::string& table, const std::string& columns, const std::string& whereClause) {
    std::string query = "SELECT " + columns + " FROM " + table;
    if (!whereClause.empty()) {
        query += " WHERE " + whereClause;
    }
    return executeSelect(query);
}

int DatabaseManager::getLastInsertId() {
    return mysql_insert_id(conn);
}

std::string DatabaseManager::escapeString(const std::string& str) {
    char* escaped = new char[str.length() * 2 + 1];
    mysql_real_escape_string(conn, escaped, str.c_str(), str.length());
    std::string result(escaped);
    delete[] escaped;
    return result;
}

std::string DatabaseManager::getLastError() const {
    return mysql_error(conn);
}

Pengguna* DatabaseManager::validateLogin(const std::string& username, const std::string& password) {
    std::string query = "SELECT id_pengguna, nama, username, password, role FROM pengguna WHERE username='" + escapeString(username) + "' AND password='" + escapeString(password) + "'";
    MYSQL_RES* result = executeSelect(query);
    if (!result) return nullptr;
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        mysql_free_result(result);
        return nullptr;
    }
    std::string id = row[0];
    std::string nama = row[1];
    std::string uname = row[2];
    std::string pwd = row[3];
    std::string role = row[4];
    Pengguna* user = nullptr;
    if (role == "admin") {
        user = new Admin(id, nama, uname, pwd, this);
    } else if (role == "anggota") {
        user = new Anggota(id, nama, uname, pwd, this);
    }
    mysql_free_result(result);
    return user;
}

std::string DatabaseManager::getRoleByUsername(const std::string& username) {
    std::string query = "SELECT role FROM pengguna WHERE username='" + escapeString(username) + "'";
    MYSQL_RES* result = executeSelect(query);
    if (!result) return "";
    MYSQL_ROW row = mysql_fetch_row(result);
    std::string role = row ? row[0] : "";
    mysql_free_result(result);
    return role;
}

bool DatabaseManager::insertBuku(const std::string& judul, const std::string& penulis, const std::string& status) {
    std::string query = "INSERT INTO buku (judul, penulis, status_ketersediaan) VALUES ('" + escapeString(judul) + "', '" + escapeString(penulis) + "', '" + escapeString(status) + "')";
    return executeQuery(query);
}

bool DatabaseManager::updateBuku(int id_buku, const std::string& judul, const std::string& penulis, const std::string& status) {
    std::string query = "UPDATE buku SET judul='" + escapeString(judul) + "', penulis='" + escapeString(penulis) + "', status_ketersediaan='" + escapeString(status) + "' WHERE id_buku=" + std::to_string(id_buku);
    return executeQuery(query);
}

bool DatabaseManager::deleteBuku(int id_buku) {
    std::string query = "DELETE FROM buku WHERE id_buku=" + std::to_string(id_buku);
    return executeQuery(query);
}

std::vector<std::vector<std::string>> DatabaseManager::searchBuku(const std::string& keyword) {
    std::vector<std::vector<std::string>> resultVec;
    std::string query = "SELECT id_buku, judul, penulis, status_ketersediaan FROM buku WHERE judul LIKE '%" + escapeString(keyword) + "%' OR penulis LIKE '%" + escapeString(keyword) + "%'";
    MYSQL_RES* result = executeSelect(query);
    if (!result) return resultVec;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        resultVec.push_back({row[0], row[1], row[2], row[3]});
    }
    mysql_free_result(result);
    return resultVec;
}

std::vector<std::vector<std::string>> DatabaseManager::getAllBuku() {
    std::vector<std::vector<std::string>> resultVec;
    std::string query = "SELECT id_buku, judul, penulis, status_ketersediaan FROM buku";
    MYSQL_RES* result = executeSelect(query);
    if (!result) return resultVec;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        resultVec.push_back({row[0], row[1], row[2], row[3]});
    }
    mysql_free_result(result);
    return resultVec;
}

bool DatabaseManager::insertPeminjaman(int id_buku, int id_anggota, const std::string& tanggal_pinjam, const std::string& tanggal_kembali, const std::string& status) {
    std::string query = "INSERT INTO peminjaman (id_buku, id_anggota, tanggal_pinjam, tanggal_kembali, status) VALUES (" + std::to_string(id_buku) + ", " + std::to_string(id_anggota) + ", '" + escapeString(tanggal_pinjam) + "', '" + escapeString(tanggal_kembali) + "', '" + escapeString(status) + "')";
    return executeQuery(query);
}

bool DatabaseManager::updatePeminjaman(int id_peminjaman, const std::string& status, const std::string& tanggal_kembali) {
    std::string query = "UPDATE peminjaman SET status='" + escapeString(status) + "', tanggal_kembali='" + escapeString(tanggal_kembali) + "' WHERE id_peminjaman=" + std::to_string(id_peminjaman);
    return executeQuery(query);
}

std::vector<std::vector<std::string>> DatabaseManager::getPeminjamanByAnggota(int id_anggota) {
    std::vector<std::vector<std::string>> resultVec;
    std::string query = "SELECT id_peminjaman, id_buku, tanggal_pinjam, tanggal_kembali, status FROM peminjaman WHERE id_anggota=" + std::to_string(id_anggota);
    MYSQL_RES* result = executeSelect(query);
    if (!result) return resultVec;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        resultVec.push_back({row[0], row[1], row[2], row[3], row[4]});
    }
    mysql_free_result(result);
    return resultVec;
}

std::vector<std::vector<std::string>> DatabaseManager::getAllPeminjaman() {
    std::vector<std::vector<std::string>> resultVec;
    std::string query = "SELECT id_peminjaman, id_buku, id_anggota, tanggal_pinjam, tanggal_kembali, status FROM peminjaman";
    MYSQL_RES* result = executeSelect(query);
    if (!result) return resultVec;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        resultVec.push_back({row[0], row[1], row[2], row[3], row[4], row[5]});
    }
    mysql_free_result(result);
    return resultVec;
}

bool DatabaseManager::insertDenda(int id_peminjaman, float jumlah_denda, const std::string& status_pembayaran) {
    std::string query = "INSERT INTO denda (id_peminjaman, jumlah_denda, status_pembayaran) VALUES (" + std::to_string(id_peminjaman) + ", " + std::to_string(jumlah_denda) + ", '" + escapeString(status_pembayaran) + "')";
    return executeQuery(query);
}

bool DatabaseManager::updateDenda(int id_denda, const std::string& status_pembayaran) {
    std::string query = "UPDATE denda SET status_pembayaran='" + escapeString(status_pembayaran) + "' WHERE id_denda=" + std::to_string(id_denda);
    return executeQuery(query);
}

std::vector<std::vector<std::string>> DatabaseManager::getDendaByPeminjaman(int id_peminjaman) {
    std::vector<std::vector<std::string>> resultVec;
    std::string query = "SELECT id_denda, jumlah_denda, status_pembayaran FROM denda WHERE id_peminjaman=" + std::to_string(id_peminjaman);
    MYSQL_RES* result = executeSelect(query);
    if (!result) return resultVec;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        resultVec.push_back({row[0], row[1], row[2]});
    }
    mysql_free_result(result);
    return resultVec;
}

bool DatabaseManager::insertWishlist(int id_anggota, int id_buku) {
    std::string query = "INSERT INTO wishlist (id_anggota, id_buku) VALUES (" + std::to_string(id_anggota) + ", " + std::to_string(id_buku) + ")";
    return executeQuery(query);
}

bool DatabaseManager::deleteWishlist(int id_anggota, int id_buku) {
    std::string query = "DELETE FROM wishlist WHERE id_anggota=" + std::to_string(id_anggota) + " AND id_buku=" + std::to_string(id_buku);
    return executeQuery(query);
}

std::vector<int> DatabaseManager::getWishlist(int id_anggota) {
    std::vector<int> resultVec;
    std::string query = "SELECT id_buku FROM wishlist WHERE id_anggota=" + std::to_string(id_anggota);
    MYSQL_RES* result = executeSelect(query);
    if (!result) return resultVec;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        resultVec.push_back(std::stoi(row[0]));
    }
    mysql_free_result(result);
    return resultVec;
}

bool DatabaseManager::pinjamBuku(int id_buku, const std::string& id_anggota) {
    if (!conn) return false;
    // Check if book is available
    std::string check = "SELECT status FROM buku WHERE id = " + std::to_string(id_buku);
    if (mysql_query(conn, check.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (!row || std::string(row[0]) != "tersedia") {
                mysql_free_result(res);
                return false;
            }
            mysql_free_result(res);
        }
    } else return false;
    // Insert peminjaman
    std::string query = "INSERT INTO peminjaman (id_buku, id_anggota, tanggal_pinjam, status) VALUES (" + std::to_string(id_buku) + ", '" + id_anggota + "', CURDATE(), 'dipinjam')";
    if (mysql_query(conn, query.c_str()) != 0) return false;
    // Update book status
    std::string update = "UPDATE buku SET status = 'dipinjam' WHERE id = " + std::to_string(id_buku);
    if (mysql_query(conn, update.c_str()) != 0) return false;
    return true;
}

bool DatabaseManager::kembalikanBuku(int id_peminjaman) {
    if (!conn) return false;
    // Get id_buku
    std::string get = "SELECT id_buku FROM peminjaman WHERE id = " + std::to_string(id_peminjaman);
    int id_buku = -1;
    if (mysql_query(conn, get.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row) id_buku = std::stoi(row[0]);
            mysql_free_result(res);
        }
    }
    if (id_buku == -1) return false;
    // Update peminjaman
    std::string update = "UPDATE peminjaman SET status = 'kembali', tanggal_kembali = CURDATE() WHERE id = " + std::to_string(id_peminjaman);
    if (mysql_query(conn, update.c_str()) != 0) return false;
    // Update buku
    std::string updateBuku = "UPDATE buku SET status = 'tersedia' WHERE id = " + std::to_string(id_buku);
    if (mysql_query(conn, updateBuku.c_str()) != 0) return false;
    return true;
}

bool DatabaseManager::tambahWishlist(const std::string& id_anggota, int id_buku) {
    if (!conn) return false;
    std::string query = "INSERT INTO wishlist (id_anggota, id_buku) VALUES ('" + id_anggota + "', " + std::to_string(id_buku) + ")";
    if (mysql_query(conn, query.c_str()) != 0) return false;
    return true;
} 