#ifndef PENGGUNA_H
#define PENGGUNA_H

#include <string>

class Pengguna {
protected:
    std::string id_pengguna;
    std::string nama;
    std::string username;
    std::string password;
public:
    Pengguna();
    Pengguna(const std::string& id, const std::string& nama, const std::string& username, const std::string& password);
    virtual ~Pengguna() = default;

    virtual bool login(const std::string& username, const std::string& password);
    virtual void logout();

    // Getters and setters
    std::string getIdPengguna() const;
    std::string getNama() const;
    std::string getUsername() const;
    void setNama(const std::string& nama);
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
};

#endif // PENGGUNA_H 