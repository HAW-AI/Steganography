#ifndef INTERMEDIARY_H
#define INTERMEDIARY_H

#include "bitChanger.h"
#include "QImage"
#include "QList"
#include "QMap"
#include "QString"

class Intermediary
{
public:
    Intermediary(QString* text, int format, QString imagePath); //konstruktor unbenutzt
    Intermediary(QString imagePath);

    void setText(QString* text, int format);
    void setImage(QString imagePath);

    bool addImage(QString imagePath);

    bool isReady_1Bit() { return remainingPixel_1Bit() <= 0; }
    bool isReady_3Bit() { return remainingPixel_3Bit() <= 0; }
    bool isReady_6Bit() { return remainingPixel_6Bit() <= 0; }

    void hide_1Bit(QString savePath);
    void hide_3Bit(QString savePath);
    void hide_6Bit(QString savePath);

    QString* getHiddenText();
    QImage* getHiddenImage();
    int imageOrTextHidden();

    // public fields
    QMap<QString, QImage>* images;

private:
    long availableInts_1Bit();
    long availableInts_3Bit() { return availableInts_1Bit() * 3; }
    long availableInts_6Bit() { return availableInts_1Bit() * 6; }
    long availableInts_1Bit(QImage image);
    long availableInts_3Bit(QImage image) { return availableInts_1Bit(image) * 3; }
    long availableInts_6Bit(QImage image) { return availableInts_1Bit(image) * 6; }
    long remainingPixel_1Bit() { return (intsToHide->size() - availableInts_1Bit()) * 32; }
    long remainingPixel_3Bit() { return (intsToHide->size() - availableInts_3Bit()) * 32; }
    long remainingPixel_6Bit() { return (intsToHide->size() - availableInts_6Bit()) * 32; }
    long bitsPerChar() { return format == 0? 8 : 16; }
    void hide(bool isReady, long availableBits, int bitsPerPixel, QString savePath);

    // private fields
    int format;                 // ASCII = 0 / UNICODE = 1
    int type;                   // PICTURE = 0 / TEXT = 1
    QImage imageToHide;
    QString* textToHide;
    QList<uint>* intsToHide;
    BitChanger* bitChanger;
};

#endif // INTERMEDIARY_H

//QString test = "";
//for (int i = 0; i < 300; i++) {
//    test.append("abcdefghijklmnopqrstuvwxyz ");
//}
//Intermediary* iHide = new Intermediary(&test, 0, "/Users/Raimund/test1.png");
//iHide->addImage("/Users/Raimund/test2.png");
//iHide->addImage("/Users/Raimund/test3.png");
//iHide->hide_3Bit("/Users/Raimund/huhu.png");

//Intermediary* iFind = new Intermediary("/Users/Raimund/huhu1.png");
//iFind->addImage("/Users/Raimund/huhu2.png");
//iFind->getHiddenText();
