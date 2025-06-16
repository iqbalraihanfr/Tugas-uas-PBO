#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include "DatabaseManager.h"
#include <string>
#include <vector>

class Librarian {
private:
    int id;
    std::string username;
    std::string password;
    std::string name;
    std::string email;
    DatabaseManager* db;  // Changed from reference to pointer

public:
    // Constructors
    Librarian();
    Librarian(DatabaseManager* db, const std::string& username, const std::string& password,
              const std::string& name, const std::string& email);
    Librarian(DatabaseManager* db, int id, const std::string& username, const std::string& password,
              const std::string& name, const std::string& email);

    // Getters
    int getId() const;
    std::string getUsername() const;
    std::string getName() const;
    std::string getEmail() const;

    // Setters
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
    void setName(const std::string& name);
    void setEmail(const std::string& email);

    // Database operations
    bool save();
    bool update();
    bool remove();
    static Librarian* findById(DatabaseManager* db, int id);
    static Librarian* findByUsername(DatabaseManager* db, const std::string& username);
    static std::vector<Librarian> findAll(DatabaseManager* db);

    // Authentication methods
    bool authenticate(const std::string& password) const;
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);

    // Display methods
    void display() const;
    static void displayAll(const std::vector<Librarian>& librarians);

    // Validation
    bool isValid() const;
};

#endif // LIBRARIAN_H 