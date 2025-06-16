cmake_minimum_required(VERSION 3.10)
project(library_management)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MySQL paths for macOS with Homebrew
set(MYSQL_INCLUDE_DIR "/opt/homebrew/include/mysql")
set(MYSQL_LIB_DIR "/opt/homebrew/lib")
set(MYSQL_LIBRARIES "/opt/homebrew/lib/libmysqlclient.dylib")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${MYSQL_INCLUDE_DIR}
)

# Add executable
add_executable(library_management
    src/main.cpp
    src/DatabaseManager.cpp
    src/Book.cpp
    src/Member.cpp
    src/LoanTransaction.cpp
    src/Librarian.cpp
    src/LibrarySystem.cpp
    src/Menu.cpp
)

# Link libraries
target_link_libraries(library_management
    ${MYSQL_LIBRARIES}
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) 