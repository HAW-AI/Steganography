#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include "qimage.h"
class Steganography
{
public:
    Steganography(QString file);
    int insertText(QString*,int);
    bool imageIsNull();
    QString* getHiddenText();
    bool saveImage(QString file);
    int insertBitstream(QString*);
    int getFormatFromHeader();
    int getIntFromHeader(int from);
    int insertFirstAttribute(int);
    int insertSecondAttribute(int);
    int insertSizeInHeader(int);
    int insertIntInHeader(int, int);
    int insertPictureHeader(int, int, int);
    int insertTextHeader(int, int);
    int getFirstAttributeFromHeader();
    int getSecondAttributeFromHeader();
    QString* getBitStream();



private:
    QImage image;
};

#endif // STEGANOGRAPHY_H
