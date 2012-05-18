#ifndef CRYPT_H
#define CRYPT_H

#include "QString"

class Crypt{
public:
    Crypt(QString *text, QString *key, int format);

    void caesar(int mode);
    void vigenere(int mode);

private:
    QString* text;
    QString* key;
    int format;
};

#endif // CRYPT_H
