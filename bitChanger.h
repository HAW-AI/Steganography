#ifndef bitChanger_H
#define bitChanger_H

#include "QString"
#include "QChar"
#include "qimage.h"



class BitChanger
{
public:
    BitChanger();
    static QString* toBits(int i, int size = 16);
    static uint toIntVal(QString*);
    static int redVal(int value,QString c);
    static int blueVal(int value, QString c);
    static int greenVal(int value, QString c);
   // static int changeLastBits(int, QString,int, int);
    static int changeLastBit(int value, QChar bit);
    static QString* textToBits_8Bit(QString*);
    static QString* textToBits_16Bit(QString*);
    static QString* bitStreamToText_16Bit(QString*);
    static QString* bitStreamToText_8Bit(QString*);
    static int changeLastBits(int value, QString* bits);
    static QString getLastBits(int value, int bits);
    static QImage* bitStreamToPicture(QString* stream, int height, int width);
    static QString* pictureToBitstream(QImage* image);
    static QString* toBits(uint value, int size);

    static void stringTest();
    static QImage* erstelleQimage(int width, int height);
    static QString* bitStreamToText_8Bit(QList<uint>* bitstream);
};

#endif // bitChanger_H
