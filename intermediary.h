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
    long remainingPixel_1Bit() { return availablePixel() - totalBits(); }
    long remainingPixel_3Bit() { return availablePixel() - (totalBits() / 3); }
    long remainingPixel_6Bit() { return availablePixel() - (totalBits() / 6); }
    long availablePixel();
    long totalBits() { return bitsToHide->count(); }
    void hide_1Bit(QString savePath);
    void hide_3Bit(QString savePath);
    void hide_6Bit(QString savePath);
    QString* getHiddenText();

private:
    int format;
    BitChanger* bitChanger;
    QString* bitsToHide;
    QMap<QString, QImage>* images;
};

#endif // INTERMEDIARY_H
