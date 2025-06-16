#ifndef LOAN_TRANSACTION_H
#define LOAN_TRANSACTION_H

#include <string>
#include <ctime>
#include "DatabaseManager.h"
#include "Book.h"
#include "Member.h"
#include <vector>

class LoanTransaction {
private:
    int id;
    int bookId;
    int memberId;
    std::string borrowDate;
    std::string dueDate;
    std::string returnDate;
    std::string status;
    DatabaseManager* db;

public:
    // Constructors
    LoanTransaction();
    LoanTransaction(DatabaseManager* db, int bookId, int memberId);
    LoanTransaction(DatabaseManager* db, int id, int bookId, int memberId, 
                   const std::string& borrowDate, const std::string& dueDate,
                   const std::string& returnDate, const std::string& status);

    // Getters
    int getId() const { return id; }
    int getBookId() const { return bookId; }
    int getMemberId() const { return memberId; }
    std::string getBorrowDate() const { return borrowDate; }
    std::string getDueDate() const { return dueDate; }
    std::string getReturnDate() const { return returnDate; }
    std::string getStatus() const { return status; }

    // Setters
    void setBookId(int bookId) { this->bookId = bookId; }
    void setMemberId(int memberId) { this->memberId = memberId; }
    void setBorrowDate(const std::string& borrowDate) { this->borrowDate = borrowDate; }
    void setDueDate(const std::string& dueDate) { this->dueDate = dueDate; }
    void setReturnDate(const std::string& returnDate) { this->returnDate = returnDate; }
    void setStatus(const std::string& status) { this->status = status; }

    // Database operations
    bool save();
    bool update();
    bool remove();
    static LoanTransaction* findById(DatabaseManager* db, int id);
    static std::vector<LoanTransaction> findAll(DatabaseManager* db);
    static std::vector<LoanTransaction> findByBookId(DatabaseManager* db, int bookId);
    static std::vector<LoanTransaction> findByMemberId(DatabaseManager* db, int memberId);
    static std::vector<LoanTransaction> findByStatus(DatabaseManager* db, const std::string& status);

    // Business logic
    bool borrowBook();
    bool returnBook();
    bool isOverdue() const;
    int getOverdueDays() const;

    // Display methods
    void display() const;
    static void displayAll(const std::vector<LoanTransaction>& transactions);

    // Validation
    bool isValid() const;
    bool isBorrowed() const { return status == "borrowed"; }
    bool isReturned() const { return status == "returned"; }
};

#endif // LOAN_TRANSACTION_H 