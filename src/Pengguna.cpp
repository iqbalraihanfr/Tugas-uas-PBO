#include "../include/Pengguna.h"
#include <iostream>

Pengguna::Pengguna() {}

Pengguna::Pengguna(const std::string& id, const std::string& nama, const std::string& username, const std::string& password)
    : id_pengguna(id), nama(nama), username(username), password(password) {}

bool Pengguna::login(const std::string& uname, const std::string& pwd) {
    std::cout << "[STUB] Pengguna::login dipanggil\n";
    return (username == uname && password == pwd);
}

void Pengguna::logout() {
    std::cout << "[STUB] Pengguna::logout dipanggil\n";
}

std::string Pengguna::getIdPengguna() const { return id_pengguna; }
std::string Pengguna::getNama() const { return nama; }
std::string Pengguna::getUsername() const { return username; }
void Pengguna::setNama(const std::string& n) { nama = n; }
void Pengguna::setUsername(const std::string& u) { username = u; }
void Pengguna::setPassword(const std::string& p) { password = p; } 