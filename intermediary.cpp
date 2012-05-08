#include "bitChanger.h"
#include "intermediary.h"
#include "steganography.h"

#include "QImage"
#include "QMap"

#define UNICODE 1
#define ASCII 0

Intermediary::Intermediary(QString* text, int format, QString imagePath)
{
    bitChanger = new BitChanger();
    images = new QMap<QString, QImage>();
    setText(text, format);
    addImage(imagePath);
}

void Intermediary::setText(QString* text, int format)
{
    this->format = format;
    if (format == 0)
    {
        bitsToHide = bitChanger->textToBits_8Bit(text);
    }
    else
    {
        bitsToHide = bitChanger->textToBits_16Bit(text);
    }
}

void Intermediary::addImage(QString imagePath)
{
    images->insert(imagePath, QImage(imagePath));
}

long Intermediary::availablePixel()
{
    long result = 0;
    foreach (const QImage &image, images->values())
    {
        result += (image.width() * image.height()) - 160;
    }
    return result;
}

void Intermediary::hide_1Bit(QString savePath)
{
    if (isReady_1Bit())
    {
        int sequenceNo = 0;

        long totalPixel = 0;
        long start = 0;
        long end = 0;
        long range = 0;
        long bitRate = (availablePixel() / totalBits());

        QMap<QString, QImage>::const_iterator it = images->constBegin();
        while (it != images->constEnd())
        {
            QImage image = it.value();
            totalPixel = image.width() * image.height();
            start = end;
            range = (totalPixel / bitRate);
            end = start + range;

            QString currentText = bitsToHide->mid(start, end);

            Steganography* stego = new Steganography(it.key());
            stego->insertSizeInHeader(range);
            stego->insertTBFieldInHeader(1);
            stego->insertFirstAttribute(format);
            stego->insertSecondAttribute(0);
            stego->insertBitsPerPixelInHeader(1);
            stego->insertSequenceNoInHeader(sequenceNo);
            stego->insertText_1BitPerPixel(&currentText, format);
            stego->saveImage(savePath + sequenceNo);

            sequenceNo++;
            it++;
        }
    }

    //for (int i = 0; i < pictures->count(); i++)
    //{
        //long currentPosition = (pictures->value(i).height() * pictures->value(i).width()) / avarageBitRate;
        //QString splitPlain = bitsToHide->mid(startPosition, startPosition + currentPosition);

        //Steganography* stego = new Steganography("asd");
        //stego->insertText_1BitPerPixel(&splitPlain, UNICODE);

        // set new start position
        //startPosition = currentPosition + 1;

        // picture numbering
        //QString pictureNumber = pictures->count() > 1 ? (i + 1) + "" : "";
        //stego->saveImage(savePath + pictureNumber);
    //}
}

void Intermediary::hide_3Bit(QString savePath)
{
    if (isReady_3Bit())
    {
        //
    }
}

void Intermediary::hide_6Bit(QString savePath)
{
    if (isReady_6Bit())
    {
        //
    }
}

QString* getHiddenText()
{
    return new QString();
}
