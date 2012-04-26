#ifndef bitChanger_H
#define bitChanger_H

#include "QString"
#include "QChar"

#define BITS_PER_LETTER 16

class BitChanger
{
public:
    BitChanger();
    static QString* toBits(int i, int size = BITS_PER_LETTER);
    static uint toIntVal(QString*);
    static int redVal(int value,QString c);
    static int blueVal(int value, QString c);
    static int greenVal(int value, QString c);
    static int changeLastBits(int value, QString bits, int start, int end);
    static int changeLastBit(int value, QChar bit);
    static QString* textToBits_8Bit(QString*);
    static QString* textToBits_16Bit(QString*);
    static QString* bitStreamToText_16Bit(QString*);
    static QString* bitStreamToText_8Bit(QString*);
};

#endif // bitChanger_H
