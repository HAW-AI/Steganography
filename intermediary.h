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
    Intermediary(QString imagePath);
    void setText(QString* text, int format);
    void addImage(QString imagePath);
    bool isReady_1Bit() { return remainingPixel_1Bit() <= 0; }
    bool isReady_3Bit() { return remainingPixel_3Bit() <= 0; }
    bool isReady_6Bit() { return remainingPixel_6Bit() <= 0; }
    void hide_1Bit(QString savePath);
    void hide_3Bit(QString savePath);
    void hide_6Bit(QString savePath);
    QString* getHiddenText();
    QImage* getHiddenImage();
    int imageOrTextHidden();

    QMap<QString, QImage>* images;

private:
    long remainingPixel_1Bit() { return totalBits() - availableBits(); }
    long remainingPixel_3Bit() { return totalBits() - availableBits() * 3; }
    long remainingPixel_6Bit() { return totalBits() - availableBits() * 6; }
    long availableBits() { return availableChar() * bitsPerChar(); }
    long availableChar();
    long availableChar(QImage image);
    long totalBits() { return textToHide->size() * bitsPerChar(); }
    long bitsPerChar() { return format == 0? 8 : 16; }

    int format;
    long bitsToHide;
    QString* textToHide;
    //QMap<QString, QImage>* images; //Fenja war's
    BitChanger* bitChanger;
};

// Konstruktor: 0 = ASCII / 1 = UNICODE

// HIDE
//QString test = "hey!!!!";
//Intermediary* iHide = new Intermediary(&test, 0, "/Users/Raimund/test1.png");
//iHide->addImage("/Users/Raimund/test2.png");
//iHide->addImage("/Users/Raimund/test3.png");
//iHide->hide_1Bit("/Users/Raimund/testResult"); // savePath erstmal ohne ".png" damit wir die Zahl im Dateinamen inkrementieren können!

// FIND
//Intermediary* iFind = new Intermediary("/Users/Raimund/testResult1.png");
//iFind->addImage("/Users/Raimund/testResult2.png");
//iFind->addImage("/Users/Raimund/testResult3.png");
//iFind->getHiddenText();

#endif // INTERMEDIARY_H
