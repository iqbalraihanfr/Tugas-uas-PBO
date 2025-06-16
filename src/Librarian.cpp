#include "Librarian.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <stdexcept>

Librarian::Librarian() : db(nullptr), id(0) {}

Librarian::Librarian(DatabaseManager* db, const std::string& username, const std::string& password,
                     const std::string& name, const std::string& email)
    : db(db), id(0), username(username), password(password), name(name), email(email) {}

Librarian::Librarian(DatabaseManager* db, int id, const std::string& username, const std::string& password,
                     const std::string& name, const std::string& email)
    : db(db), id(id), username(username), password(password), name(name), email(email) {}

bool Librarian::save() {
    if (!isValid()) {
        return false;
    }

    std::string columns = "username, password, name, email";
    std::string values = "'" + db->escapeString(username) + "', '" +
                        db->escapeString(password) + "', '" +
                        db->escapeString(name) + "', '" +
                        db->escapeString(email) + "'";

    if (db->insertRecord("librarians", columns, values)) {
        MYSQL_RES* result = db->executeSelect("SELECT LAST_INSERT_ID()");
        if (result) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row) {
                id = std::stoi(row[0]);
            }
            mysql_free_result(result);
            return true;
        }
    }
    return false;
}

bool Librarian::update() {
    if (!isValid() || id <= 0) {
        return false;
    }

    std::string setClause = "username = '" + db->escapeString(username) + "', " +
                           "password = '" + db->escapeString(password) + "', " +
                           "name = '" + db->escapeString(name) + "', " +
                           "email = '" + db->escapeString(email) + "'";
    std::string whereClause = "id = " + std::to_string(id);

    return db->updateRecord("librarians", setClause, whereClause);
}

bool Librarian::remove() {
    if (id <= 0) {
        return false;
    }

    std::string whereClause = "id = " + std::to_string(id);
    return db->deleteRecord("librarians", whereClause);
}

Librarian* Librarian::findById(DatabaseManager* db, int id) {
    std::string query = "SELECT * FROM librarians WHERE id = " + std::to_string(id);
    MYSQL_RES* result = db->executeSelect(query);
    
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            Librarian* librarian = new Librarian(db, std::stoi(row[0]), row[1], row[2], row[3], row[4]);
            mysql_free_result(result);
            return librarian;
        }
        mysql_free_result(result);
    }
    return nullptr;
}

Librarian* Librarian::findByUsername(DatabaseManager* db, const std::string& username) {
    std::string query = "SELECT * FROM librarians WHERE username = '" + db->escapeString(username) + "'";
    MYSQL_RES* result = db->executeSelect(query);
    
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            Librarian* librarian = new Librarian(db, std::stoi(row[0]), row[1], row[2], row[3], row[4]);
            mysql_free_result(result);
            return librarian;
        }
        mysql_free_result(result);
    }
    return nullptr;
}

std::vector<Librarian> Librarian::findAll(DatabaseManager* db) {
    std::vector<Librarian> librarians;
    std::string query = "SELECT * FROM librarians";
    MYSQL_RES* result = db->executeSelect(query);
    
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            librarians.emplace_back(db, std::stoi(row[0]), row[1], row[2], row[3], row[4]);
        }
        mysql_free_result(result);
    }
    return librarians;
}

bool Librarian::authenticate(const std::string& password) const {
    return this->password == password;
}

bool Librarian::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (this->password != oldPassword) return false;
    this->password = newPassword;
    return update();
}

void Librarian::display() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Username: " << username << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "------------------------" << std::endl;
}

void Librarian::displayAll(const std::vector<Librarian>& librarians) {
    for (const auto& librarian : librarians) {
        librarian.display();
    }
}

bool Librarian::isValid() const {
    return !username.empty() && !password.empty() && !name.empty() && !email.empty();
} 