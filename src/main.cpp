#include "LibrarySystem.h"
#include "Menu.h"
#include <iostream>
#include <memory>

int main() {
    try {
        // Create and initialize the library system
        auto system = std::make_unique<LibrarySystem>();
        
        // Create and run the menu
        Menu menu(system.get());
        menu.run();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 