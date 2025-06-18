#ifndef BUKU_H
#define BUKU_H

#include <string>

class Buku {
private:
    std::string id_buku;
    std::string judul;
    std::string penulis;
    std::string status_ketersediaan;
public:
    Buku();
    Buku(const std::string& id, const std::string& judul, const std::string& penulis, const std::string& status);

    std::string getIdBuku() const;
    std::string getJudul() const;
    std::string getPenulis() const;
    std::string getStatusKetersediaan() const;
    void setJudul(const std::string& judul);
    void setPenulis(const std::string& penulis);
    void setStatusKetersediaan(const std::string& status);

    bool cekKetersediaan() const;
    void updateStatus(const std::string& status);
};

#endif // BUKU_H 