#include "LibrarySystem.h"
#include <iostream>

LibrarySystem::LibrarySystem() {
    db = std::make_unique<DatabaseManager>();
    if (!initializeDatabase()) {
        throw std::runtime_error("Failed to initialize database");
    }
}

LibrarySystem::~LibrarySystem() {
    logout();
}

bool LibrarySystem::initializeDatabase() {
    if (!db->connect("localhost", "root", "", "library_system")) {
        throw std::runtime_error("Failed to connect to database");
    }

    // Create all necessary tables
    db->createTables();

    // Create default librarian if none exists
    return createDefaultLibrarian();
}

bool LibrarySystem::createDefaultLibrarian() {
    // Check if any librarian exists
    std::string query = "SELECT COUNT(*) FROM librarians";
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row && std::stoi(row[0]) == 0) {
            // Create default librarian
            Librarian librarian(db.get(), "admin", "admin123", "Administrator", "admin@library.com");
            return librarian.save();
        }
        mysql_free_result(result);
    }
    return true;
}

bool LibrarySystem::login(const std::string& username, const std::string& password) {
    if (isLoggedIn()) {
        logout();
    }

    Librarian* librarian = Librarian::findByUsername(db.get(), username);
    if (librarian && librarian->authenticate(password)) {
        currentLibrarian.reset(librarian);
        return true;
    }
    delete librarian;
    return false;
}

void LibrarySystem::logout() {
    currentLibrarian.reset();
}

bool LibrarySystem::isLoggedIn() const {
    return currentLibrarian != nullptr;
}

Librarian* LibrarySystem::getCurrentLibrarian() const {
    return currentLibrarian.get();
}

bool LibrarySystem::addBook(const std::string& title, const std::string& author, const std::string& isbn, int quantity) {
    if (!isLoggedIn()) return false;
    Book book(db.get(), title, author, isbn, quantity);
    return book.save();
}

bool LibrarySystem::updateBook(int id, const std::string& title, const std::string& author, const std::string& isbn, int quantity) {
    if (!isLoggedIn()) return false;
    Book* book = Book::findById(db.get(), id);
    if (!book) return false;
    book->setTitle(title);
    book->setAuthor(author);
    book->setIsbn(isbn);
    book->setQuantity(quantity);
    bool result = book->update();
    delete book;
    return result;
}

bool LibrarySystem::deleteBook(int id) {
    if (!isLoggedIn()) return false;

    Book* book = Book::findById(db.get(), id);
    if (!book) return false;

    bool success = book->remove();
    delete book;
    return success;
}

Book* LibrarySystem::findBook(int id) {
    return Book::findById(db.get(), id);
}

std::vector<Book> LibrarySystem::searchBooks(const std::string& query) {
    std::vector<Book> results;
    
    // Search by title
    auto titleResults = Book::searchByTitle(db.get(), query);
    results.insert(results.end(), titleResults.begin(), titleResults.end());
    
    // Search by author
    auto authorResults = Book::searchByAuthor(db.get(), query);
    results.insert(results.end(), authorResults.begin(), authorResults.end());
    
    // Search by isbn
    auto isbnResults = Book::searchByIsbn(db.get(), query);
    results.insert(results.end(), isbnResults.begin(), isbnResults.end());
    
    return results;
}

std::vector<Book> LibrarySystem::getAllBooks() {
    return Book::findAll(db.get());
}

bool LibrarySystem::addMember(const std::string& name, const std::string& email) {
    if (!isLoggedIn()) return false;

    Member member(db.get(), name, email);
    return member.save();
}

bool LibrarySystem::updateMember(int id, const std::string& name, const std::string& email) {
    if (!isLoggedIn()) return false;

    Member* member = Member::findById(db.get(), id);
    if (!member) return false;

    member->setName(name);
    member->setEmail(email);

    bool success = member->update();
    delete member;
    return success;
}

bool LibrarySystem::deleteMember(int id) {
    if (!isLoggedIn()) return false;

    Member* member = Member::findById(db.get(), id);
    if (!member) return false;

    bool success = member->remove();
    delete member;
    return success;
}

Member* LibrarySystem::findMember(int id) {
    return Member::findById(db.get(), id);
}

std::vector<Member> LibrarySystem::searchMembers(const std::string& query) {
    std::vector<Member> results;
    
    // Search by name
    auto nameResults = Member::searchByName(db.get(), query);
    results.insert(results.end(), nameResults.begin(), nameResults.end());
    
    // Search by email
    auto emailResults = Member::searchByEmail(db.get(), query);
    results.insert(results.end(), emailResults.begin(), emailResults.end());
    
    return results;
}

std::vector<Member> LibrarySystem::getAllMembers() {
    return Member::findAll(db.get());
}

bool LibrarySystem::borrowBook(int bookId, int memberId) {
    if (!isLoggedIn()) return false;

    // Check if book exists and is available
    Book* book = Book::findById(db.get(), bookId);
    if (!book) return false;

    // Check if member exists and is active
    Member* member = Member::findById(db.get(), memberId);
    if (!member || !member->isActive()) {
        delete book;
        delete member;
        return false;
    }

    // Create and save transaction
    LoanTransaction transaction(db.get(), bookId, memberId);
    bool success = transaction.borrowBook();

    delete book;
    delete member;
    return success;
}

bool LibrarySystem::returnBook(int transactionId) {
    if (!isLoggedIn()) return false;

    LoanTransaction* transaction = LoanTransaction::findById(db.get(), transactionId);
    if (!transaction) return false;

    transaction->setStatus("returned");
    // Set return date to today
    {
        time_t now = time(0);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
        transaction->setReturnDate(buf);
    }
    bool success = transaction->update();
    delete transaction;
    return success;
}

std::vector<LoanTransaction> LibrarySystem::getMemberTransactions(int memberId) {
    return LoanTransaction::findByMemberId(db.get(), memberId);
}

std::vector<LoanTransaction> LibrarySystem::getOverdueTransactions() {
    std::vector<LoanTransaction> all = LoanTransaction::findByStatus(db.get(), "borrowed");
    std::vector<LoanTransaction> overdue;
    
    for (const auto& t : all) {
        if (t.isOverdue()) {
            overdue.push_back(t);
        }
    }
    
    return overdue;
}

std::vector<LoanTransaction> LibrarySystem::getAllTransactions() {
    return LoanTransaction::findAll(db.get());
}

int LibrarySystem::getTotalBooks() const {
    MYSQL_RES* result = db->executeSelect("SELECT COUNT(*) FROM books");
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            int count = std::stoi(row[0]);
            mysql_free_result(result);
            return count;
        }
        mysql_free_result(result);
    }
    return 0;
}

int LibrarySystem::getTotalMembers() const {
    MYSQL_RES* result = db->executeSelect("SELECT COUNT(*) FROM members");
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            int count = std::stoi(row[0]);
            mysql_free_result(result);
            return count;
        }
        mysql_free_result(result);
    }
    return 0;
}

int LibrarySystem::getTotalBorrowedBooks() const {
    MYSQL_RES* result = db->executeSelect("SELECT COUNT(*) FROM transactions WHERE status = 'borrowed'");
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            int count = std::stoi(row[0]);
            mysql_free_result(result);
            return count;
        }
        mysql_free_result(result);
    }
    return 0;
}

int LibrarySystem::getTotalOverdueBooks() const {
    std::vector<LoanTransaction> overdueTransactions = const_cast<LibrarySystem*>(this)->getOverdueTransactions();
    return overdueTransactions.size();
} 