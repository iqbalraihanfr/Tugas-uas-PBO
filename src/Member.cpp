#include "Member.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

Member::Member() : db(nullptr), id(0), status("active") {}

Member::Member(DatabaseManager* db, const std::string& name, const std::string& email)
    : db(db), id(0), name(name), email(email), status("active") {}

Member::Member(DatabaseManager* db, int id, const std::string& name, const std::string& email, const std::string& status)
    : db(db), id(id), name(name), email(email), status(status) {}

// Copy constructor
Member::Member(const Member& other)
    : db(other.db), id(other.id), name(other.name), email(other.email), status(other.status) {}

// Copy assignment operator
Member& Member::operator=(const Member& other) {
    if (this != &other) {
        db = other.db;
        id = other.id;
        name = other.name;
        email = other.email;
        status = other.status;
    }
    return *this;
}

int Member::getId() const { return id; }
std::string Member::getName() const { return name; }
std::string Member::getEmail() const { return email; }
std::string Member::getStatus() const { return status; }

void Member::setName(const std::string& name) { this->name = name; }
void Member::setEmail(const std::string& email) { this->email = email; }
void Member::setStatus(const std::string& status) { this->status = status; }

bool Member::save() {
    if (!isValid()) return false;
    std::string columns = "name, email, status";
    std::string values = "'" + db->escapeString(name) + "', '" +
                        db->escapeString(email) + "', '" +
                        db->escapeString(status) + "'";
    if (db->insertRecord("members", columns, values)) {
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

bool Member::update() {
    if (!isValid() || id <= 0) return false;
    std::string setClause = "name = '" + db->escapeString(name) + "', " +
                           "email = '" + db->escapeString(email) + "', " +
                           "status = '" + db->escapeString(status) + "'";
    std::string whereClause = "id = " + std::to_string(id);
    return db->updateRecord("members", setClause, whereClause);
}

bool Member::remove() {
    if (id <= 0) return false;
    std::string whereClause = "id = " + std::to_string(id);
    return db->deleteRecord("members", whereClause);
}

Member* Member::findById(DatabaseManager* db, int id) {
    std::string query = "SELECT * FROM members WHERE id = " + std::to_string(id);
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            Member* member = new Member(db, std::stoi(row[0]), row[1], row[2], row[3]);
            mysql_free_result(result);
            return member;
        }
        mysql_free_result(result);
    }
    return nullptr;
}

std::vector<Member> Member::findAll(DatabaseManager* db) {
    std::vector<Member> members;
    std::string query = "SELECT * FROM members";
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            members.emplace_back(db, std::stoi(row[0]), row[1], row[2], row[3]);
        }
        mysql_free_result(result);
    }
    return members;
}

std::vector<Member> Member::searchByName(DatabaseManager* db, const std::string& name) {
    std::vector<Member> members;
    std::string query = "SELECT * FROM members WHERE name LIKE '%" + db->escapeString(name) + "%'";
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            members.emplace_back(db, std::stoi(row[0]), row[1], row[2], row[3]);
        }
        mysql_free_result(result);
    }
    return members;
}

std::vector<Member> Member::searchByEmail(DatabaseManager* db, const std::string& email) {
    std::vector<Member> members;
    std::string query = "SELECT * FROM members WHERE email LIKE '%" + db->escapeString(email) + "%'";
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            members.emplace_back(db, std::stoi(row[0]), row[1], row[2], row[3]);
        }
        mysql_free_result(result);
    }
    return members;
}

void Member::display() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Status: " << status << std::endl;
    std::cout << "------------------------" << std::endl;
}

void Member::displayAll(const std::vector<Member>& members) {
    for (const auto& member : members) {
        member.display();
    }
}

bool Member::isValid() const {
    return !name.empty() && !email.empty() && !status.empty();
} 