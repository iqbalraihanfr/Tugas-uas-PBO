#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

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

    // Private methods
    void initializeDatabase();
    void createTables();
    void checkConnection();

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
};

#endif // DATABASE_MANAGER_H 