#include "bitChanger.h"
#include "intermediary.h"
#include "steganography.h"

#include "iostream"
#include "QImage"
#include "QList"
#include "QMap"

#define LINE "==============================="
#define HEADER_SIZE 193
#define INT_SIZE 32

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
    this->type = 1;
    this->format = format;
    this->textToHide = text;
    if (format == 0) {
        this->intsToHide = bitChanger->textToBitsInIntList_8Bit(this->textToHide);
    } else {
        this->intsToHide = bitChanger->textToBitsInIntList_16Bit(this->textToHide);
    }
}

void Intermediary::setImage(QString imagePath) {
    this->type = 0;
    this->imageToHide = QImage(imagePath);
    this->intsToHide = bitChanger->pictureToBitstreamAsIntList(&(this->imageToHide));
}

bool Intermediary::addImage(QString imagePath) {
    bool result = false;
    QImage image = QImage(imagePath);
    if (availableInts_1Bit(image) > 0) {
        images->insert(imagePath, image);
        result = true;
    }
    return result;
}

long Intermediary::availableInts_1Bit() {
    long result = 0;
    foreach (const QImage &image, images->values()) {
        result += availableInts_1Bit(image);
    }
    return result;
}

long Intermediary::availableInts_1Bit(QImage image) {
    long result = 0;
    if (image.width() >= HEADER_SIZE && image.height() > 1) {
        result = (image.width() * (image.height() - 1)) / INT_SIZE;
    }
    return result;
}

void Intermediary::hide_1Bit(QString savePath) {
    std::cout<<endl<<LINE<<endl<<"is ready? ";
    if (!isReady_1Bit()) {
        std::cout<<"no"<<endl;
    } else {
        std::cout<<"yes"<<endl;

        int sequenceNo = 1;
        int realSize = 0;
        long totalInts = 0;
        long start = 0;
        long range = 0;
        long intRate = (availableInts_1Bit() / intsToHide->size());

        std::cout<<"textToHide: "<<textToHide->toStdString()<<endl;
        std::cout<<"type: "<<type<<endl;
        std::cout<<"format: "<<format<<endl;
        std::cout<<"availableInts (1Bit): "<<availableInts_1Bit()<<endl;
        std::cout<<"neededInts: "<<intsToHide->size()<<endl;
        std::cout<<"textSize: "<<textToHide->size()<<endl;
        std::cout<<"intRate: "<<intRate<<endl;

        QMap<QString, QImage>::const_iterator it = images->constBegin();
        while (it != images->constEnd() && (start + range) < intsToHide->size()) {
            QImage image = it.value();
            totalInts = availableInts_1Bit(image);
            start = start + range;
            range = (totalInts / intRate);

            // falls noch ein int passt, dann nimm den auch noch
            if (totalInts % intRate != 0) range++;

            // beim letzten Bild setze die Range auf -1 damit bei
            // der list.mid Methode alle restlichen ints gegriffen werden
            if (start + range >= intsToHide->size()) {
                range = -1;
            }

            std::cout<<LINE<<endl;
            std::cout<<"totalInts: "<<totalInts<<endl;
            std::cout<<"start: "<<start<<endl;
            std::cout<<"range: "<<range<<endl;

            QList<uint> currentInts = intsToHide->mid(start, range);
            Steganography* stego = new Steganography(it.key());
            stego->insertSizeInHeader(currentInts.size() * INT_SIZE);
            stego->insertTBFieldInHeader(type);
            if (type == 0) {
                stego->insertFirstAttribute(format);
                stego->insertSecondAttribute(0);
                realSize = (currentInts.size() * INT_SIZE) / bitsPerChar();
                if (range == -1) {
                    int sub1 = (format == 0 ? 4 : 2);           // bei ASCII passen 4 Char in einen Int, bei UNICODE nur 2
                    int sub2 = textToHide->size() % sub1;       // ermittle wie viele Char im letzten Int stecken müssen
                    int sub3 = (sub2 != 0 ? sub1 - sub2 : 0);   // ziehe die Char ab die nicht mehr verwendet werden
                    realSize -= sub3;
                }
                stego->insertRealSizeInHeader(realSize);
            } else {
                stego->insertFirstAttribute(imageToHide.height());
                stego->insertSecondAttribute(imageToHide.width());
            }
            stego->insertBitsPerPixelInHeader(1);
            stego->insertSequenceNoInHeader(sequenceNo);

            std::cout<<"sequenceNo: "<<sequenceNo<<endl;
            std::cout<<"size: "<<currentInts.size() * INT_SIZE<<endl;
            std::cout<<"realSize: "<<(currentInts.size() * INT_SIZE) / bitsPerChar()<<endl;
            int result = stego->insertBitstream(&currentInts);
            std::cout<<"result: "<<result<<endl;

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
    std::cout<<endl<<LINE<<endl<<"is ready? ";
    if (!isReady_3Bit()) {
        std::cout<<"no"<<endl;
    } else {
        std::cout<<"yes"<<endl;

        int sequenceNo = 1;
        int realSize = 0;
        long totalInts = 0;
        long start = 0;
        long range = 0;
        long intRate = (availableInts_3Bit() / intsToHide->size());

        std::cout<<"textToHide: "<<textToHide->toStdString()<<endl;
        std::cout<<"type: "<<type<<endl;
        std::cout<<"format: "<<format<<endl;
        std::cout<<"availableInts (3Bit): "<<availableInts_3Bit()<<endl;
        std::cout<<"neededInts: "<<intsToHide->size()<<endl;
        std::cout<<"textSize: "<<textToHide->size()<<endl;
        std::cout<<"intRate: "<<intRate<<endl;

        QMap<QString, QImage>::const_iterator it = images->constBegin();
        while (it != images->constEnd() && (start + range) < intsToHide->size()) {
            QImage image = it.value();
            totalInts = availableInts_3Bit(image);
            start = start + range;
            range = (totalInts / intRate);

            // falls noch ein int passt, dann nimm den auch noch
            if (totalInts % intRate != 0) range++;

            // beim letzten Bild setze die Range auf -1 damit bei
            // der list.mid Methode alle restlichen ints gegriffen werden
            if (start + range >= intsToHide->size()) {
                range = -1;
            }

            std::cout<<LINE<<endl;
            std::cout<<"totalInts: "<<totalInts<<endl;
            std::cout<<"start: "<<start<<endl;
            std::cout<<"range: "<<range<<endl;

            QList<uint> currentInts = intsToHide->mid(start, range);
            Steganography* stego = new Steganography(it.key());
            stego->insertSizeInHeader(currentInts.size() * INT_SIZE);
            stego->insertTBFieldInHeader(type);
            if (type == 0) {
                stego->insertFirstAttribute(format);
                stego->insertSecondAttribute(0);
                realSize = (currentInts.size() * INT_SIZE) / bitsPerChar();
                if (range == -1) {
                    int sub1 = (format == 0 ? 4 : 2);           // bei ASCII passen 4 Char in einen Int, bei UNICODE nur 2
                    int sub2 = textToHide->size() % sub1;       // ermittle wie viele Char im letzten Int stecken müssen
                    int sub3 = (sub2 != 0 ? sub1 - sub2 : 0);   // ziehe die Char ab die nicht mehr verwendet werden
                    realSize -= sub3;
                }
                stego->insertRealSizeInHeader(realSize);
            } else {
                stego->insertFirstAttribute(imageToHide.height());
                stego->insertSecondAttribute(imageToHide.width());
            }
            stego->insertBitsPerPixelInHeader(3);
            stego->insertSequenceNoInHeader(sequenceNo);

            std::cout<<"sequenceNo: "<<sequenceNo<<endl;
            std::cout<<"size: "<<currentInts.size() * INT_SIZE<<endl;
            std::cout<<"realSize: "<<(currentInts.size() * INT_SIZE) / bitsPerChar()<<endl;
            int result = stego->insertBitstream_3BitsPerPixel(&currentInts);
            std::cout<<"result: "<<result<<endl;

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

void Intermediary::hide_6Bit(QString savePath) {
    std::cout<<endl<<LINE<<endl<<"is ready? ";
    if (!isReady_6Bit()) {
        std::cout<<"no"<<endl;
    } else {
        std::cout<<"yes"<<endl;

        int sequenceNo = 1;
        int realSize = 0;
        long totalInts = 0;
        long start = 0;
        long range = 0;
        long intRate = (availableInts_6Bit() / intsToHide->size());

        std::cout<<"textToHide: "<<textToHide->toStdString()<<endl;
        std::cout<<"type: "<<type<<endl;
        std::cout<<"format: "<<format<<endl;
        std::cout<<"availableInts (6Bit): "<<availableInts_6Bit()<<endl;
        std::cout<<"neededInts: "<<intsToHide->size()<<endl;
        std::cout<<"textSize: "<<textToHide->size()<<endl;
        std::cout<<"intRate: "<<intRate<<endl;

        QMap<QString, QImage>::const_iterator it = images->constBegin();
        while (it != images->constEnd() && (start + range) < intsToHide->size()) {
            QImage image = it.value();
            totalInts = availableInts_6Bit(image);
            start = start + range;
            range = (totalInts / intRate);

            // falls noch ein int passt, dann nimm den auch noch
            if (totalInts % intRate != 0) range++;

            // beim letzten Bild setze die Range auf -1 damit bei
            // der list.mid Methode alle restlichen ints gegriffen werden
            if (start + range >= intsToHide->size()) {
                range = -1;
            }

            std::cout<<LINE<<endl;
            std::cout<<"totalInts: "<<totalInts<<endl;
            std::cout<<"start: "<<start<<endl;
            std::cout<<"range: "<<range<<endl;

            QList<uint> currentInts = intsToHide->mid(start, range);
            Steganography* stego = new Steganography(it.key());
            stego->insertSizeInHeader(currentInts.size() * INT_SIZE);
            stego->insertTBFieldInHeader(type);
            if (type == 0) {
                stego->insertFirstAttribute(format);
                stego->insertSecondAttribute(0);
                realSize = (currentInts.size() * INT_SIZE) / bitsPerChar();
                if (range == -1) {
                    int sub1 = (format == 0 ? 4 : 2);           // bei ASCII passen 4 Char in einen Int, bei UNICODE nur 2
                    int sub2 = textToHide->size() % sub1;       // ermittle wie viele Char im letzten Int stecken müssen
                    int sub3 = (sub2 != 0 ? sub1 - sub2 : 0);   // ziehe die Char ab die nicht mehr verwendet werden
                    realSize -= sub3;
                }
                stego->insertRealSizeInHeader(realSize);
            } else {
                stego->insertFirstAttribute(imageToHide.height());
                stego->insertSecondAttribute(imageToHide.width());
            }
            stego->insertBitsPerPixelInHeader(6);
            stego->insertSequenceNoInHeader(sequenceNo);

            std::cout<<"sequenceNo: "<<sequenceNo<<endl;
            std::cout<<"size: "<<currentInts.size() * INT_SIZE<<endl;
            std::cout<<"realSize: "<<(currentInts.size() * INT_SIZE) / bitsPerChar()<<endl;
            int result = stego->insertBitstream_6BitsPerPixel(&currentInts);
            std::cout<<"result: "<<result<<endl;

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

QString* Intermediary::getHiddenText() {
    std::cout<<endl<<LINE<<endl;
    std::cout<<"images: "<<images->size()<<endl;

    // With QMap, the items are always sorted by key!
    QMap<int, QString*>* textMap = new QMap<int, QString*>();

    // get all parts of the text
    QMap<QString, QImage>::const_iterator it = images->constBegin();
    while (it != images->constEnd()) {
        Steganography* stego = new Steganography(it.key());
        int realSize = stego->getRealSizeFromHeader();

        std::cout<<"realSize: "<<realSize<<endl;

        QString* currentText = new QString();
        QList<uint>* bits = stego->getBitStreamAsIntList();
        if (stego->getFirstAttributeFromHeader() == 0) {
            currentText = bitChanger->bitStreamToText_8Bit(bits, realSize);
        } else {
            currentText = bitChanger->bitStreamToText_16Bit(bits, realSize);
        }

        std::cout<<"currentText: "<<currentText->toStdString()<<endl;

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
    std::cout<<"result: "<<result->toStdString()<<endl;
    std::cout<<LINE<<endl;
    return result;
}

QImage* Intermediary::getHiddenImage()
{
    //TODO
    //QImage fromData(const uchar * data, int size, const char * format = 0)
    //QImage fromData(const QByteArray & data, const char * format = 0)
    return new QImage();
}

int Intermediary::imageOrTextHidden() {
    Steganography* stego = new Steganography(images->constBegin().key());
    return stego->getFormatFromHeader();
}
