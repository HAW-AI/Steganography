#include "bitChanger.h"
#include "intermediary.h"
#include "steganography.h"

#include "iostream"
#include "QImage"
#include "QList"
#include "QMap"

#define LINE "==============================="
#define HEADER_SIZE 193

using namespace std;

Intermediary::Intermediary(QString* text, int format, QString imagePath) {
    bitChanger = new BitChanger();
    images = new QMap<QString, QImage>();
    addImage(imagePath);
    setText(text, format);
}

Intermediary::Intermediary(QString imagePath) {
    bitChanger = new BitChanger();
    images = new QMap<QString, QImage>();
    addImage(imagePath);
    setText(new QString(), 0);
}

void Intermediary::setText(QString* text, int format) {
    this->format = format;
    this->textToHide = text;
}

void Intermediary::addImage(QString imagePath) {
    QImage image = QImage(imagePath);
    if (availableChar(image) > 0) {
        images->insert(imagePath, QImage(imagePath));
    }
}

long Intermediary::availableChar() {
    long result = 0;
    foreach (const QImage &image, images->values()) {
        result += availableChar(image);
    }
    return result;
}

long Intermediary::availableChar(QImage image) {
    long result = 0;
    if (image.width() >= HEADER_SIZE && image.height() > 1) {
        result = (image.width() * (image.height() - 1)) / bitsPerChar();
    }
    return result;
}

void Intermediary::hide_1Bit(QString savePath) {
    std::cout<<endl;
    std::cout<<LINE<<endl;
    std::cout<<"is ready? ";
    if (!isReady_1Bit()) {
        std::cout<<"no"<<endl;
    } else {
        std::cout<<"yes"<<endl;

        int sequenceNo = 1;
        long totalChar = 0;
        long start = 0;
        long range = 0;
        long charRate = (availableChar() / textToHide->count()) + (availableChar() % textToHide->count());

        std::cout<<"availableChar: "<<availableChar()<<endl;
        std::cout<<"charRate: "<<charRate<<endl;

        QMap<QString, QImage>::const_iterator it = images->constBegin();
        while (it != images->constEnd() && (start + range) < textToHide->count()) {
            QImage image = it.value();
            totalChar = availableChar(image);
            start = start + range;
            range = (totalChar / charRate);
            if (range == 0 || totalChar % charRate != 0) range++;

            std::cout<<LINE<<endl;
            std::cout<<"totalChar: "<<totalChar<<endl;
            std::cout<<"start: "<<start<<endl;
            std::cout<<"range: "<<range<<endl;

            QString currentText = textToHide->mid(start, range);
            std::cout<<"text: "<<currentText.toStdString()<<endl;

            // bits to hide
            QList<uint>* bits = new QList<uint>();
            if (format == 0)
                bits = bitChanger->textToBitsInIntList_8Bit(&currentText);
            else
                bits = bitChanger->textToBitsInIntList_16Bit(&currentText);

            Steganography* stego = new Steganography(it.key());
            stego->insertSizeInHeader(bits->size() * 32);
            stego->insertTBFieldInHeader(1);
            stego->insertFirstAttribute(format);
            stego->insertSecondAttribute(0);
            stego->insertBitsPerPixelInHeader(1);
            stego->insertSequenceNoInHeader(sequenceNo);
            stego->insertRealSizeInHeader(currentText.size());
            stego->insertBitstream(bits);

            QString fileName = savePath;
            if (images->size() > 1){
                fileName.insert(fileName.size()-4, QString().setNum(sequenceNo));
            }
            std::cout<<fileName.toStdString()<<endl;
            stego->saveImage(fileName);

            it++;
            sequenceNo++;
        }
    }
    std::cout<<LINE<<endl<<endl;
}

void Intermediary::hide_3Bit(QString savePath) {
    std::cout<<endl;
    std::cout<<LINE<<endl;
    std::cout<<"is ready? ";
    if (!isReady_3Bit()) {
        std::cout<<"no"<<endl;
    } else {
        std::cout<<"yes"<<endl;

        // ToDo
    }
    std::cout<<LINE<<endl<<endl;
}

void Intermediary::hide_6Bit(QString savePath) {
    std::cout<<endl;
    std::cout<<LINE<<endl;
    std::cout<<"is ready? ";
    if (!isReady_1Bit()) {
        std::cout<<"no"<<endl;
    } else {
        std::cout<<"yes"<<endl;

        // ToDo
    }
    std::cout<<LINE<<endl<<endl;
}

QString* Intermediary::getHiddenText() {
    // With QMap, the items are always sorted by key!
    QMap<int, QString*>* textMap = new QMap<int, QString*>();

    // get all parts of the text
    QMap<QString, QImage>::const_iterator it = images->constBegin();
    while (it != images->constEnd()) {
        Steganography* stego = new Steganography(it.key());
        QString* currentText = new QString();
        QList<uint>* bits = stego->getBitStreamAsIntList();
        int realSize = stego->getRealSizeFromHeader();
        if (stego->getFirstAttributeFromHeader() == 0) {
            currentText = bitChanger->bitStreamToText_8Bit(bits, realSize);
        } else {
            currentText = bitChanger->bitStreamToText_16Bit(bits, realSize);
        }
        textMap->insert(stego->getSequenceNoFromHeader(), currentText);
        it++;
    }

    // put all parts of the text together
    QString* result = new QString();
    QMap<int, QString*>::const_iterator it2 = textMap->constBegin();
    while (it2 != textMap->constEnd()) {
        result->append(it2.value());
        it2++;
    }
    std::cout<<endl;
    std::cout<<LINE<<endl;
    std::cout<<"hidden text: "<<result->toStdString()<<endl;
    std::cout<<LINE<<endl;
    return result;
}

int Intermediary::imageOrTextHidden() {
    Steganography* stego = new Steganography(images->constBegin().key());
    return stego->getIntFromHeader(32);
}
