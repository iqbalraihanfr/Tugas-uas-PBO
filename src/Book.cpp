#include "Book.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

// Constructors
Book::Book() : db(nullptr), id(0), quantity(0) {}

Book::Book(DatabaseManager* db, const std::string& title, const std::string& author,
           const std::string& isbn, int quantity)
    : db(db), id(0), title(title), author(author), isbn(isbn), quantity(quantity) {}

Book::Book(DatabaseManager* db, int id, const std::string& title, const std::string& author,
           const std::string& isbn, int quantity)
    : db(db), id(id), title(title), author(author), isbn(isbn), quantity(quantity) {}

// Copy constructor
Book::Book(const Book& other)
    : db(other.db), id(other.id), title(other.title), author(other.author),
      isbn(other.isbn), quantity(other.quantity) {}

// Copy assignment operator
Book& Book::operator=(const Book& other) {
    if (this != &other) {
        db = other.db;
        id = other.id;
        title = other.title;
        author = other.author;
        isbn = other.isbn;
        quantity = other.quantity;
    }
    return *this;
}

// Getters
int Book::getId() const { return id; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getIsbn() const { return isbn; }
int Book::getQuantity() const { return quantity; }

// Setters
void Book::setTitle(const std::string& title) { this->title = title; }
void Book::setAuthor(const std::string& author) { this->author = author; }
void Book::setIsbn(const std::string& isbn) { this->isbn = isbn; }
void Book::setQuantity(int quantity) { this->quantity = quantity; }

// Database operations
bool Book::save() {
    if (!isValid()) return false;
    std::string columns = "title, author, isbn, quantity";
    std::string values = "'" + db->escapeString(title) + "', '" +
                        db->escapeString(author) + "', '" +
                        db->escapeString(isbn) + "', " +
                        std::to_string(quantity);
    if (db->insertRecord("books", columns, values)) {
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

bool Book::update() {
    if (!isValid() || id <= 0) return false;
    std::string setClause = "title = '" + db->escapeString(title) + "', " +
                           "author = '" + db->escapeString(author) + "', " +
                           "isbn = '" + db->escapeString(isbn) + "', " +
                           "quantity = " + std::to_string(quantity);
    std::string whereClause = "id = " + std::to_string(id);
    return db->updateRecord("books", setClause, whereClause);
}

bool Book::remove() {
    if (id <= 0) return false;
    std::string whereClause = "id = " + std::to_string(id);
    return db->deleteRecord("books", whereClause);
}

Book* Book::findById(DatabaseManager* db, int id) {
    std::string query = "SELECT * FROM books WHERE id = " + std::to_string(id);
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            Book* book = new Book(db, std::stoi(row[0]), row[1], row[2], row[3], std::stoi(row[4]));
            mysql_free_result(result);
            return book;
        }
        mysql_free_result(result);
    }
    return nullptr;
}

std::vector<Book> Book::findAll(DatabaseManager* db) {
    std::vector<Book> books;
    std::string query = "SELECT * FROM books";
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            books.emplace_back(db, std::stoi(row[0]), row[1], row[2], row[3], std::stoi(row[4]));
        }
        mysql_free_result(result);
    }
    return books;
}

std::vector<Book> Book::searchByTitle(DatabaseManager* db, const std::string& title) {
    std::vector<Book> books;
    std::string query = "SELECT * FROM books WHERE title LIKE '%" + db->escapeString(title) + "%'";
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            books.emplace_back(db, std::stoi(row[0]), row[1], row[2], row[3], std::stoi(row[4]));
        }
        mysql_free_result(result);
    }
    return books;
}

std::vector<Book> Book::searchByAuthor(DatabaseManager* db, const std::string& author) {
    std::vector<Book> books;
    std::string query = "SELECT * FROM books WHERE author LIKE '%" + db->escapeString(author) + "%'";
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            books.emplace_back(db, std::stoi(row[0]), row[1], row[2], row[3], std::stoi(row[4]));
        }
        mysql_free_result(result);
    }
    return books;
}

std::vector<Book> Book::searchByIsbn(DatabaseManager* db, const std::string& isbn) {
    std::vector<Book> books;
    std::string query = "SELECT * FROM books WHERE isbn LIKE '%" + db->escapeString(isbn) + "%'";
    MYSQL_RES* result = db->executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            books.emplace_back(db, std::stoi(row[0]), row[1], row[2], row[3], std::stoi(row[4]));
        }
        mysql_free_result(result);
    }
    return books;
}

// Display methods
void Book::display() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Title: " << title << std::endl;
    std::cout << "Author: " << author << std::endl;
    std::cout << "ISBN: " << isbn << std::endl;
    std::cout << "Quantity: " << quantity << std::endl;
    std::cout << "------------------------" << std::endl;
}

void Book::displayAll(const std::vector<Book>& books) {
    for (const auto& book : books) {
        book.display();
    }
}

// Validation
bool Book::isValid() const {
    return !title.empty() && !author.empty() && !isbn.empty() && quantity >= 0;
} 