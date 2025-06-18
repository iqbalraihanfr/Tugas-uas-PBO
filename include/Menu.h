#ifndef MENU_H
#define MENU_H

#include "Admin.h"
#include "Anggota.h"
#include "DatabaseManager.h"
#include <string>
#include <vector>

class Menu {
private:
    Admin* admin;
    Anggota* anggota;
    DatabaseManager* db;
    bool running;

    // Helper methods
    void clearScreen();
    void waitForKey();
    bool confirmAction(const std::string& message);
    std::string getInput(const std::string& prompt);
    int getIntInput(const std::string& prompt);

    // Menu methods
    void showLoginMenu();
    void showMainMenu();
    void showBookMenu();
    void showMemberMenu();
    void showTransactionMenu();
    void showReportMenu();

    // Book operations
    void addBook();
    void updateBook();
    void deleteBook();
    void searchBooks();
    void listBooks();

    // Member operations
    void addMember();
    void updateMember();
    void deleteMember();
    void searchMembers();
    void listMembers();

    // Transaction operations
    void borrowBook();
    void returnBook();
    void listTransactions();
    void listOverdueBooks();

    // Report operations
    void showStatistics();
    void exportToCSV();

public:
    Menu(Admin* admin);
    Menu(Anggota* anggota);
    void run();
    void runAdminMenu();
    void runAnggotaMenu();
};

#endif // MENU_H 