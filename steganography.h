#ifndef VERARBEITUNG_H
#define VERARBEITUNG_H

#include "qimage.h"
class Steganography
{
public:
    Steganography(QString file);
    QString insertText(QString text);
    QString insertHeader(int Wert);
    int getHeader();
    bool imageIsNull();
    static QString bitStreamToText(QString);
    QString getText();
    bool saveImage(QString file);
private:
    QImage image;
};

#endif // VERARBEITUNG_H
