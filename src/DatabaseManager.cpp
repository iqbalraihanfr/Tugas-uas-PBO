#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager() : conn(nullptr), connected(false) {
    conn = mysql_init(nullptr);
    if (!conn) {
        throw std::runtime_error("Failed to initialize MySQL connection");
    }
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect(const std::string& host, const std::string& user, 
                            const std::string& password, const std::string& database) {
    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(),
                           database.c_str(), 0, nullptr, 0)) {
        return false;
    }
    
    connected = true;
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

void DatabaseManager::createTables() {
    // Create books table
    std::string booksTable = "CREATE TABLE IF NOT EXISTS books ("
                            "id INT AUTO_INCREMENT PRIMARY KEY,"
                            "title VARCHAR(255) NOT NULL,"
                            "author VARCHAR(255) NOT NULL,"
                            "year INT,"
                            "category VARCHAR(100),"
                            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                            ")";
    
    // Create members table
    std::string membersTable = "CREATE TABLE IF NOT EXISTS members ("
                              "id INT AUTO_INCREMENT PRIMARY KEY,"
                              "name VARCHAR(255) NOT NULL,"
                              "email VARCHAR(255) UNIQUE NOT NULL,"
                              "status ENUM('active', 'inactive') DEFAULT 'active',"
                              "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                              ")";
    
    // Create transactions table
    std::string transactionsTable = "CREATE TABLE IF NOT EXISTS transactions ("
                                   "id INT AUTO_INCREMENT PRIMARY KEY,"
                                   "book_id INT NOT NULL,"
                                   "member_id INT NOT NULL,"
                                   "borrow_date DATE NOT NULL,"
                                   "return_date DATE,"
                                   "status ENUM('borrowed', 'returned') DEFAULT 'borrowed',"
                                   "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                                   "FOREIGN KEY (book_id) REFERENCES books(id),"
                                   "FOREIGN KEY (member_id) REFERENCES members(id)"
                                   ")";
    
    executeQuery(booksTable);
    executeQuery(membersTable);
    executeQuery(transactionsTable);
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