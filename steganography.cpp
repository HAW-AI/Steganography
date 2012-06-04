#include "steganography.h"
#include "QImage"
#include "QColor"
#include "QRgb"
#include "iostream"
#include "bitChanger.h"
#include "stringiterator.h"
#include "qlist.h"

#define RED 0
#define GREEN 1
#define BLUE 2
#define ASCII 0
#define UNICODE 1
#define MAX_INT 4294967295
#define INT_SIZE 32

Steganography::Steganography(QString filename)
{
    image = QImage(filename);
}



/*
 fuegt eine Bitfolge in das Bild ein. Es wird pro Pixel ein Bit versteckt

 Eingabe: QList<uint>* text -> zu versteckender Text (konvertiert in Bitfolge, die als Liste von uint gespeichert ist)
 Ausgabe: Integer
           1 -> alles ok
           -1 -> bei der Farbauswahl ist ein Fehler aufgetreten
           -2 -> der text ist zu lang fuer das Bild und die Versteck_methode
*/
int Steganography::insertBitstream(QList<uint>* list){
    int width = image.width();
    int height = image.height();

    int inkrement = calcInkrement(width * (height-1),list->size()*INT_SIZE,1);

    if(inkrement >= 1){

        QList<uint>::const_iterator outerIterator = list->begin(); //iteriert ueber die uebergebene liste
        int currentColor = RED;
        int line = -1;
        int aktBit = 31;

        for(int pixelindex = inkrement-1; pixelindex < list->size()*INT_SIZE* inkrement; pixelindex += inkrement){
            if((pixelindex / width) + 1 != line){
                line = (pixelindex / width) +1;
            }

            if(aktBit < 0){
                if(outerIterator != list->end()){
                    outerIterator++;
                    aktBit = 31;
                }else{
                    return -2;
                }
            }
            QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

            int pos = (pixelindex % width);



            int red = qRed(pixel[pos]);
            int green = qGreen(pixel[pos]);
            int blue = qBlue(pixel[pos]);

            if(currentColor == RED){
                red = BitChanger::changeLastBit(red,BitChanger::getBitAt(*outerIterator,aktBit--));
                currentColor = GREEN;
            }else if(currentColor == GREEN){
                green = BitChanger::changeLastBit(green,BitChanger::getBitAt(*outerIterator, aktBit--));
                currentColor = BLUE;
            }else if(currentColor == BLUE){
                blue = BitChanger::changeLastBit(blue,BitChanger::getBitAt(*outerIterator,aktBit--));
                currentColor = RED;
            }else{
                return -1;   //Fehler bei der Farbauswahl
            }

                pixel[pos] = qRgb(red, green, blue);



        }
        return 1; //Alles ok
    }else{
        return -2; //Text zu lang
   }
}


/*
 fuegt eine Bitfolge in das Bild ein und nutzt dabei 3 Bit pro Pixel

 Eingabe: QList<uint>* l -> zu versteckende Bitfolge
 Ausgabe: Integer
           1 -> alles ok
           -1 -> bei der Farbauswahl ist ein Fehler aufgetreten
           -2 -> der text ist zu lang fuer das Bild und die Versteck_methode


*/
int Steganography::insertBitstream_3BitsPerPixel(QList<uint>* l){
    int width = image.width();
    int height = image.height();
    int bitsPerPixel = 3;
    int aktBit = 31;

    int inkrement = calcInkrement(width * (height-1),l->size() * INT_SIZE ,bitsPerPixel);

    if(inkrement >= 1){

        QList<uint>::const_iterator outerIterator = l->begin();    //iteriert ueber die uebergebenen Bits

        int line = -1;

        for(int pixelindex = inkrement-1; pixelindex < l->size()* inkrement * INT_SIZE; pixelindex += inkrement){
            if((pixelindex / width) + 1 != line){
                line = (pixelindex / width) +1;
            }
            QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

            int pos = (pixelindex % width);


            if(aktBit < 0){
                if(outerIterator != l->end()){
                    outerIterator++;
                    aktBit = 31;
                }
            }


            if(outerIterator != l->end()){
                int red = qRed(pixel[pos]);
                int green = qGreen(pixel[pos]);
                int blue = qBlue(pixel[pos]);

                red = BitChanger::changeLastBit(red,BitChanger::getBitAt(*outerIterator, aktBit--));

                if(outerIterator != l->end()){
                    if(aktBit < 0){
                        aktBit = 31;
                        outerIterator++;
                    }
                    green = BitChanger::changeLastBit(green,BitChanger::getBitAt(*outerIterator, aktBit--));
                }

                if(outerIterator != l->end()){
                    if(aktBit < 0){
                        aktBit = 31;
                        outerIterator++;
                    }
                    blue = BitChanger::changeLastBit(blue,BitChanger::getBitAt(*outerIterator, aktBit--));
                }

                pixel[pos] = qRgb(red, green, blue);
                if(outerIterator == l->end()){
                }
            }//else{
              //  return -3; // Fehler bei der Laenge; inkrement falsch berechnet
           // }
        }
        return 1; //Alles ok
    }else{
        return -2; //Text zu lang
   }
}

/*
 fuegt eine Bitfolge in das Bild ein und nutzt dabei 6 Bit pro Pixel

 Eingabe: QList<uint>* l -> zu versteckende Bitfolge
 Ausgabe: Integer
           1 -> alles ok
           -1 -> bei der Farbauswahl ist ein Fehler aufgetreten
           -2 -> der text ist zu lang fuer das Bild und die Versteck_methode


*/
int Steganography::insertBitstream_6BitsPerPixel(QList<uint>* l){

    int width = image.width();
    int height = image.height();
    int bitsPerPixel = 6;
    int aktBit = 31;

    int inkrement = calcInkrement(width * (height-1),l->size() * INT_SIZE ,bitsPerPixel);
    qDebug("Inkrement: %i", inkrement);
    if(inkrement >= 1){

        QList<uint>::const_iterator outerIterator = l->begin();    //iteriert ueber die uebergebenen Bits
        QString temp = "";

        int line = -1;

        for(int pixelindex = inkrement-1; pixelindex < l->size()* inkrement * INT_SIZE; pixelindex += inkrement){
            if((pixelindex / width) + 1 != line){
                line = (pixelindex / width) +1;
            }
            QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

            int pos = (pixelindex % width);


            if(outerIterator != l->end()){

                int red = qRed(pixel[pos]);
                int green = qGreen(pixel[pos]);
                int blue = qBlue(pixel[pos]);
                if(aktBit < 0){
                    outerIterator++;
                    aktBit = 31;
                }
                red = BitChanger::changeBitAt(red,1,BitChanger::getBitAt(*outerIterator,aktBit--));
                red = BitChanger::changeBitAt(red,0,BitChanger::getBitAt(*outerIterator,aktBit--));

                if(outerIterator != l->end()){

                    if(aktBit < 0){
                        outerIterator++;
                        aktBit = 31;
                    }
                    green = BitChanger::changeBitAt(green,1,BitChanger::getBitAt(*outerIterator,aktBit--));
                    green = BitChanger::changeBitAt(green,0,BitChanger::getBitAt(*outerIterator,aktBit--));;

                }

                if(outerIterator != l->end()){
                    if(aktBit < 0){
                        outerIterator++;
                        aktBit = 31;
                    }
                    blue = BitChanger::changeBitAt(blue,1,BitChanger::getBitAt(*outerIterator,aktBit--));
                    blue = BitChanger::changeBitAt(blue,0,BitChanger::getBitAt(*outerIterator,aktBit--));
                }

                pixel[pos] = qRgb(red, green, blue);
            }
        }
        return 1; //Alles ok
    }else{
        return -2; //Text zu lang
   }
}

/*
  berechnet das Inkrement
  Eingabe:
    - int pixels : Anzahl der zur Verfuegung stehenden Pixel
    - int bits : Anzahl der zu versteckenden Bits
    - int bitsPerPixel : wie viele Bit werden in einem Pixel versteckt
  Ausgabe:
   -  int -> das inkrement
  */
int Steganography::calcInkrement(int pixels, int bits, int bitsPerPixel){
    int inkrement;
    if((bits % bitsPerPixel) != 0){
        inkrement = (pixels) / ((bits / bitsPerPixel) +1);
    }else{
        inkrement = pixels / (bits / bitsPerPixel);
    }
    return inkrement;
}



/*
 extrahiert eine Bitfolge aus einem Bild. Dabei werden pro Pixel 3 Bit extrahiert

 Eingabe: /
 Ausgabe: QList<uint> -> die extrahierte Bitfolge

*/
QList<uint>* Steganography::getBitstreamAsIntList_3BitsPerPixel(){
    int width = image.width();
    int height = image.height();
    int bitsPerPixel = 3;
    QList<uint>* result = new QList<uint>();
    int aktBit = 31;
    uint listElement = 0;
    int hiddenLetters = getSizeFromHeader();
    int extractedLetters = 0;

    int inkrement = calcInkrement(width*(height-1), hiddenLetters, bitsPerPixel);

    if(inkrement >= 1){

        int line = -1;

        for(int pixelindex = inkrement-1; pixelindex < hiddenLetters* inkrement; pixelindex += inkrement){
            if((pixelindex / width) + 1 != line){
                line = (pixelindex / width) +1;
            }

            QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

            int pos = (pixelindex % width);

            int red = qRed(pixel[pos]);
            int green = qGreen(pixel[pos]);
            int blue = qBlue(pixel[pos]);

            if(extractedLetters < hiddenLetters){
                listElement = BitChanger::changeBitAt(listElement,aktBit,lastBit(red));
                aktBit--;
                if(aktBit < 0){
                    result->append(listElement);
                    aktBit = 31;
                }
                extractedLetters++;
            }
            if(extractedLetters < hiddenLetters){
                listElement = BitChanger::changeBitAt(listElement,aktBit,lastBit(green));
                aktBit--;
                if(aktBit < 0){
                    result->append(listElement);
                    aktBit = 31;
                }
                extractedLetters++;
            }
            if(extractedLetters < hiddenLetters){
                listElement = BitChanger::changeBitAt(listElement,aktBit,lastBit(blue));
                aktBit--;
                if(aktBit < 0){
                    result->append(listElement);
                    aktBit = 31;
                }
                extractedLetters++;
            }else{
                return result;
            }

        }
        return result;

    }
}



/*
 extrahiert eine Bitfolge aus einem Bild. Dabei werden pro Pixel 6 Bit extrahiert

 Eingabe: /
 Ausgabe: Integer
           1 -> alles ok
           -1 -> bei der Farbauswahl ist ein Fehler aufgetreten
           -2 -> der text ist zu lang fuer das Bild und die Versteck_methode


*/
QList<uint>* Steganography::getBitstreamAsIntList_6BitsPerPixel(){
    int width = image.width();
    int height = image.height();
    int bitsPerPixel = 6;
    int aktBit = 31;
    int listElement = 0;
    QList<uint>* result = new QList<uint>();
    int hiddenLetters = getSizeFromHeader();
    int extractedLetters = 0;

    int inkrement = calcInkrement(width*(height-1), hiddenLetters, bitsPerPixel);
    qDebug("Inkrement auslesen = %i", inkrement);

    if(inkrement >= 1){

        int line = -1;

        for(int pixelindex = inkrement-1; pixelindex < hiddenLetters* inkrement; pixelindex += inkrement){
            if((pixelindex / width) + 1 != line){
                line = (pixelindex / width) +1;
            }

            QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

            int pos = (pixelindex % width);

            int red = qRed(pixel[pos]);
            int green = qGreen(pixel[pos]);
            int blue = qBlue(pixel[pos]);

            if(extractedLetters < hiddenLetters){
                listElement = BitChanger::changeBitAt(listElement,aktBit--,BitChanger::getBitAt(red,1));
                listElement = BitChanger::changeBitAt(listElement, aktBit--,BitChanger::getBitAt(red,0));
                if(aktBit < 0){
                    result->append(listElement);
                    aktBit = 31;
                }
                extractedLetters+=2;
            }
            if(extractedLetters < hiddenLetters){
                listElement = BitChanger::changeBitAt(listElement,aktBit--,BitChanger::getBitAt(green,1));
                listElement = BitChanger::changeBitAt(listElement, aktBit--,BitChanger::getBitAt(green,0));
                if(aktBit < 0){
                    result->append(listElement);
                    aktBit = 31;
                }
                extractedLetters+=2;
            }
            if(extractedLetters < hiddenLetters){
                listElement = BitChanger::changeBitAt(listElement,aktBit--,BitChanger::getBitAt(blue,1));
                listElement = BitChanger::changeBitAt(listElement, aktBit--,BitChanger::getBitAt(blue,0));
                if(aktBit < 0){
                    result->append(listElement);
                    aktBit = 31;
                }
                extractedLetters+=2;
            }else{
                return result;
            }

        }

        return result;

    }
    return &QList<uint>();
}


QChar Steganography::lastBit(int i){
    if(i % 2 == 0){
        return '0';
    }else{
        return '1';
    }
}









/*
  extrahiert die enthaltene Bitfolge aus dem Bild. Dabei wird aus einem Pixel ein Bit extrahiert
  Eingabe: /
  Ausgabe: QString* -> die extrahierte Bitfolge
  */
QList<uint>* Steganography::getBitStreamAsIntList(){
    int width = image.width();
    int height = image.height();

    int inkrement = calcInkrement(width * (height-1), getSizeFromHeader(),1);

    int currentColor = RED;
    int color;
    int line = -1;
    int aktBit = 31;
    int listElement = 0;
    QList<uint>* result = new QList<uint>;

    for(int pixelindex = inkrement-1; pixelindex < getSizeFromHeader() * inkrement; pixelindex += inkrement){
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
                    return new QList<uint>();
                }
                if(color == 0){
                   listElement = BitChanger::changeBitAt(listElement,aktBit,'0');
                   aktBit--;
                }
                else if(color == 1){
                    listElement = BitChanger::changeBitAt(listElement,aktBit,'1');
                    aktBit--;
                }
                else{
                    return new QList<uint>();
                }
                if(aktBit < 0){
                    aktBit = 31;
                    result->append(listElement);
                    listElement = 0;
                }
    }
    return result;
}


/*
  holt das erste Attribut aus dem Header
  Eingabe: /
  Ausgabe: int -> der Wert, der im ersten Attribute-Feld eingetragen ist
   im Fehlerfall (Bild nicht breit genug fuer Header) : Ausgabe == -1
  */
int Steganography::getFirstAttributeFromHeader(){
    if(image.width() < 160 ) return -1;
    return getIntFromHeader(33);
}

/*
  fuegt den uebergebenen Wert in das RealSize-Feld des Headers ein
  Eingabe: int realSize -> der einzutragende Wert
  */
int Steganography::insertRealSizeInHeader(int realSize){
    return insertIntInHeader(realSize, 161);
}

/*
  liest den Wert, der im RealSize-Feld des Headers steht, aus
  Eingabe: /
  Ausabe: int -> Wert, der im RealSize-Feld des Headers steht
  */
int Steganography::getRealSizeFromHeader(){
    if(image.width() < 182) return -1;
    return getIntFromHeader(161);
}

/*
  holt den Wert des Anzahl_Bits-Feldes aus dem Header
  Eingabe: /
  Ausgabe: int -> der Wert der in das Anzahl_Bits-Feld eingetragen ist
   im Fehlerfall (Bild nicht breit genug fuer Header) : Ausgabe == -1
  */
int Steganography::getSizeFromHeader(){
    if(image.width() < 160 ) return -1;
    return getIntFromHeader(0);
}

/*
  holt den Bits/Pixel-Wert aus dem Header
  Eingabe: /
  Ausgabe: int -> der Bits/Pixel-Wert
   im Fehlerfall (Bild nicht breit genug fuer Header) : Ausgabe == -1
  */
int Steganography::getBitsPerPixelFromHeader(){
    if(image.width() < 160 ) return -1;
    return getIntFromHeader(97);
}

/*
  holt die Sequenznummer dem Header
  Eingabe: /
  Ausgabe: int -> die ausgelesene Sequenznummer
    im Fehlerfall (Bild nicht breit genug fuer Header) : Ausgabe == -1
  */
int Steganography::getSequenceNoFromHeader(){
    if(image.width() < 160 ) return -1;
    return getIntFromHeader(129);
}


/*
  holt das zweite Attribut aus dem Header
  Eingabe: /
  Ausgabe: int -> der Wert, der im zweiten Attribute-Feld eingetragen ist
        im Fehlerfall (Bild nicht breit genug fuer Header) : Ausgabe == -1
  */
int Steganography::getSecondAttributeFromHeader(){
    if(image.width() < 160 ) return -1;
    return getIntFromHeader(65);
}




/*
  fuegt den uebergebenen Wert in das T/B Pixel des Headers ein
  Eingabe: int --> Text == 1, Bild == 0
  Ausgabe: int -> 1 == ok;
            -1 illegal value
            -2 -> Bild nicht breit genug fuer Header
  */
int Steganography::insertTBFieldInHeader(int size){
    if(image.width() < 160) return -2;
    if(size != 1 && size != 0) return -1;

    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));
    int red = qRed(pixel[32]);
    if(size == 1){
        red = BitChanger::changeLastBit(red,'1');
    }else{
        red = BitChanger::changeLastBit(red,'0');
    }

    pixel[32] = qRgb(red,qGreen(pixel[32]),qBlue(pixel[32]));
    return 1;
}



/*
  fuegt den uebergebenen Wert in Attributfeld_1 des Headers ein
  Eingabe: i -> der einzutragende Wert
  Ausgabe: int -> falls 1, kein Fehler
           -1 -> Fehler beim Einfuegen
           -2 -> Bild nicht breit genug fuer Header
  */
int Steganography::insertFirstAttribute(int i){
     if(image.width() < 160 ) return -2;
    return insertIntInHeader(i, 33);
}

/*
  fuegt den uebergebenen Wert in Attributfeld_2 des Headers ein
  Eingabe: i -> der einzutragende Wert
  Ausgabe: int -> falls 1, kein Fehler
           -1 -> Fehler beim Einfuegen
           -2 -> Bild nicht breit genug fuer Header
  */
int Steganography::insertSecondAttribute(int i){
     if(image.width() < 160 ) return -2;
   return insertIntInHeader(i,65);
}

/*
  fuegt einen int-Wert in einen bestimmten Header-Bereich ein -> begrenzt durch from und to
  Eingabe: int i -> der einzutragende Wert
           int from -> der Index des ersten Pixels, das fuer den Eintrag verwendet werden soll
           int to -> der Index des letzten Pixels, das fuer den Eintrag verwendet werden soll
  Ausgabe: int -> 1 alles ok
               -> -1 Fehler
  */
int Steganography::insertIntInHeader(int i, int from){
    QString* bits = new QString();
    bits = BitChanger::toBits(i,32);
    int to = from + 31;

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
            return -1; //Fehler
        }

        pixel[pos] = qRgb(red, green, blue);
        bitIterator++;
    }
    delete bits;
    return 1; //alles ok

}

/*
  fuegt den uebergebenen Wert in das Anzahl_Bits Feld des Headers ein
  Eingabe: size -> die Anzahl der Bits, die im Treagerbild versteckt werden soll
  Ausgabe: int -> 1 == ok;
            -1 -> Fehler beim Einfuegen
            -2 -> Bild nicht breit genug fuer Header
  */
int Steganography::insertSizeInHeader(int size){
    if(image.width() < 160) return -2;
    return insertIntInHeader(size,0);
}

/*
  fuegt den uebergebenen Wert in das Bits/Pixel-Feld ein
  Eingabe: size -> Anzahl pro Bits, die pro Pixel veraendert wurden
  Ausgabe: int -> 1 == ok;
            -1 -> Fehler beim Einfuegen
            -2 -> Bild nicht breit genug fuer Header
  */
int Steganography::insertBitsPerPixelInHeader(int size){
    if(image.width() < 160) return -2;
    return insertIntInHeader(size,97);
}

/*
  fuegt den uebergebenen Wert in das Sequenznummer-Feld des Headers ein
  Eingabe: size -> die einzutragende Sequenznummer
  Ausgabe: int -> 1 == ok;
            -1 -> Fehler beim Einfuegen
            -2 -> Bild nicht breit genug fuer Header
  */
int Steganography::insertSequenceNoInHeader(int size){
    if(image.width() < 160) return -2;
    return insertIntInHeader(size,129);
}

/*
  extrahiert den Wert, der im uebergebenen Bereich des Header steht
  Eingabe: int from -> Anfang des Header-Bereichs
           int to -> Ende des Header-Bereichs
  Ausgabe:  int -> der ausgelene Wert
  */
int Steganography::getIntFromHeader(int from){
    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));
    int to = from + 31;

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

/*
  liest das FormatBit des Headers aus. 0 == Bild, 1 == Text
  Eingabe: /
  Ausgabe: int -> 1 oder 0
  */
int Steganography::getFormatFromHeader(){
    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));
    return qRed(pixel[32])%2;
}

bool Steganography::imageIsNull(){
    return image.isNull();
}

bool Steganography::saveImage(QString file){
    return image.save(file, "PNG");
}






















