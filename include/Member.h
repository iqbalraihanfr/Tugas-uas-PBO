#ifndef MEMBER_H
#define MEMBER_H

#include "DatabaseManager.h"
#include <string>
#include <vector>

class Member {
private:
    int id;
    std::string name;
    std::string email;
    std::string status;
    DatabaseManager* db;  // Changed from reference to pointer

public:
    // Constructors
    Member();
    Member(DatabaseManager* db, const std::string& name, const std::string& email);
    Member(DatabaseManager* db, int id, const std::string& name, const std::string& email, 
           const std::string& status = "active");

    // Copy constructor and assignment operator
    Member(const Member& other);
    Member& operator=(const Member& other);

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getStatus() const;

    // Setters
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setStatus(const std::string& status);

    // Database operations
    bool save();
    bool update();
    bool remove();
    static Member* findById(DatabaseManager* db, int id);
    static std::vector<Member> findAll(DatabaseManager* db);
    static std::vector<Member> searchByName(DatabaseManager* db, const std::string& name);
    static std::vector<Member> searchByEmail(DatabaseManager* db, const std::string& email);

    // Display methods
    void display() const;
    static void displayAll(const std::vector<Member>& members);

    // Validation
    bool isValid() const;
    bool isActive() const { return status == "active"; }
    void activate() { status = "active"; }
    void deactivate() { status = "inactive"; }
};

#endif // MEMBER_H 