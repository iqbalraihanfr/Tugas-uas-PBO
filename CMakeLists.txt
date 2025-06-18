cmake_minimum_required(VERSION 3.10)
project(library_schema)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific MySQL configuration
if(WIN32)
    # Windows MySQL paths - adjust these paths according to your MySQL installation
    set(MYSQL_INCLUDE_DIR "C:/Program Files/MySQL/MySQL Server 8.0/include")
    set(MYSQL_LIB_DIR "C:/Program Files/MySQL/MySQL Server 8.0/lib")
    set(MYSQL_LIBRARIES "libmysql")
elseif(APPLE)
    # macOS MySQL paths with Homebrew
    set(MYSQL_INCLUDE_DIR "/opt/homebrew/include/mysql")
    set(MYSQL_LIB_DIR "/opt/homebrew/lib")
    set(MYSQL_LIBRARIES "/opt/homebrew/lib/libmysqlclient.dylib")
    
    # Add compiler flags for macOS
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/homebrew/include")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/lib")
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${MYSQL_INCLUDE_DIR}
)

# Add executable
add_executable(library_schema
    src/main.cpp
    src/DatabaseManager.cpp
    src/Menu.cpp
    src/Admin.cpp
    src/Anggota.cpp
    src/Pengguna.cpp
    src/Buku.cpp
    src/Peminjaman.cpp
    src/Denda.cpp
)

# Link libraries
target_link_libraries(library_schema
    ${MYSQL_LIBRARIES}
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) 