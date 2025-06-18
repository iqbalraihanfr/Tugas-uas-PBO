#include "../include/Buku.h"

Buku::Buku() : id_buku(""), judul(""), penulis(""), status_ketersediaan("tersedia") {}

Buku::Buku(const std::string& id, const std::string& judul, const std::string& penulis, const std::string& status)
    : id_buku(id), judul(judul), penulis(penulis), status_ketersediaan(status) {}

std::string Buku::getIdBuku() const { return id_buku; }
std::string Buku::getJudul() const { return judul; }
std::string Buku::getPenulis() const { return penulis; }
std::string Buku::getStatusKetersediaan() const { return status_ketersediaan; }
void Buku::setJudul(const std::string& j) { judul = j; }
void Buku::setPenulis(const std::string& p) { penulis = p; }
void Buku::setStatusKetersediaan(const std::string& status) { status_ketersediaan = status; }
bool Buku::cekKetersediaan() const { return status_ketersediaan == "tersedia"; }
void Buku::updateStatus(const std::string& status) { status_ketersediaan = status; } 