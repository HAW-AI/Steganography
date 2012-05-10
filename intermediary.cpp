#include "bitChanger.h"
#include "intermediary.h"
#include "steganography.h"

#include "iostream"
#include "QImage"
#include "QMap"

#define LINE "==============================="

using namespace std;

Intermediary::Intermediary(QString* text, int format, QString imagePath)
{
    images = new QMap<QString, QImage>();
    setText(text, format);
    addImage(imagePath);
}

void Intermediary::setText(QString* text, int format)
{
    this->format = format;
    this->textToHide = text;
}

void Intermediary::addImage(QString imagePath)
{
    QImage image = QImage(imagePath);
    if (availableChar(image) > 0)
    {
        images->insert(imagePath, QImage(imagePath));
    }
}

long Intermediary::availableChar()
{
    long result = 0;
    foreach (const QImage &image, images->values())
    {
        result += availableChar(image);
    }
    return result;
}

long Intermediary::availableChar(QImage image)
{
    long result = 0;
    if (image.width() >= 160 && image.height() > 1)
    {
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

        int sequenceNo = 0;
        long totalChar = 0;
        long start = 0;
        long range = 0;
        long charRate = (availableChar() / textToHide->count()) + (availableChar() % textToHide->count());

        std::cout<<"totalBits: "<<totalBits()<<endl;
        std::cout<<"availableChar: "<<availableChar()<<endl;
        std::cout<<"charRate: "<<charRate<<endl;

        // 100 Char verf√ºgbar / 99 Char ben√∂tigt = charRate: 1 = Textl√§nge entspricht availableChar(image)
        // 100 Char verf√ºgbar / 52 Char ben√∂tigt = charRate: 1 = Textl√§nge entspricht availableChar(image)
        // 100 Char verf√ºgbar / 49 Char ben√∂tigt = charRate: 2 = Textl√§nge entspricht availableChar(image) / 2
        // 100 Char verf√ºgbar / 33 Char ben√∂tigt = charRate: 3 = Textl√§nge entspricht availableChar(image) / 3

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

            // 100 totalChar / charRate: 1 = 100 Chars
            // 100 totalChar / charRate: 2 = 50 Chars
            // 100 totalChar / charRate: 3 = 33 Chars

            Steganography* stego = new Steganography(it.key());
            stego->insertSizeInHeader(range);
            stego->insertTBFieldInHeader(1);
            stego->insertFirstAttribute(format);
            stego->insertSecondAttribute(0);
            stego->insertBitsPerPixelInHeader(1);
            stego->insertSequenceNoInHeader(sequenceNo);
            stego->insertText_1BitPerPixel(&currentText, format);

            QString fileName = savePath;
            fileName.append(QString().setNum(sequenceNo)).append(".png");
            std::cout<<fileName.toStdString()<<endl;
            stego->saveImage(fileName);

            it++;
            sequenceNo++;
        }
    }
    std::cout<<LINE<<endl<<endl;
}

void Intermediary::hide_3Bit(QString savePath) {
    if (isReady_3Bit()) {

    }
}

void Intermediary::hide_6Bit(QString savePath) {
    if (isReady_6Bit()) {

    }
}

QString* getHiddenText() {
    return new QString();
}
