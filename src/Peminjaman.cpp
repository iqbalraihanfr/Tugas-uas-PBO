#include "../include/Peminjaman.h"

Peminjaman::Peminjaman() : id_peminjaman(""), tanggal_pinjam(""), tanggal_kembali(""), status("dipinjam"), denda(nullptr) {}

Peminjaman::Peminjaman(const std::string& id, const std::string& tgl_pinjam, const std::string& tgl_kembali, const std::string& status)
    : id_peminjaman(id), tanggal_pinjam(tgl_pinjam), tanggal_kembali(tgl_kembali), status(status), denda(nullptr) {}

Peminjaman::~Peminjaman() {
    if (denda) delete denda;
}

std::string Peminjaman::getIdPeminjaman() const { return id_peminjaman; }
std::string Peminjaman::getTanggalPinjam() const { return tanggal_pinjam; }
std::string Peminjaman::getTanggalKembali() const { return tanggal_kembali; }
std::string Peminjaman::getStatus() const { return status; }
void Peminjaman::setTanggalPinjam(const std::string& tgl) { tanggal_pinjam = tgl; }
void Peminjaman::setTanggalKembali(const std::string& tgl) { tanggal_kembali = tgl; }
void Peminjaman::setStatus(const std::string& s) { status = s; }
Denda* Peminjaman::getDenda() const { return denda; }
void Peminjaman::setDenda(Denda* d) { denda = d; }
float Peminjaman::hitungDenda() const {
    return denda ? denda->getJumlahDenda() : 0.0f;
} 