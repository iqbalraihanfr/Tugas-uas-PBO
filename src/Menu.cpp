#include "Menu.h"
#include <iostream>
#include <limits>
#include <cstdlib>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

Menu::Menu(LibrarySystem* system) : system(system), running(false) {}

void Menu::run() {
    running = true;
    while (running) {
        if (!system->isLoggedIn()) {
            showLoginMenu();
        } else {
            showMainMenu();
        }
    }
}

void Menu::clearScreen() {
    std::system(CLEAR_SCREEN);
}

void Menu::waitForKey() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

void Menu::showLoginMenu() {
    clearScreen();
    std::cout << "=== Library Management System ===\n\n";
    std::string username = getInput("Username: ");
    std::string password = getInput("Password: ");

    if (system->login(username, password)) {
        std::cout << "\nLogin successful!\n";
        waitForKey();
    } else {
        std::cout << "\nInvalid username or password.\n";
        waitForKey();
    }
}

void Menu::showMainMenu() {
    clearScreen();
    std::cout << "=== Main Menu ===\n\n";
    std::cout << "1. Book Management\n";
    std::cout << "2. Member Management\n";
    std::cout << "3. Transaction Management\n";
    std::cout << "4. Reports\n";
    std::cout << "5. Logout\n";
    std::cout << "0. Exit\n\n";

    int choice = getIntInput("Enter your choice: ");
    switch (choice) {
        case 1:
            showBookMenu();
            break;
        case 2:
            showMemberMenu();
            break;
        case 3:
            showTransactionMenu();
            break;
        case 4:
            showReportMenu();
            break;
        case 5:
            system->logout();
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << "Invalid choice.\n";
            waitForKey();
    }
}

void Menu::showBookMenu() {
    clearScreen();
    std::cout << "=== Book Management ===\n\n";
    std::cout << "1. Add Book\n";
    std::cout << "2. Update Book\n";
    std::cout << "3. Delete Book\n";
    std::cout << "4. Search Books\n";
    std::cout << "5. List All Books\n";
    std::cout << "0. Back to Main Menu\n\n";

    int choice = getIntInput("Enter your choice: ");
    switch (choice) {
        case 1:
            addBook();
            break;
        case 2:
            updateBook();
            break;
        case 3:
            deleteBook();
            break;
        case 4:
            searchBooks();
            break;
        case 5:
            listBooks();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid choice.\n";
    }
    waitForKey();
}

void Menu::showMemberMenu() {
    clearScreen();
    std::cout << "=== Member Management ===\n\n";
    std::cout << "1. Add Member\n";
    std::cout << "2. Update Member\n";
    std::cout << "3. Delete Member\n";
    std::cout << "4. Search Members\n";
    std::cout << "5. List All Members\n";
    std::cout << "0. Back to Main Menu\n\n";

    int choice = getIntInput("Enter your choice: ");
    switch (choice) {
        case 1:
            addMember();
            break;
        case 2:
            updateMember();
            break;
        case 3:
            deleteMember();
            break;
        case 4:
            searchMembers();
            break;
        case 5:
            listMembers();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid choice.\n";
    }
    waitForKey();
}

void Menu::showTransactionMenu() {
    clearScreen();
    std::cout << "=== Transaction Management ===\n\n";
    std::cout << "1. Borrow Book\n";
    std::cout << "2. Return Book\n";
    std::cout << "3. List All Transactions\n";
    std::cout << "4. List Overdue Books\n";
    std::cout << "0. Back to Main Menu\n\n";

    int choice = getIntInput("Enter your choice: ");
    switch (choice) {
        case 1:
            borrowBook();
            break;
        case 2:
            returnBook();
            break;
        case 3:
            listTransactions();
            break;
        case 4:
            listOverdueBooks();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid choice.\n";
    }
    waitForKey();
}

void Menu::showReportMenu() {
    clearScreen();
    std::cout << "=== Reports ===\n\n";
    std::cout << "1. Show Statistics\n";
    std::cout << "2. Export to CSV\n";
    std::cout << "0. Back to Main Menu\n\n";

    int choice = getIntInput("Enter your choice: ");
    switch (choice) {
        case 1:
            showStatistics();
            break;
        case 2:
            exportToCSV();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid choice.\n";
    }
    waitForKey();
}

// Book operations
void Menu::addBook() {
    clearScreen();
    std::cout << "=== Add New Book ===\n\n";
    std::string title = getInput("Title: ");
    std::string author = getInput("Author: ");
    std::string isbn = getInput("ISBN: ");
    int quantity = getIntInput("Quantity: ");

    if (system->addBook(title, author, isbn, quantity)) {
        std::cout << "\nBook added successfully!\n";
    } else {
        std::cout << "\nFailed to add book.\n";
    }
}

void Menu::updateBook() {
    clearScreen();
    std::cout << "=== Update Book ===\n\n";
    int id = getIntInput("Enter book ID: ");
    Book* book = system->findBook(id);
    if (!book) {
        std::cout << "Book not found.\n";
        return;
    }

    std::string title = getInput("New title [" + book->getTitle() + "]: ");
    if (title.empty()) title = book->getTitle();

    std::string author = getInput("New author [" + book->getAuthor() + "]: ");
    if (author.empty()) author = book->getAuthor();

    std::string isbn = getInput("New ISBN [" + book->getIsbn() + "]: ");
    if (isbn.empty()) isbn = book->getIsbn();

    int quantity = getIntInput("New quantity [" + std::to_string(book->getQuantity()) + "]: ");

    if (system->updateBook(id, title, author, isbn, quantity)) {
        std::cout << "\nBook updated successfully!\n";
    } else {
        std::cout << "\nFailed to update book.\n";
    }
    delete book;
}

void Menu::deleteBook() {
    clearScreen();
    std::cout << "=== Delete Book ===\n\n";
    int id = getIntInput("Enter book ID: ");
    if (confirmAction("Are you sure you want to delete this book?")) {
        if (system->deleteBook(id)) {
            std::cout << "\nBook deleted successfully!\n";
        } else {
            std::cout << "\nFailed to delete book.\n";
        }
    }
}

void Menu::searchBooks() {
    clearScreen();
    std::cout << "=== Search Books ===\n\n";
    std::string query = getInput("Enter search term: ");
    auto results = system->searchBooks(query);
    if (results.empty()) {
        std::cout << "No books found.\n";
    } else {
        std::cout << "\nFound " << results.size() << " books:\n\n";
        Book::displayAll(results);
    }
}

void Menu::listBooks() {
    clearScreen();
    std::cout << "=== All Books ===\n\n";
    auto books = system->getAllBooks();
    if (books.empty()) {
        std::cout << "No books in the library.\n";
    } else {
        Book::displayAll(books);
    }
}

// Member operations
void Menu::addMember() {
    clearScreen();
    std::cout << "=== Add New Member ===\n\n";
    std::string name = getInput("Name: ");
    std::string email = getInput("Email: ");

    if (system->addMember(name, email)) {
        std::cout << "\nMember added successfully!\n";
    } else {
        std::cout << "\nFailed to add member.\n";
    }
}

void Menu::updateMember() {
    clearScreen();
    std::cout << "=== Update Member ===\n\n";
    int id = getIntInput("Enter member ID: ");
    Member* member = system->findMember(id);
    if (!member) {
        std::cout << "Member not found.\n";
        return;
    }

    std::string name = getInput("New name [" + member->getName() + "]: ");
    if (name.empty()) name = member->getName();

    std::string email = getInput("New email [" + member->getEmail() + "]: ");
    if (email.empty()) email = member->getEmail();

    if (system->updateMember(id, name, email)) {
        std::cout << "\nMember updated successfully!\n";
    } else {
        std::cout << "\nFailed to update member.\n";
    }
    delete member;
}

void Menu::deleteMember() {
    clearScreen();
    std::cout << "=== Delete Member ===\n\n";
    int id = getIntInput("Enter member ID: ");
    if (confirmAction("Are you sure you want to delete this member?")) {
        if (system->deleteMember(id)) {
            std::cout << "\nMember deleted successfully!\n";
        } else {
            std::cout << "\nFailed to delete member.\n";
        }
    }
}

void Menu::searchMembers() {
    clearScreen();
    std::cout << "=== Search Members ===\n\n";
    std::string query = getInput("Enter search term: ");
    auto results = system->searchMembers(query);
    if (results.empty()) {
        std::cout << "No members found.\n";
    } else {
        std::cout << "\nFound " << results.size() << " members:\n\n";
        Member::displayAll(results);
    }
}

void Menu::listMembers() {
    clearScreen();
    std::cout << "=== All Members ===\n\n";
    auto members = system->getAllMembers();
    if (members.empty()) {
        std::cout << "No members in the library.\n";
    } else {
        Member::displayAll(members);
    }
}

// Transaction operations
void Menu::borrowBook() {
    clearScreen();
    std::cout << "=== Borrow Book ===\n\n";
    int bookId = getIntInput("Enter book ID: ");
    int memberId = getIntInput("Enter member ID: ");

    if (system->borrowBook(bookId, memberId)) {
        std::cout << "\nBook borrowed successfully!\n";
    } else {
        std::cout << "\nFailed to borrow book.\n";
    }
}

void Menu::returnBook() {
    clearScreen();
    std::cout << "=== Return Book ===\n\n";
    int transactionId = getIntInput("Enter transaction ID: ");

    if (system->returnBook(transactionId)) {
        std::cout << "\nBook returned successfully!\n";
    } else {
        std::cout << "\nFailed to return book.\n";
    }
}

void Menu::listTransactions() {
    clearScreen();
    std::cout << "=== All Transactions ===\n\n";
    auto transactions = system->getAllTransactions();
    if (transactions.empty()) {
        std::cout << "No transactions found.\n";
    } else {
        for (const auto& t : transactions) {
            t.display();
        }
    }
}

void Menu::listOverdueBooks() {
    clearScreen();
    std::cout << "=== Overdue Books ===\n\n";
    auto transactions = system->getOverdueTransactions();
    if (transactions.empty()) {
        std::cout << "No overdue books found.\n";
    } else {
        for (const auto& t : transactions) {
            t.display();
        }
    }
}

// Report operations
void Menu::showStatistics() {
    clearScreen();
    std::cout << "=== Library Statistics ===\n\n";
    std::cout << "Total Books: " << system->getTotalBooks() << "\n";
    std::cout << "Total Members: " << system->getTotalMembers() << "\n";
    std::cout << "Total Borrowed Books: " << system->getTotalBorrowedBooks() << "\n";
    std::cout << "Total Overdue Books: " << system->getTotalOverdueBooks() << "\n";
}

void Menu::exportToCSV() {
    // TODO: Implement CSV export functionality
    std::cout << "CSV export functionality not implemented yet.\n";
} 