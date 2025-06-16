#ifndef BOOK_H
#define BOOK_H

#include "DatabaseManager.h"
#include <string>
#include <vector>

class Book {
private:
    int id;
    std::string title;
    std::string author;
    std::string isbn;
    int quantity;
    DatabaseManager* db;  // Changed from reference to pointer

public:
    // Constructors
    Book();
    Book(DatabaseManager* db, const std::string& title, const std::string& author, 
         const std::string& isbn, int quantity);
    Book(DatabaseManager* db, int id, const std::string& title, const std::string& author, 
         const std::string& isbn, int quantity);

    // Copy constructor and assignment operator
    Book(const Book& other);
    Book& operator=(const Book& other);

    // Getters
    int getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getIsbn() const;
    int getQuantity() const;

    // Setters
    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setIsbn(const std::string& isbn);
    void setQuantity(int quantity);

    // Database operations
    bool save();
    bool update();
    bool remove();
    static Book* findById(DatabaseManager* db, int id);
    static std::vector<Book> findAll(DatabaseManager* db);
    static std::vector<Book> searchByTitle(DatabaseManager* db, const std::string& title);
    static std::vector<Book> searchByAuthor(DatabaseManager* db, const std::string& author);
    static std::vector<Book> searchByIsbn(DatabaseManager* db, const std::string& isbn);

    // Display methods
    void display() const;
    static void displayAll(const std::vector<Book>& books);

    // Validation
    bool isValid() const;
};

#endif // BOOK_H 