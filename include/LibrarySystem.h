#ifndef LIBRARY_SYSTEM_H
#define LIBRARY_SYSTEM_H

#include <memory>
#include "DatabaseManager.h"
#include "Book.h"
#include "Member.h"
#include "Librarian.h"
#include "LoanTransaction.h"
#include <vector>

class LibrarySystem {
private:
    std::unique_ptr<DatabaseManager> db;
    std::unique_ptr<Librarian> currentLibrarian;

    // Helper methods
    bool initializeDatabase();
    bool createDefaultLibrarian();

public:
    // Constructor & Destructor
    LibrarySystem();
    ~LibrarySystem();

    // Authentication
    bool login(const std::string& username, const std::string& password);
    void logout();
    bool isLoggedIn() const;
    Librarian* getCurrentLibrarian() const;

    // Book management
    bool addBook(const std::string& title, const std::string& author, const std::string& isbn, int quantity);
    bool updateBook(int id, const std::string& title, const std::string& author, const std::string& isbn, int quantity);
    bool deleteBook(int id);
    Book* findBook(int id);
    std::vector<Book> searchBooks(const std::string& query);
    std::vector<Book> getAllBooks();

    // Member management
    bool addMember(const std::string& name, const std::string& email);
    bool updateMember(int id, const std::string& name, const std::string& email);
    bool deleteMember(int id);
    Member* findMember(int id);
    std::vector<Member> searchMembers(const std::string& query);
    std::vector<Member> getAllMembers();

    // Transaction management
    bool borrowBook(int bookId, int memberId);
    bool returnBook(int transactionId);
    std::vector<LoanTransaction> getMemberTransactions(int memberId);
    std::vector<LoanTransaction> getOverdueTransactions();
    std::vector<LoanTransaction> getAllTransactions();

    // Statistics
    int getTotalBooks() const;
    int getTotalMembers() const;
    int getTotalBorrowedBooks() const;
    int getTotalOverdueBooks() const;
};

#endif // LIBRARY_SYSTEM_H 