#ifndef CRYPT_H
#define CRYPT_H

#include "QString"

class Crypt{
//Constructor
public:
    Crypt(QString *text, QString *key, int format);

//Methods
    void caesar(int mode);
    void vigenere(int mode);

//Attributes
private:
    QString* text;
    QString* key;
    int format;
};

#endif // CRYPT_H
