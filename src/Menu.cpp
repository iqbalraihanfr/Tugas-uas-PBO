#include "Menu.h"
#include <iostream>
#include <limits>
#include <cstdlib>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

Menu::Menu(Admin* admin) : admin(admin), anggota(nullptr), db(nullptr), running(false) {}
Menu::Menu(Anggota* anggota) : admin(nullptr), anggota(anggota), db(nullptr), running(false) {}

void Menu::clearScreen() {
    std::system(CLEAR_SCREEN);
}

void Menu::waitForKey() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int Menu::getIntInput(const std::string& prompt) {
    int input;
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        try {
            input = std::stoi(line);
            break;
        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a number.\n";
        }
    }
    return input;
}

bool Menu::confirmAction(const std::string& message) {
    std::cout << message << " (y/n): ";
    std::string response;
    std::getline(std::cin, response);
    return response == "y" || response == "Y";
}

std::string Menu::getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

void Menu::run() {
    if (admin) runAdminMenu();
    else if (anggota) runAnggotaMenu();
}

void Menu::runAdminMenu() {
    running = true;
    while (running) {
        clearScreen();
        std::cout << "=== Admin Menu ===\n";
        std::cout << "1. Tambah Buku\n";
        std::cout << "2. Kelola Data Buku\n";
        std::cout << "3. Kelola Transaksi Peminjaman\n";
        std::cout << "0. Logout\n";
        int choice = getIntInput("Pilih menu: ");
        switch (choice) {
            case 1: admin->tambahBuku(); break;
            case 2: admin->kelolaDataBuku(); break;
            case 3: admin->kelolaTransaksiPeminjaman(); break;
            case 0: running = false; break;
            default: std::cout << "Pilihan tidak valid\n"; waitForKey();
        }
    }
}

void Menu::runAnggotaMenu() {
    running = true;
    while (running) {
        clearScreen();
        std::cout << "=== Anggota Menu ===\n";
        std::cout << "1. Cari Buku\n";
        std::cout << "2. Pinjam Buku\n";
        std::cout << "3. Kembalikan Buku\n";
        std::cout << "4. Tambah ke Wishlist\n";
        std::cout << "0. Logout\n";
        int choice = getIntInput("Pilih menu: ");
        switch (choice) {
            case 1: anggota->cariBuku(); waitForKey(); break;
            case 2: anggota->pinjamBuku(); waitForKey(); break;
            case 3: anggota->kembalikanBuku(); waitForKey(); break;
            case 4: anggota->tambahKeWishlist(); waitForKey(); break;
            case 0: running = false; break;
            default: std::cout << "Pilihan tidak valid\n"; waitForKey();
        }
    }
} 