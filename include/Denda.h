#ifndef DENDA_H
#define DENDA_H

#include <string>

class Denda {
private:
    std::string id_denda;
    float jumlah_denda;
    std::string status_pembayaran;
public:
    Denda();
    Denda(const std::string& id, float jumlah, const std::string& status);

    std::string getIdDenda() const;
    float getJumlahDenda() const;
    std::string getStatusPembayaran() const;
    void setJumlahDenda(float jumlah);
    void setStatusPembayaran(const std::string& status);

    void bayarDenda();
};

#endif // DENDA_H 