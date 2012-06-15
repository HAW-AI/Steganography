#include "bitChanger.h"
#include "intermediary.h"
#include "steganography.h"

#include "iostream"
#include "QImage"
#include "QList"
#include "QMap"

//********************************************************************
//
// Class: Intermediary
// Authors: Raimund Wege, Kathrin Kahlhoefer
//
// Purpose: connects GUI, BitChanger and Steganography
//          tests if text/image to hide fits into chosen picture and 
//          collects more pictures or changes the amount of bits that
//          are hidden in one pixel if necessary
//
//********************************************************************

#define LINE "==============================="
#define HEADER_SIZE 194
#define INT_SIZE 32

using namespace std;

//********************************************************************
//
// Method: Constructor
// Parameter: QString imagePath, path of the image in which the data shall be hidden
// Purpose: construction
//
//********************************************************************
Intermediary::Intermediary(QString imagePath) 
{
    bitChanger = new BitChanger();
    images = new QMap<QString, QImage>();
    addImage(imagePath);
    setText(new QString(), 0);
}

//********************************************************************
//
// Method: setText
// Parameter: QString text, text to hide
//            int format, format of the text, ASCII=0, UNICODE=1
// Purpose: sets type, format, textToHide and intsToHide according to the chosen text
//
//********************************************************************
void Intermediary::setText(QString* text, int format) 
{
    this->type = 1;
    this->format = format;
    this->textToHide = text;
    if (format == 0) 
    {
        this->intsToHide = bitChanger->textToBitsInIntList_8Bit(this->textToHide);
    } else 
    {
        this->intsToHide = bitChanger->textToBitsInIntList_16Bit(this->textToHide);
    }
}

//********************************************************************
//
// Method: setImage
// Parameter: QString imagePath, path of the image to hide
// Purpose: sets type, imageToHide and intsToHide according to the chosen image
//
//********************************************************************
void Intermediary::setImage(QString imagePath) 
{
    this->type = 0;
    this->imageToHide = QImage(imagePath);
    this->intsToHide = bitChanger->pictureToBitstreamAsIntList(&(this->imageToHide));
}

//********************************************************************
//
// Method: addImage
// Parameter: QString imagePath, path of the image to add
// Purpose: adds an image to the map of images in which the data shall be hidden
//
//********************************************************************
bool Intermediary::addImage(QString imagePath) 
{
    std::cout<<endl<<LINE<<endl;
    bool result = false;
    QImage image = QImage(imagePath);
    if (availableInts_1Bit(image) > 0) 
    {
        images->insert(imagePath, image);
        result = true;
        std::cout<<"image added"<<endl;
    } else 
    {
        std::cout<<"image not added, because the image is to small"<<endl;
    }
    std::cout<<LINE<<endl;
    return result;
}

//********************************************************************
//
// Method: availableInts_1Bit
// Parameter: 
// Purpose: calculates the number of ints that can be hidden in the set
//          of images when 1 Bit is hidden per pixel
//
//********************************************************************
long Intermediary::availableInts_1Bit() 
{
    long result = 0;
    foreach (const QImage &image, images->values()) 
    {
        result += availableInts_1Bit(image);
    }
    return result;
}

//********************************************************************
//
// Method: availableInts_1Bit
// Parameter: QImage image, image to examine
// Purpose: calculates the number of ints that can be hidden in this
//          image when 1 Bit is hidden per pixel
//
//********************************************************************
long Intermediary::availableInts_1Bit(QImage image) 
{
    long result = 0;
    if (image.width() >= HEADER_SIZE && image.height() > 1) 
    {
        result = (image.width() * (image.height() - 1)) / INT_SIZE;
    }
    return result;
}

//********************************************************************
//
// Method: hide_1Bit
// Parameter: QString savePath, path to save the resulting image
// Purpose: hides data using 1 Bit per pixel
//
//********************************************************************
void Intermediary::hide_1Bit(QString savePath) 
{
    hide(isReady_1Bit(), availableInts_1Bit(), 1, savePath);
}

//********************************************************************
//
// Method: hide_3Bit
// Parameter: QString savePath, path to save the resulting image
// Purpose: hides data using 3 Bit per pixel
//
//********************************************************************
void Intermediary::hide_3Bit(QString savePath) 
{
    hide(isReady_3Bit(), availableInts_3Bit(), 3, savePath);
}

//********************************************************************
//
// Method: hide_6Bit
// Parameter: QString savePath, path to save the resulting image
// Purpose: hides data using 6 Bit per pixel
//
//********************************************************************
void Intermediary::hide_6Bit(QString savePath) 
{
    hide(isReady_6Bit(), availableInts_6Bit(), 6, savePath);
}

//********************************************************************
//
// Method: hide
// Parameter: bool isReady, certifies if hiding is possible
//            long availableInts, maximum number of ints that can be hidden
//            int bitsPerPixel, 1, 3 or 6 bits can be hidden per pixel
//            QString savePath, path to save the resulting image
// Purpose: hides data
//
//********************************************************************
void Intermediary::hide(bool isReady, long availableInts, int bitsPerPixel, QString savePath) 
{
    std::cout<<endl<<LINE<<endl<<"is ready? ";
    if (!isReady) 
    {
        std::cout<<"no"<<endl;
    } else 
    {
        std::cout<<"yes"<<endl;

        int sequenceNo = 1;
        int realSize = 0;
        long totalInts = 0;
        long start = 0;
        long range = 0;
        long intRate = (availableInts / intsToHide->size());

        std::cout<<"type: "<<type<<endl;
        std::cout<<"availableInts: "<<availableInts<<endl;
        std::cout<<"neededInts: "<<intsToHide->size()<<endl;
        if (type == 1) 
        {
            std::cout<<"textToHide: "<<textToHide->toStdString()<<endl;
            std::cout<<"textSize: "<<textToHide->size()<<endl;
            std::cout<<"format: "<<format<<endl;
        } else 
        {
            std::cout<<"imageWidth: "<<imageToHide.width()<<endl;
            std::cout<<"imageHeight: "<<imageToHide.height()<<endl;
        }
        std::cout<<"intRate: "<<intRate<<endl;

        QMap<QString, QImage>::const_iterator it = images->constBegin();
        while (it != images->constEnd() && range >= 0) 
        {
            QImage image = it.value();
            if (bitsPerPixel == 1) 
            {
                totalInts = availableInts_1Bit(image);
            } else if (bitsPerPixel == 3) 
            {
                totalInts = availableInts_3Bit(image);
            } else 
            {
                totalInts = availableInts_6Bit(image);
            }
            start = start + range;
            range = (totalInts / intRate);

            // if another int fits, add it in order to
            // improve the rounding of the division
            if (totalInts % intRate != 0) range++;

            // for the last image set range to -1 in order that
            // the list.mid method gets all remaining ints
            if (start + range >= intsToHide->size()) range = -1;

            std::cout<<LINE<<endl;
            std::cout<<"totalInts: "<<totalInts<<endl;
            std::cout<<"start: "<<start<<endl;
            std::cout<<"range: "<<range<<endl;

            QList<uint> currentInts = intsToHide->mid(start, range);
            Steganography* stego = new Steganography(it.key());
            stego->insertSizeInHeader(currentInts.size() * INT_SIZE);
            stego->insertTBFieldInHeader(type);
            if (type == 1) 
            {
                stego->insertFirstAttribute(format);
                stego->insertSecondAttribute(0);
                realSize = (currentInts.size() * INT_SIZE) / bitsPerChar();
                if (range < 0) 
                {
                    int sub1 = (format == 0 ? 4 : 2);           // with ASCII 4 Char fit into one Int, with UNICODE only 2
                    int sub2 = textToHide->size() % sub1;       // calculate how many Char habe to be in the last int
                    int sub3 = (sub2 != 0 ? sub1 - sub2 : 0);   // subtract the Char that aren't used
                    realSize -= sub3;
                }
                stego->insertRealSizeInHeader(realSize);
            } else 
            {
                stego->insertFirstAttribute(imageToHide.height());
                stego->insertSecondAttribute(imageToHide.width());
            }
            stego->insertBitsPerPixelInHeader(bitsPerPixel);
            stego->insertSequenceNoInHeader(sequenceNo);

            std::cout<<"sequenceNo: "<<sequenceNo<<endl;
            std::cout<<"size: "<<currentInts.size() * INT_SIZE<<endl;
            std::cout<<"realSize: "<<realSize<<endl;
            int result = 0;
            if (bitsPerPixel == 1) 
            {
                std::cout<<"bitsPerPixel: "<<1<<endl;
                result = stego->insertBitstream(&currentInts);
            } else if (bitsPerPixel == 3) 
            {
                std::cout<<"bitsPerPixel: "<<3<<endl;
                result = stego->insertBitstream_3BitsPerPixel(&currentInts);
            } else 
            {
                std::cout<<"bitsPerPixel: "<<6<<endl;
                result = stego->insertBitstream_6BitsPerPixel(&currentInts);
            }
            std::cout<<"result: "<<result<<endl;

            QString fileName = savePath;
            if (images->size() > 1)
            {
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

//********************************************************************
//
// Method: getHiddenText
// Parameter: 
// Purpose: extracts hidden text from the set of images
//
//********************************************************************
QString* Intermediary::getHiddenText() 
{
    std::cout<<endl<<LINE<<endl;
    std::cout<<"images: "<<images->size()<<endl;

    QString* result = new QString();
    if (images->size() > 0) 
    {
        // With QMap, the items are always sorted by key!
        QMap<int, QString*>* textMap = new QMap<int, QString*>();
        if(isTextHidden()) 
        {
            // get all parts of the text
            QMap<QString, QImage>::const_iterator it = images->constBegin();
            while (it != images->constEnd()) 
            {
                Steganography* stego = new Steganography(it.key());
                int realSize = stego->getRealSizeFromHeader();

                std::cout<<"realSize: "<<realSize<<endl;

                QString* currentText = new QString();
                QList<uint>* bits;
                int bitsPerPixel = stego->getBitsPerPixelFromHeader();
                if (bitsPerPixel == 1) 
                {
                    bits = stego->getBitStreamAsIntList();
                } else if (bitsPerPixel == 3) 
                {
                    bits = stego->getBitstreamAsIntList_3BitsPerPixel();
                } else 
                {
                    bits = stego->getBitstreamAsIntList_6BitsPerPixel();
                }

                if (stego->getFirstAttributeFromHeader() == 0) 
                {
                    currentText = bitChanger->bitStreamToText_8Bit(bits, realSize);
                } else 
                {
                    currentText = bitChanger->bitStreamToText_16Bit(bits, realSize);
                }

                std::cout<<"currentText: "<<currentText->toStdString()<<endl;

                textMap->insert(stego->getSequenceNoFromHeader(), currentText);
                it++;
            }
        }

        // put all parts of the text together
        QMap<int, QString*>::const_iterator it2 = textMap->constBegin();
        while (it2 != textMap->constEnd()) 
        {
            result->append(it2.value());
            it2++;
        }
    } else 
    {
        std::cout<<"no images available"<<endl;
    }
    std::cout<<"result: "<<result->toStdString()<<endl;
    std::cout<<LINE<<endl;
    return result;
}

//********************************************************************
//
// Method: getHiddenImage
// Parameter: 
// Purpose: extracts the hidden image from the set of images
//
//********************************************************************
QImage* Intermediary::getHiddenImage()
{
    std::cout<<endl<<LINE<<endl;
    std::cout<<"images: "<<images->size()<<endl;

    QImage* result = new QImage();
    if (images->size() > 0 && isImageHidden()) 
    {
        QList<uint>* bits = new QList<uint>();
        QMap<QString, QImage>::const_iterator it = images->constBegin();

        //get height and width of the picture
        Steganography* stego = new Steganography(it.key());
        int height = stego->getFirstAttributeFromHeader();
        int width = stego->getSecondAttributeFromHeader();
        int bitsPerPixel = stego->getBitsPerPixelFromHeader();
        std::cout<<"imageSize: "<<width<<" x "<<height<<endl;

        // get all bits of the picture
        while (it != images->constEnd()) 
        {
            Steganography* stego = new Steganography(it.key());
            QList<uint>* stream;
            if (bitsPerPixel == 1) 
            {
                stream = stego->getBitStreamAsIntList();
            } else if (bitsPerPixel == 3) 
            {
                stream = stego->getBitstreamAsIntList_3BitsPerPixel();
            } else 
            {
                stream = stego->getBitstreamAsIntList_6BitsPerPixel();
            }
            bits->append(*stream);
            it++;
        }
        result = bitChanger->bitStreamToPicture(bits, height, width);
    } else 
    {
        std::cout<<"no images available"<<endl;
    }
    std::cout<<LINE<<endl;
    return result;
}

//********************************************************************
//
// Method: imageOrTextHidden
// Parameter: 
// Purpose: gets the hidden type from the header, returns 1 if text is
//          hidden and 0 if image is hidden
//
//********************************************************************
int Intermediary::imageOrTextHidden() 
{
    std::cout<<LINE<<endl;
    int result = -1;
    if (images->size() > 0) 
    {
        Steganography* stego = new Steganography(images->constBegin().key());
        result = stego->getFormatFromHeader();
        std::cout<<"format: "<<result<<endl;
    } else 
    {
        std::cout<<"no images available"<<endl;
    }
    std::cout<<LINE<<endl;
    return result;
}

//********************************************************************
//
// Method: isTextHidden
// Parameter: 
// Purpose: analyzes the header data on logically reasonable values for
//          the case that a text is supposed to be hidden in the image(s)       
//
//********************************************************************
bool Intermediary::isTextHidden() 
{
    bool result = true;
    QMap<QString, QImage>::const_iterator it = images->constBegin();
    while (it != images->constEnd()) 
    {
        QImage image = it.value();
        Steganography* stego = new Steganography(it.key());
        int bitsPerPixel = stego->getBitsPerPixelFromHeader();
        int maxBits = (image.width()*(image.height()-1)*bitsPerPixel);
        if (stego->getFormatFromHeader() != 1) 
        {
            result = false;
            std::cout<<"no text hidden, T/B-Field != 1"<<endl;
        } else if (bitsPerPixel != 1 && bitsPerPixel != 3 && bitsPerPixel != 6) 
        {
            result = false;
            std::cout<<"no text hidden, bitsPerPixel = "<<bitsPerPixel<<endl;
        } else if (stego->getSizeFromHeader() > maxBits)
        {
            result = false;
            std::cout<<"no text hidden, expected number of hidden bits don't fit into image"<<endl;
        } else 
        {
            int realSize = stego->getRealSizeFromHeader();
            int maxRealSize = (maxBits/INT_SIZE) * (stego->getFirstAttributeFromHeader() == 0 ? 4 : 2);
            result = realSize <= maxRealSize;
            std::cout<<"realSize = "<<realSize<<", maxRealSize = "<<maxRealSize<<endl;
        }
        it++;
    }
    std::cout<<"text Hidden? "<<result<<endl;
    return result;
}

//********************************************************************
//
// Method: isImageHidden
// Parameter: 
// Purpose: analyzes the header data on logically reasonable values for
//          the case that an image is supposed to be hidden in the image(s)       
//
//********************************************************************
bool Intermediary::isImageHidden() 
{
    bool result = true;
    QMap<QString, QImage>::const_iterator it = images->constBegin();
    int totalBits = 0;
    int totalMaxBits = 0;
    Steganography* stego = new Steganography(it.key());
    while (it != images->constEnd()) 
    {
        QImage image = it.value();
        stego = new Steganography(it.key());
        int bitsPerPixel = stego->getBitsPerPixelFromHeader();
        if (stego->getFormatFromHeader() != 0) 
        {
            result = false;
            std::cout<<"no image hidden, T/B-Field != 0"<<endl;
        } else if (bitsPerPixel != 1 && bitsPerPixel != 3 && bitsPerPixel != 6) 
        {
            result = false;
            std::cout<<"no image hidden, bitsPerPixel = "<<bitsPerPixel<<endl;
        }
        totalMaxBits += (image.width()*(image.height()-1)*bitsPerPixel);
        totalBits += stego->getSizeFromHeader();
        it++;
    }
    if (totalBits > totalMaxBits) 
    {
        result = false;
        std::cout<<"no image hidden, totalBits = "<<totalBits<<", maxBits = "<<totalMaxBits<<endl;
    } else if (totalBits != (stego->getFirstAttributeFromHeader()*stego->getSecondAttributeFromHeader()*24)) 
    {
        result = false;
        std::cout<<"no image hidden, totalBits = "<<totalBits<<", expectedBits = "<<(stego->getFirstAttributeFromHeader()*stego->getSecondAttributeFromHeader()*24)<<endl;
    }
    std::cout<<"image Hidden? "<<result<<endl;
    return result;
}
