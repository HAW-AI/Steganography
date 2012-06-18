#include "tests.h"
#include <QDebug>
#include "bitChanger.h"
#include "steganography.h"
#include "intermediary.h"
#include <QList>
#include "crypt.h"

#define UNICODE 1
#define ASCII 0
#define ENCRYPT 1
#define DECRYPT 0

Tests::Tests()
{
    mainWindowTest();
    steganographyTest();
    bitChangerTest();
}

void Tests::mainWindowTest()
{
    QString plain = QString("HALLO dies IST ein TeSt");
    QString savePath = "C://QtSDK//projects//HAW-AI-Steganography-22305d1//test_ausgabe.png";
    int format = 0;
    QString oldPath = "C://QtSDK//projects//HAW-AI-Steganography-22305d1//test.png";
    Intermediary hideIntermediary = Intermediary(oldPath);

    hideIntermediary.setText(&plain,format);
    hideIntermediary.hide_1Bit(savePath);
    Intermediary findIntermediary = Intermediary(savePath);
    qDebug()<<"'HALLO dies IST ein TeSt' wurde versteckt, gefunden wurde :"<<*findIntermediary.getHiddenText();

    QString key = QString("TEST");
    Crypt c (&plain, &key, ASCII);
    c.caesar(1);
    qDebug()<<"'HALLO dies IST ein TeSt' wurde verschlüsselt (caesar): "<< plain;
    c.caesar(0);
    qDebug()<<"Text wurde wieder entschlüsselt (caesar): "<<plain;

    c.vigenere(1);
    qDebug()<<"'HALLO dies IST ein TeSt' wurde verschlüsselt (viginere): "<< plain;
    c.vigenere(0);
    qDebug()<<"Text wurde wieder entschlüsselt (viginere): "<<plain;

}

void Tests::bitChangerTest()
{
    qDebug("### Test von BitChanger:");
    qDebug();
    BitChanger bitchanger = BitChanger();
    QString* a = bitchanger.toBits(1,1);
    qDebug()<< "a = bitchanger.toBits(1,1) --> "<< *a <<", erwartet war: 1";

    QString* b = bitchanger.toBits(1,3);
    qDebug()<<"b = bitchanger.toBits(1,3) -->"<<*b<<", erwartet war: 001";

    QString* c = bitchanger.toBits(0,1);
    qDebug()<<"c = bitchanger.toBits(0,1) -->"<<*c<<", erwartet war: 0";

    QString* d = bitchanger.toBits(1,0);
    qDebug()<<"d = bitchanger.toBits(1,0) -->"<< *d << ", erwartet war: 1";

    QString* e = bitchanger.toBits(33,8);
    qDebug()<<"e = bitchanger.toBits(33,8) --> "<<*e<<", erwartet war: 00100001";

    QString* f = bitchanger.toBits(1999,1);
    qDebug()<<"f = bitchanger.toBits(1999,1) --> "<<*f<<" erwartet war: 11111001111";

    qDebug();
    qDebug()<<"*bitchanger.toIntVal(a) --> "<< bitchanger.toIntVal(a) <<", erwartet war: 1";
    qDebug()<<"*bitchanger.toIntVal(b) --> "<< bitchanger.toIntVal(b) <<", erwartet war: 1";
    qDebug()<<"*bitchanger.toIntVal(c) --> "<< bitchanger.toIntVal(c) <<", erwartet war: 0";
    qDebug()<<"*bitchanger.toIntVal(d) --> "<< bitchanger.toIntVal(d) <<", erwartet war: 1";
    qDebug()<<"*bitchanger.toIntVal(e) --> "<< bitchanger.toIntVal(e) <<", erwartet war: 33";
    qDebug()<<"*bitchanger.toIntVal(f) --> "<< bitchanger.toIntVal(f) <<", erwartet war: 1999";

    qDebug();
    qDebug()<<"*bitchanger.textToBitsInIntList_8Bit(&QString('A') -->  "<<*bitchanger.textToBitsInIntList_8Bit(&QString("A"))<<", erwartet war: 01000001";
    qDebug()<<"*bitchanger.textToBitsInIntList_8Bit(&QString('') --> "<<*bitchanger.textToBitsInIntList_8Bit(&QString(""))<<", erwartet war: 00000000";
    qDebug()<<"*bitchanger.textToBitsInIntList_8Bit(&QString('AAAA') --> "<<*bitchanger.textToBitsInIntList_8Bit(&QString("AAAA"))<<", erwartet war: 01000001010000010100000101000001";
    qDebug()<<"*bitchanger.textToBitsInIntList_8Bit(&QString('ABC') --> "<<*bitchanger.textToBitsInIntList_8Bit(&QString("ABC"))<<", erwartet war: 010000010100001001000011";

    qDebug(); // FEHLER - Immer der Selbe Wert!
    qDebug()<<"*bitchanger.textToBitsInIntList_16Bit(&QString('A')) --> "<< *bitchanger.textToBitsInIntList_16Bit(&QString("A"))<<", erwartet war: 0000000001000001";
    qDebug()<<"*bitchanger.textToBitsInIntList_16Bit(&QString('')) --> "<<*bitchanger.textToBitsInIntList_16Bit(&QString(""))<<", erwartet war: 0000000000000000";
    qDebug()<<"*bitchanger.textToBitsInIntList_16Bit(&QString('AAAA')) -->" <<*bitchanger.textToBitsInIntList_16Bit(&QString("AAAA"))<<", erwartet war: 0000000001000001000000000100000100000000010000010000000001000001";
    qDebug()<<"*bitchanger.textToBitsInIntList_16Bit(&QString('ABC')) --> "<<*bitchanger.textToBitsInIntList_16Bit(&QString("ABC"))<<", erwartet war: 000000000100000100000000010000100000000001000011";
    qDebug();

    QList<uint> list;
    list<< 1 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 0;
    qDebug()<<"bitchanger.bitStreamToText_8Bit(1000000110000010) --> "<<bitchanger.bitStreamToText_8Bit(&list,16)<<", erwartet war: 'AB'";
    list<<1;
    qDebug()<<"bitchanger.bitStreamToText_8Bit(&QString('1')) --> "<<*bitchanger.bitStreamToText_8Bit(&list,1)<<", erwartet war: FEHLER";
    list<< 0 << 0 << 1 << 1 << 0 << 0 << 1 << 1;
    qDebug()<<"bitchanger.bitStreamToText_8Bit(&QString('00110011')) --> "<<*bitchanger.bitStreamToText_8Bit(&list,8)<<", erwartet war: '3'";
    list<< 1 << 0 << 0 << 0 << 0 << 1 << 0 << 0;
    qDebug()<<"bitchanger.bitStreamToText_8Bit(&QString('10000100')) --> "<<*bitchanger.bitStreamToText_8Bit(&list,8)<<", erwartet war: 'Z'";
    list<< 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 ;
    qDebug()<<"bitchanger.bitStreamToText_8Bit(&QString('111111111')) --> "<<*bitchanger.bitStreamToText_8Bit(&list,9)<<", erwartet war: FEHLER";
    list<< 1 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 1;
    qDebug()<<"bitchanger.bitStreamToText_8Bit(&QString('100000011000001010000011')) --> "<<*bitchanger.bitStreamToText_8Bit(&list,24)<<", erwartet war: 'ABC'";

    qDebug();
    list<< 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 1 << 0;
    qDebug()<<"bitchanger.bitStreamToText_16Bit(&QString('00000000100000010000000010000010')) --> "<<*bitchanger.bitStreamToText_16Bit(&list,32)<<", erwartet war: 'AB'";
    list<< 1;
    qDebug()<<"bitchanger.bitStreamToText_16Bit(&QString('1')) --> "<<*bitchanger.bitStreamToText_16Bit(&list,1)<<", erwartet war: FEHLER";
    list<< 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 1 << 0 << 0 << 1 << 1;
    qDebug()<<"bitchanger.bitStreamToText_16Bit(&QString('0000000000110011')) --> "<<*bitchanger.bitStreamToText_16Bit(&list, 16)<<", erwartet war: '3'";
    list<< 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 0 << 1 << 1 << 0 << 1 << 0;
    qDebug()<<"bitchanger.bitStreamToText_16Bit(&QString('0000000001011010')) --> "<<*bitchanger.bitStreamToText_16Bit(&list, 16)<<", erwartet war: 'Z'";
    list<< 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 ;
    qDebug()<<"bitchanger.bitStreamToText_16Bit(&QString('111111111')) --> "<<*bitchanger.bitStreamToText_16Bit(&list,8)<<", erwartet war: FEHLER";
    list<< 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 1 << 1 ;
    qDebug()<<"bitchanger.bitStreamToText_16Bit(&QString('000000001000000100000000100000100000000010000011')) --> "<<*bitchanger.bitStreamToText_16Bit(&list,48)<<", erwartet war: 'ABC'";

    qDebug();
    qDebug()<<"bitchanger.changeLastBit(200,'1') --> "<< bitchanger.changeLastBit(200,'1')<<", erwartet war: 201";
    qDebug()<<"bitchanger.changeLastBit(201,'1') --> "<< bitchanger.changeLastBit(201,'1')<<", erwartet war: 201";
    qDebug()<<"bitchanger.changeLastBit(0,'1') --> "<< bitchanger.changeLastBit(0,'1')<<", erwartet war: 1";
    qDebug()<<"bitchanger.changeLastBit(200,'10') --> "<< bitchanger.changeLastBit(200,'10')<<", erwartet war: 200";
    qDebug()<<"bitchanger.changeLastBit(200,'010') --> "<< bitchanger.changeLastBit(200,'010')<<", erwartet war: 200";


    qDebug();
}



void Tests::steganographyTest()
{

    BitChanger bitchanger = BitChanger();

    qDebug();
    qDebug()<<"SteganographyTest - Anfang";

    qDebug();
    Steganography a = Steganography::Steganography("te");
    Steganography b = Steganography::Steganography("C:\\QtSDK\\projects\\HAW-AI-Steganography-22305d1\\test.png");
    qDebug()<<"Steganography a = Steganography::Steganography('te');";
    qDebug()<<"Steganography b = Steganography::Steganography('C:\\QtSDK\\projects\\HAW-AI-Steganography-22305d1\\test.png');";
    qDebug()<<"a.imageIsNull() --> "<<a.imageIsNull()<<", erwartet war: true";
    qDebug()<<"b.imageIsNull() --> "<<b.imageIsNull()<<", erwartet war: false";


    QString newPath = QString("C:\\QtSDK\\projects\\HAW-AI-Steganography-22305d1\\test_ausgabe.png");
    qDebug();
    qDebug()<<"b.saveImage('newPath') --> "<< b.saveImage(QString("C:\\QtSDK\\projects\\HAW-AI-Steganography-22305d1\\test_ausgabe.png")) << ", erwartet war: true";
    qDebug()<<"b.saveImage('C:\\QtSDK\\projects\\HAW-AI-Steganography-22305d1\\') --> "<< b.saveImage(QString("C:\\QtSDK\\projects\\HAW-AI-Steganography-22305d1\\")) << ", erwartet war: false";
    qDebug()<<"b.saveImage('test_ausgabe') --> "<< b.saveImage("test_ausgabe2") << ", erwartet war: true";
    qDebug()<<"b.saveImage('test_ausgabe.png') --> "<< b.saveImage(QString("test_ausgabe3.png")) << ", erwartet war: true";
    qDebug()<<"a.saveImage('C:\\QtSDK\\projects\\HAW-AI-Steganography-22305d1\\test_ausgabe') --> "<< a.saveImage(QString("C:\\QtSDK\\projects\\HAW-AI-Steganography-22305d1\\test_ausgabe.png")) << ", erwartet war: false";

    qDebug();
    qDebug()<<"b.insertBitstream(bitchanger.textToBitsInIntList_16Bit(&QString('HALLO'))) --> "<<b.insertBitstream(bitchanger.textToBitsInIntList_16Bit(&QString("HALLO")))<<", erwartet war: 1";
    qDebug()<<"a.insertBitstream(bitchanger.textToBitsInIntList_16Bit(&QString('HALLO'))) --> "<<a.insertBitstream(bitchanger.textToBitsInIntList_16Bit(&QString("HALLO")))<<", erwartet war: -2";

    qDebug();
    qDebug()<<"b.insertBitstream_3BitsPerPixel(bitchanger.textToBitsInIntList_16Bit(&QString('HALLO'))) --> "<<b.insertBitstream_3BitsPerPixel(bitchanger.textToBitsInIntList_16Bit(&QString("HALLO")))<<", erwartet war: 1";
    qDebug()<<"a.insertBitstream_3BitsPerPixel(bitchanger.textToBitsInIntList_16Bit(&QString('HALLO'))) --> "<<a.insertBitstream_3BitsPerPixel(bitchanger.textToBitsInIntList_16Bit(&QString("HALLO")))<<", erwartet war: -2";

    qDebug();
    qDebug()<<"insertIntInHeader(5,5) --> "<< b.insertIntInHeader(5,5) <<", erwartet war: 1";
//    qDebug()<<"insertIntInHeader(5,5) --> "<< a.insertIntInHeader(5,5) <<", erwartet war: FEHLER"; // FEHLER PROGRAMM ABSTURZ

    qDebug();
    qDebug()<<"b.getIntFromHeader(5) --> "<< b.getIntFromHeader(5) <<", erwartet war: 5";
//    qDebug()<<"a.getIntFromHeader(5) --> "<< a.getIntFromHeader(5) <<", erwartet war: FEHLER"; // ABSTURZ DES PROGRAMMS

    qDebug();
    qDebug()<<"b.insertSizeInHeader(50) --> "<<b.insertSizeInHeader(50)<<", erwartet war: 1";
    qDebug()<<"a.insertSizeInHeader(50) --> "<<a.insertSizeInHeader(50)<<", erwartet war: -2";

    qDebug();
    qDebug()<<"b.getSizeFromHeader() -->"<<b.getSizeFromHeader()<<", erwartet war: 50";
    qDebug()<<"a.getSizeFromHeader() -->"<<a.getSizeFromHeader()<<", erwartet war: -1";

    qDebug();
    qDebug()<<"b.insertBitsPerPixelInHeader(3) -->"<<b.insertBitsPerPixelInHeader(3)<<", erwartet war: 1";
    qDebug()<<"a.insertBitsPerPixelInHeader(3) -->"<<a.insertBitsPerPixelInHeader(3)<<", erwartet war: -2";

    qDebug();
    qDebug()<<"b.getBitsPerPixelFromHeader() -->"<<b.getBitsPerPixelFromHeader()<<", erwartet war: 3";
    qDebug()<<"a.getBitsPerPixelFromHeader() -->"<<a.getBitsPerPixelFromHeader()<<", erwartet war: -1";

    qDebug();
    qDebug()<<"b.insertSequenceNoInHeader(3) -->"<<b.insertSequenceNoInHeader(3)<<", erwartet war: 1";
    qDebug()<<"a.insertSequenceNoInHeader(3) -->"<<a.insertSequenceNoInHeader(3)<<", erwartet war: -2";

    qDebug();
    qDebug()<<"b.getSequenceNoFromHeader() -->"<<b.getSequenceNoFromHeader()<<", erwartet war: 3";
    qDebug()<<"a.getSequenceNoFromHeader() -->"<<a.getSequenceNoFromHeader()<<", erwartet war: -1";

    qDebug();
    qDebug()<<"b.insertFirstAttribute(5) -->"<<b.insertFirstAttribute(5)<<", erwartet war: 1";
    qDebug()<<"a.insertFirstAttribute(5) -->"<<a.insertFirstAttribute(5)<<", erwartet war: -2";

    qDebug();
    qDebug()<<"b.getFirstAttributeFromHeader() -->"<<b.getFirstAttributeFromHeader()<<", erwartet war: 5";
    qDebug()<<"a.getFirstAttributeFromHeader() -->"<<a.getFirstAttributeFromHeader()<<", erwartet war: -1";

    qDebug();
    qDebug()<<"b.insertSecondAttribute(5) -->"<<b.insertSecondAttribute(5)<<", erwartet war: 1";
    qDebug()<<"a.insertSecondAttribute(5) -->"<<a.insertSecondAttribute(5)<<", erwartet war: -2";

    qDebug();
    qDebug()<<"b.getSecondAttributeFromHeader() -->"<<b.getSecondAttributeFromHeader()<<", erwartet war: 5";
    qDebug()<<"a.getSecondAttributeFromHeader() -->"<<a.getSecondAttributeFromHeader()<<", erwartet war: -1";

    qDebug();
    qDebug()<<"b.lastBit(1) -->"<<b.lastBit(1)<<", erwartet war: 1";
    qDebug()<<"b.lastBit(1) -->"<<b.lastBit(0)<<", erwartet war: 0";
    qDebug()<<"b.lastBit(1) -->"<<b.lastBit(9)<<", erwartet war: 1";
    qDebug()<<"b.lastBit(1) -->"<<b.lastBit(10)<<", erwartet war: 0";
    qDebug()<<"b.lastBit(1) -->"<<b.lastBit(27)<<", erwartet war: 1";
    qDebug()<<"a.lastBit(1) -->"<<a.lastBit(1)<<", erwartet war: 1";
    qDebug()<<"a.lastBit(1) -->"<<a.lastBit(0)<<", erwartet war: 0";
    qDebug()<<"a.lastBit(1) -->"<<a.lastBit(9)<<", erwartet war: 1";
    qDebug()<<"a.lastBit(1) -->"<<a.lastBit(10)<<", erwartet war: 0";
    qDebug()<<"a.lastBit(1) -->"<<a.lastBit(27)<<", erwartet war: 1";


    qDebug();
    qDebug()<<"b.getFormatFromHeader() --> "<<b.getFormatFromHeader()<<", erwartet war: 2";

    qDebug();
    qDebug("Fertig");

}
