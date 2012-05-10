#ifndef INTERMEDIARY_H
#define INTERMEDIARY_H

#include "bitChanger.h"
#include "QImage"
#include "QMap"
#include "QString"

class Intermediary
{
public:
    Intermediary(QString* text, int format, QString imagePath);
    void setText(QString* text, int format);
    void addImage(QString imagePath);
    bool isReady_1Bit() { return remainingPixel_1Bit() <= 0; }
    bool isReady_3Bit() { return remainingPixel_3Bit() <= 0; }
    bool isReady_6Bit() { return remainingPixel_6Bit() <= 0; }
    long remainingPixel_1Bit() { return totalBits() - availableBits(); }
    long remainingPixel_3Bit() { return (totalBits() / 3) - availableBits(); }
    long remainingPixel_6Bit() { return (totalBits() / 6) - availableBits(); }
    long availableBits() { return availableChar() * bitsPerChar(); }
    long availableChar();
    long availableChar(QImage image);
    long totalBits() { return textToHide->count() * bitsPerChar(); }
    long bitsPerChar() { return format == 0? 8 : 16; }
    void hide_1Bit(QString savePath);
    void hide_3Bit(QString savePath);
    void hide_6Bit(QString savePath);
    QString* getHiddenText();
    QImage* getHiddenImage();

private:
    int format;
    long bitsToHide;
    QString* textToHide;
    QMap<QString, QImage>* images;
};

#endif // INTERMEDIARY_H
