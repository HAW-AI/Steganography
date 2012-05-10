#ifndef CRYPT_H
#define CRYPT_H

#include "QString"

class Crypt{
public:
    Crypt(QString text, QString key, int format);

    QString caesar(int mode);
    QString vigenere(int mode);

private:
    QString text;
    QString key;
    int format;
};

#endif // CRYPT_H
