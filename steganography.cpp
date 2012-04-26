#include "steganography.h"
#include "QImage"
#include "QColor"
#include "QRgb"
#include "iostream"
#include "bitChanger.h"
#include "stringiterator.h"

#define RED 0
#define GREEN 1
#define BLUE 2
#define BITS_PER_LETTER 16
#define ASCII 0
#define UNICODE 1

Steganography::Steganography(QString filename)
{
    image = QImage(filename);
}



 /*
  fuegt den Text in das Image ein.
  Um den Text gleichmaessig einzufuegen wird berechnet in welchen Abstaenden ein Bit des Textes in einen Pixel einzufuegen ist:
  
  Variable: inkrement
  
  Um den Text schwerer auslesbar zu machen, wird zudem noch zwischen den Rot, Blau und Gruen Werten des Pixels rotiert.
  
  Eingabe: QString text -> zu versteckender Text
  Ausgabe: "Image" (das veraenderte Bild wird an einer gewuenschten Stelle gespeichert)
 */
QString Steganography::insertText(QString* text, int format){
    QString* bitsToInsert;
    if(format == ASCII){
        bitsToInsert = BitChanger::textToBits_8Bit(text);
    }else if(format == UNICODE){
        bitsToInsert = BitChanger::textToBits_16Bit(text);
    }else{
        return "ERROR Wrong Format";
    }
    insertTextHeader(bitsToInsert->size(), format);

    int width = image.width();
    int height = image.height();

    int inkrement = (width * (height-1)) / bitsToInsert->size();
    qDebug("Inkrement einfuegen: %i",inkrement);

    if(inkrement >=1){

        insertBitstream(bitsToInsert);

        return "Alles supi";
    }else{
        return "Alles falsch";
   }
}



int Steganography::insertBitstream(QString* s){
    int width = image.width();
    int height = image.height();

    int inkrement = (width * (height-1)) / s->size();

    if(inkrement >= 1){


        QString::const_iterator bitIterator = s->begin();    //iteriert ueber die uebergebenen Bits

        int currentColor = RED;
        int line = -1;



        for(int pixelindex = inkrement-1; pixelindex < s->size()*BITS_PER_LETTER * inkrement; pixelindex += inkrement){
            if((pixelindex / width) + 1 != line){
                line = (pixelindex / width) +1;
            }
            QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

            int pos = (pixelindex % width);


            if(bitIterator != s->end()){
                int red = qRed(pixel[pos]);
                int green = qGreen(pixel[pos]);
                int blue = qBlue(pixel[pos]);

                if(currentColor == RED){
                    red = BitChanger::changeLastBit(red,(*bitIterator));
                    currentColor = GREEN;
                }else if(currentColor == GREEN){
                    green = BitChanger::changeLastBit(green,(*bitIterator));
                    currentColor = BLUE;
                }else if(currentColor == BLUE){
                    blue = BitChanger::changeLastBit(blue,(*bitIterator));
                    currentColor = RED;
                }else{
                    return -1;   //Fehler bei der Farbauswahl
                }

                pixel[pos] = qRgb(red, green, blue);
            }else{
                return -2; // Fehler bei der Laenge; inkrement falsch berechnet
            }
            bitIterator++;

        }






        return 1; //Alles ok
    }else{
        return -3; //Text zu lang
   }
}



/*
  fuegt die Headerdatei in das image ein. In der Headerdatei steht die laenge des versteckten Textes (Zeichenzahl),
  die fuer die Extrahierung des Textes wichtig ist. Die Zahl der Zeichen wird in den ersten 32 Pixeln des Bildes versteckt
  (1 Pixel pro bit)
  Eingabe: int textsize -> die laenge des zu versteckenden Strings
  Ausgabe: QString -> gibt evtl. Fehlercodes aus
  */
/*QString Steganography::insertHeader(int textsize){
    if(image.width() < 32) return "Bild zu klein!!!";

    QString bits = BitChanger::toBits(textsize,32);
    Stringiterator bitIterator(bits);
    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));
    QChar aktBit;


    int currentColor = RED;

    for(int pos=0; pos < 32; pos++){
        int red = qRed(pixel[pos]);
        int green = qGreen(pixel[pos]);
        int blue = qBlue(pixel[pos]);


        if(bitIterator.hasNext()){
            aktBit = bitIterator.next();
        }

        if(currentColor == RED){
            red = BitChanger::changeLastBit(red,aktBit);
            currentColor = GREEN;
        }else if(currentColor == GREEN){
            green = BitChanger::changeLastBit(green,aktBit);
            currentColor = BLUE;
        }else if(currentColor == BLUE){
            blue = BitChanger::changeLastBit(blue,aktBit);
            currentColor = RED;
        }else{
            return "Fehler bei der Farbauswahl";
        }

        pixel[pos] = qRgb(red, green, blue);

    }
    return "Yippie!";
    qDebug("Fertig!!!");
}
*/



/*
  extrahiert den enthaltenen Text aus dem Bild
  Eingabe: /
  Ausgabe: QString -> der extrahierte Text
  */
QString* Steganography::getBitStream(){
    int width = image.width();
    int height = image.height();

    int inkrement = (width * (height-1)) / getIntFromHeader(0,31);

    qDebug("Auslesen inkrement: %i", inkrement);

    int currentColor = RED;
    int color;
    int line = -1;
    QString* lastBits = new QString();

    for(int pixelindex = inkrement-1; pixelindex < getIntFromHeader(0,31) * inkrement; pixelindex += inkrement){
        if((pixelindex / width) + 1 != line){
            line = (pixelindex / width) +1;
        }
        QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

        int pos = (pixelindex % width);
                if(currentColor == RED){
                    color = qRed(pixel[pos]) % 2;
                    currentColor = GREEN;
                }else if(currentColor == GREEN){
                    color = qGreen(pixel[pos]) % 2;
                    currentColor = BLUE;
                }else if(currentColor == BLUE){
                    color = qBlue(pixel[pos]) % 2;
                    currentColor = RED;
                }else{
                    delete lastBits;
                    (*lastBits) = "Error";
                    return lastBits;
                }
                if(color == 0){
                    lastBits->append("0");
                }
                else if(color == 1){
                    lastBits->append("1");
                }
                else{
                    delete lastBits;
                    (*lastBits) = "Error";
                    return lastBits;
                }
    }
    return lastBits;
}

int Steganography::getFirstAttributeFromHeader(){
    return getIntFromHeader(33,64);
}

int Steganography::getSecondAttributeFromHeader(){
    return getIntFromHeader(65,96);
}

QString* Steganography::getHiddenText(){
    QString* bitStream = getBitStream();
    if(getFirstAttributeFromHeader() == ASCII){
        return BitChanger::bitStreamToText_8Bit(bitStream);
    }else if(getFirstAttributeFromHeader() == UNICODE ){
        return BitChanger::bitStreamToText_16Bit(bitStream);
    }else{
        /*
          to be implemented : error
          */
    }
}





int Steganography::insertTextHeader(int bits, int format){
    if(image.width() < 97) return -1; //Bild zu klein

    insertSizeInHeader(bits);

    //in pixel 32 eintragen, dass Text versteckt
    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));
    int red = qRed(pixel[32]);
    qDebug("Red vorher: %i", red);
    red = BitChanger::changeLastBit(red,'1');
    qDebug("Red nachher: %i", red);
    pixel[32] = qRgb(red,qGreen(pixel[32]),qBlue(pixel[32]));

    insertFirstAttribute(format);


    return 1; //alles gut
    qDebug("Fertig!!!");
}

int Steganography::insertPictureHeader(int bits, int height, int width){
    if(insertSizeInHeader(bits) != 1) return -2; //Fehler
    //in pixel 32 eintragen, dass Bild versteckt
    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));
    int red = qRed(pixel[32]);
    red = BitChanger::changeLastBit(red,'0');
    pixel[32] = qRgb(red,qGreen(pixel[32]),qBlue(pixel[32]));
    if(insertFirstAttribute(height)!= 1) return -2; //Fehler
    if(insertSecondAttribute(width)!= 1) return -2;
    return 1;
}

int Steganography::insertFirstAttribute(int i){
    return insertIntInHeader(i, 33,64);
}

int Steganography::insertSecondAttribute(int i){
   return insertIntInHeader(i,65,96);
}

int Steganography::insertIntInHeader(int i, int from, int to){
    QString* bits = new QString();
    bits = BitChanger::toBits(i,32);

    QString::const_iterator bitIterator = bits->begin();

    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));

    int currentColor = RED;

    // Anzahl der Bits in den Header einfuegen
    for(int pos=from; pos <= to; pos++){
        int red = qRed(pixel[pos]);
        int green = qGreen(pixel[pos]);
        int blue = qBlue(pixel[pos]);

        if(currentColor == RED){
            red = BitChanger::changeLastBit(red,(*bitIterator));
            currentColor = GREEN;
        }else if(currentColor == GREEN){
            green = BitChanger::changeLastBit(green,(*bitIterator));
            currentColor = BLUE;
        }else if(currentColor == BLUE){
            blue = BitChanger::changeLastBit(blue,(*bitIterator));
            currentColor = RED;
        }else{
            return -2; //Fehler bei der Farbauswahl
        }

        pixel[pos] = qRgb(red, green, blue);
        bitIterator++;
    }
    delete bits;
    return 1; //alles ok

}

int Steganography::insertSizeInHeader(int size){
    insertIntInHeader(size,0,31);
}

int Steganography::getIntFromHeader(int from, int to){
    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));

    int currentColor = RED;
    QString* resultBits = new QString();

    int color = -1;

    for(int pos=from; pos <= to; pos++){
        if(currentColor == RED){
            color = qRed(pixel[pos]) % 2;
            currentColor = GREEN;
        }else if(currentColor == GREEN){
            color = qGreen(pixel[pos]) % 2;
            currentColor = BLUE;
        }else if(currentColor == BLUE){
            color = qBlue(pixel[pos]) % 2;
            currentColor = RED;
        }else{
            return -2; //Fehler bei der Farbauswahl
        }

        if(color == 1){
            resultBits->append("1");
        }else{
            resultBits->append("0");
        }

    }
    int value = BitChanger::toIntVal(resultBits);
    delete resultBits;
    return value; //alles ok
}

int Steganography::getFormatFromHeader(){
    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));
    qDebug("Red bei Auslesen: %i", qRed(pixel[32]));
    return qRed(pixel[32])%2;
}

bool Steganography::imageIsNull(){
    return image.isNull();
}

bool Steganography::saveImage(QString file){
    return image.save(file, "PNG");
}






















