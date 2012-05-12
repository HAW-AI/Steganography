#ifndef bitChanger_H
#define bitChanger_H

#include "QString"
#include "QChar"
#include "qimage.h"



class BitChanger
{
public:
    BitChanger();
    static int changeBitAt(int value, int position, QChar bit);
    static int changeLastBit(int value, QChar bit);
    static int getBits(int value,int from, int numberOfBits);

    static int changeLastBits(int value, QString* bits);
    static int changeLastBits(int value, uint bits);
    static QString getLastBits(int value, int bits);
    static QChar getBitAt(uint value, uint position);


    static QString* toBits(int i, int size = 16);
    static QString* toBits(uint value, int size);
    static uint toIntVal(QString*);

    static int redVal(int value,QString c);
    static int blueVal(int value, QString c);
    static int greenVal(int value, QString c);

    static QString* textToBits_8Bit(QString*);
    static QString* textToBits_16Bit(QString*);
    static QString* pictureToBitstream(QImage* image);

    static QString* bitStreamToText_16Bit(QString*);
    static QString* bitStreamToText_8Bit(QString*);
    static QImage* bitStreamToPicture(QString* stream, int height, int width);

    static QList<uint>* textToBitsInIntList_8Bit(QString* s);
    static QList<uint>* textToBitsInIntList_16Bit(QString* s);
    static QList<uint>* pictureToBitstreamAsIntList(QImage* image);

    static QString* bitStreamToText_8Bit(QList<uint>* bitstream, int characters);
    static QString* bitStreamToText_16Bit(QList<uint>* bitstream, int characters);
    static QImage* bitStreamToPicture(QList<uint>* stream, int height, int width);


};

#endif // bitChanger_H
