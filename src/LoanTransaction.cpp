#include "LoanTransaction.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <stdexcept>

// Constructors
LoanTransaction::LoanTransaction() : db(nullptr), id(0), bookId(0), memberId(0), status("borrowed") {}

LoanTransaction::LoanTransaction(DatabaseManager* db, int bookId, int memberId)
    : db(db), id(0), bookId(bookId), memberId(memberId), status("borrowed") {
    // Set borrowDate to today
    time_t now = time(0);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    borrowDate = buf;
    // Set dueDate to 7 days from now
    now += 7 * 24 * 60 * 60;
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    dueDate = buf;
    returnDate = "";
}

LoanTransaction::LoanTransaction(DatabaseManager* db, int id, int bookId, int memberId,
    const std::string& borrowDate, const std::string& dueDate,
    const std::string& returnDate, const std::string& status)
    : db(db), id(id), bookId(bookId), memberId(memberId), borrowDate(borrowDate), dueDate(dueDate), returnDate(returnDate), status(status) {}

// Database operations
bool LoanTransaction::save() {
    if (!isValid()) return false;
    std::string columns = "book_id, member_id, borrow_date, due_date, return_date, status";
    std::string values = std::to_string(bookId) + ", " + std::to_string(memberId) + ", '" +
                        borrowDate + "', '" + dueDate + "', '" + returnDate + "', '" + status + "'";
    if (db->insertRecord("transactions", columns, values)) {
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

bool LoanTransaction::update() {
    if (!isValid() || id <= 0) return false;
    std::string setClause = "book_id = " + std::to_string(bookId) + ", " +
                           "member_id = " + std::to_string(memberId) + ", " +
                           "borrow_date = '" + borrowDate + "', " +
                           "due_date = '" + dueDate + "', " +
                           "return_date = '" + returnDate + "', " +
                           "status = '" + status + "'";
    std::string whereClause = "id = " + std::to_string(id);
    return db->updateRecord("transactions", setClause, whereClause);
}

bool LoanTransaction::remove() {
    if (id <= 0) return false;
    std::string whereClause = "id = " + std::to_string(id);
    return db->deleteRecord("transactions", whereClause);
}

LoanTransaction* LoanTransaction::findById(DatabaseManager* db, int id) {
    std::string query = "SELECT * FROM transactions WHERE id = " + std::to_string(id);
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            LoanTransaction* transaction = new LoanTransaction(
                db,
                std::stoi(row[0]), std::stoi(row[1]), std::stoi(row[2]),
                row[3], row[4], row[5], row[6]
            );
            mysql_free_result(result);
            return transaction;
        }
        mysql_free_result(result);
    }
    return nullptr;
}

std::vector<LoanTransaction> LoanTransaction::findAll(DatabaseManager* db) {
    std::vector<LoanTransaction> transactions;
    std::string query = "SELECT * FROM transactions";
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            transactions.emplace_back(db, std::stoi(row[0]), std::stoi(row[1]), std::stoi(row[2]), row[3], row[4], row[5], row[6]);
        }
        mysql_free_result(result);
    }
    return transactions;
}

std::vector<LoanTransaction> LoanTransaction::findByBookId(DatabaseManager* db, int bookId) {
    std::vector<LoanTransaction> transactions;
    std::string query = "SELECT * FROM transactions WHERE book_id = " + std::to_string(bookId);
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            transactions.emplace_back(db, std::stoi(row[0]), std::stoi(row[1]), std::stoi(row[2]), row[3], row[4], row[5], row[6]);
        }
        mysql_free_result(result);
    }
    return transactions;
}

std::vector<LoanTransaction> LoanTransaction::findByMemberId(DatabaseManager* db, int memberId) {
    std::vector<LoanTransaction> transactions;
    std::string query = "SELECT * FROM transactions WHERE member_id = " + std::to_string(memberId);
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            transactions.emplace_back(db, std::stoi(row[0]), std::stoi(row[1]), std::stoi(row[2]), row[3], row[4], row[5], row[6]);
        }
        mysql_free_result(result);
    }
    return transactions;
}

std::vector<LoanTransaction> LoanTransaction::findByStatus(DatabaseManager* db, const std::string& status) {
    std::vector<LoanTransaction> transactions;
    std::string query = "SELECT * FROM transactions WHERE status = '" + status + "'";
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            transactions.emplace_back(db, std::stoi(row[0]), std::stoi(row[1]), std::stoi(row[2]), row[3], row[4], row[5], row[6]);
        }
        mysql_free_result(result);
    }
    return transactions;
}

// Display methods
void LoanTransaction::display() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Book ID: " << bookId << std::endl;
    std::cout << "Member ID: " << memberId << std::endl;
    std::cout << "Borrow Date: " << borrowDate << std::endl;
    std::cout << "Due Date: " << dueDate << std::endl;
    std::cout << "Return Date: " << returnDate << std::endl;
    std::cout << "Status: " << status << std::endl;
    std::cout << "------------------------" << std::endl;
}

void LoanTransaction::displayAll(const std::vector<LoanTransaction>& transactions) {
    for (const auto& transaction : transactions) {
        transaction.display();
    }
}

// Validation
bool LoanTransaction::isValid() const {
    return bookId > 0 && memberId > 0 && !borrowDate.empty() && !dueDate.empty() && !status.empty();
}

bool LoanTransaction::borrowBook() {
    // Check if book is available
    std::string whereClause = "book_id = " + std::to_string(bookId) + 
                             " AND status = 'borrowed'";
    MYSQL_RES* result = db->selectRecords("transactions", "*", whereClause);
    if (result) {
        if (mysql_num_rows(result) > 0) {
            mysql_free_result(result);
            return false; // Book is already borrowed
        }
        mysql_free_result(result);
    }

    return save();
}

bool LoanTransaction::returnBook() {
    if (id <= 0 || status == "returned") {
        return false;
    }

    // Set return date to current date
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::stringstream ss;
    ss << (1900 + ltm->tm_year) << "-"
       << std::setw(2) << std::setfill('0') << (1 + ltm->tm_mon) << "-"
       << std::setw(2) << std::setfill('0') << ltm->tm_mday;
    returnDate = ss.str();
    status = "returned";

    return update();
}

bool LoanTransaction::isOverdue() const {
    if (status == "returned") {
        return false;
    }

    // Calculate days between borrow date and current date
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    // Parse borrow date
    int borrowYear, borrowMonth, borrowDay;
    sscanf(borrowDate.c_str(), "%d-%d-%d", &borrowYear, &borrowMonth, &borrowDay);
    
    // Calculate days difference
    int days = (1900 + ltm->tm_year - borrowYear) * 365 +
               (1 + ltm->tm_mon - borrowMonth) * 30 +
               (ltm->tm_mday - borrowDay);
    
    return days > 14; // Overdue if more than 14 days
}

int LoanTransaction::getOverdueDays() const {
    if (!isOverdue()) {
        return 0;
    }

    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    // Parse borrow date
    int borrowYear, borrowMonth, borrowDay;
    sscanf(borrowDate.c_str(), "%d-%d-%d", &borrowYear, &borrowMonth, &borrowDay);
    
    // Calculate days difference
    int days = (1900 + ltm->tm_year - borrowYear) * 365 +
               (1 + ltm->tm_mon - borrowMonth) * 30 +
               (ltm->tm_mday - borrowDay);
    
    return days - 14; // Return number of days overdue
} 