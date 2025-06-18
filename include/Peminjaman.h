#ifndef PEMINJAMAN_H
#define PEMINJAMAN_H

#include <string>
#include "Denda.h"

class Peminjaman {
private:
    std::string id_peminjaman;
    std::string tanggal_pinjam;
    std::string tanggal_kembali;
    std::string status;
    Denda* denda;
public:
    Peminjaman();
    Peminjaman(const std::string& id, const std::string& tgl_pinjam, const std::string& tgl_kembali, const std::string& status);
    ~Peminjaman();

    std::string getIdPeminjaman() const;
    std::string getTanggalPinjam() const;
    std::string getTanggalKembali() const;
    std::string getStatus() const;
    void setTanggalPinjam(const std::string& tgl);
    void setTanggalKembali(const std::string& tgl);
    void setStatus(const std::string& status);

    Denda* getDenda() const;
    void setDenda(Denda* denda);

    float hitungDenda() const;
};

#endif // PEMINJAMAN_H 