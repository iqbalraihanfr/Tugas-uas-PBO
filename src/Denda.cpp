#include "../include/Denda.h"

Denda::Denda() : id_denda(""), jumlah_denda(0.0f), status_pembayaran("belum dibayar") {}

Denda::Denda(const std::string& id, float jumlah, const std::string& status)
    : id_denda(id), jumlah_denda(jumlah), status_pembayaran(status) {}

std::string Denda::getIdDenda() const { return id_denda; }
float Denda::getJumlahDenda() const { return jumlah_denda; }
std::string Denda::getStatusPembayaran() const { return status_pembayaran; }
void Denda::setJumlahDenda(float jumlah) { jumlah_denda = jumlah; }
void Denda::setStatusPembayaran(const std::string& status) { status_pembayaran = status; }

void Denda::bayarDenda() {
    status_pembayaran = "sudah dibayar";
} 