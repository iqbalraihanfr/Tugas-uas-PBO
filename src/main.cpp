#include "DatabaseManager.h"
#include "Menu.h"
#include "Admin.h"
#include "Anggota.h"
#include <iostream>
#include <memory>

int main() {
    DatabaseManager db;
    if (!db.connect("localhost", "root", "", "library_schema")) {
        std::cerr << "Failed to connect to database." << std::endl;
        return 1;
    }

    Pengguna* user = nullptr;
    while (!user) {
        std::cout << "=== Login ===\n";
        std::string username, password;
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);
        user = db.validateLogin(username, password);
        if (!user) {
            std::cout << "Login gagal. Coba lagi.\n";
        }
    }

    // Menu branching
    if (dynamic_cast<Admin*>(user)) {
        Menu menu(static_cast<Admin*>(user));
        menu.run();
    } else if (dynamic_cast<Anggota*>(user)) {
        Menu menu(static_cast<Anggota*>(user));
        menu.run();
    }

    delete user;
    db.disconnect();
    return 0;
} 