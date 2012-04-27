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
#define MAX_INT 4294967294

Steganography::Steganography(QString filename)
{
    image = QImage(filename);
}



 /*
  fuegt den Text in das Image ein.
  Um den Text gleichmaessig einzufuegen wird berechnet in welchen Abstaenden ein Bit des Textes in einen Pixel einzufuegen ist:
  
  Variable: inkrement
  
  Um den Text schwerer auslesbar zu machen, wird zudem noch zwischen den Rot, Blau und Gruen Werten des Pixels rotiert.
  
  Eingabe: QString* text -> zu versteckender Text
            int format -> 0 bedeutet ASCII, 1 bedeutet Unicode
  Ausgabe: Integer
            1 -> alles ok
            -1 -> es wurde ein unbekanntes Texformat angegeben
            -2 -> der text ist zu lang fuer das Bild und die Versteck_methode
            -3 -> Laenge des Texts kann nicht im Header abgespeichert werden

 */
int Steganography::insertText(QString* text, int format){
    QString* bitsToInsert;
    if(format == ASCII){
        bitsToInsert = BitChanger::textToBits_8Bit(text);
    }else if(format == UNICODE){
        bitsToInsert = BitChanger::textToBits_16Bit(text);
    }else{
        return -1; //unbekanntes Textformat uebergeben
    }
 /*   if(bitsToInsert->size() > std::numeric_limits<qint32>::max()){
        return -3; // TextGroesse zu gross, um in den Header eingetragen zu werden
    }
*/
    insertTextHeader(bitsToInsert->size(), format);

    int width = image.width();
    int height = image.height();

    int inkrement = (width * (height-1)) / bitsToInsert->size();

    if(inkrement >=1){

        int errorCode = insertBitstream(bitsToInsert);
        if(errorCode != 1){
            return errorCode;
        }

        return 1; //ok
    }else{
        return -2; //text zu lang
   }
}


/*
 fuegt eine Bitfolge in das Bild ein

 Eingabe: QString* text -> zu versteckender Text (muss eine Bitfolge sein)
 Ausgabe: Integer
           1 -> alles ok
           -1 -> bei der Farbauswahl ist ein Fehler aufgetreten
           -2 -> der text ist zu lang fuer das Bild und die Versteck_methode


*/
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
        return -2; //Text zu lang
   }
}




/*
  extrahiert die enthaltene Bitfolge aus dem Bild
  Eingabe: /
  Ausgabe: QString* -> die extrahierte Bitfolge
  */
QString* Steganography::getBitStream(){
    int width = image.width();
    int height = image.height();

    int inkrement = (width * (height-1)) / getIntFromHeader(0);

    int currentColor = RED;
    int color;
    int line = -1;
    QString* lastBits = new QString();

    for(int pixelindex = inkrement-1; pixelindex < getIntFromHeader(0) * inkrement; pixelindex += inkrement){
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

/*
  holt das erste Attribut aus dem Header
  Eingabe: /
  Ausgabe: int -> der Wert, der im ersten Attribute-Feld eingetragen ist
  */
int Steganography::getFirstAttributeFromHeader(){
    return getIntFromHeader(33);
}


/*
  holt das zweite Attribut aus dem Header
  Eingabe: /
  Ausgabe: int -> der Wert, der im zweiten Attribute-Feld eingetragen ist
  */
int Steganography::getSecondAttributeFromHeader(){
    return getIntFromHeader(65);
}


/*
  extrahiert den versteckten Text aus dem Bild. In der Methode wird der Header ausgelesen und erkannt, ob es sich um ASCII oder
  Unicode Zeichen handelt. Es darf aber kein Bild uebergeben, in dem ein Bild versteckt ist
  Eingabe: /
  Ausgabe: QString* -> der extrahierte Text
            im Fehlerfall den leeren String;
  */
QString* Steganography::getHiddenText(){
    QString* bitStream = getBitStream();
    if(getFirstAttributeFromHeader() == ASCII){
        return BitChanger::bitStreamToText_8Bit(bitStream);
    }else if(getFirstAttributeFromHeader() == UNICODE ){
        return BitChanger::bitStreamToText_16Bit(bitStream);
    }else{
        return new QString();
    }
}


/*
  fuegt einen Header in das Bild ein, wobei das Bit, das angibt, ob es sich bei der versteckten Datei um ein Bild oder einen Text handelt,
  auf "Text" gesetzt wird
  Eingabe: int bits -> die Anzahl der zu versteckenden Bits; int format -> 1 == unicode, 0 == ASCII
  Ausgabe: irrelevant
  */
int Steganography::insertTextHeader(int bits, int format){
    if(image.width() < 97) return -1; //Bild zu klein

    insertSizeInHeader(bits); //Anzahl_Bits-Feld

    //in pixel 32 eintragen, dass Text versteckt
    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));
    int red = qRed(pixel[32]);
    red = BitChanger::changeLastBit(red,'1');
    pixel[32] = qRgb(red,qGreen(pixel[32]),qBlue(pixel[32]));

    insertFirstAttribute(format); //ASCII oder Unicode eintragen

    return 1; //alles gut
}


/*
  fuegt einen Header in das Bild ein, wobei das Bit, das angibt, ob es sich bei der versteckten Datei um ein Bild oder einen Text handelt,
  auf "Bild" gesetzt wird
  Eingabe: int bits -> die Anzahl der zu versteckenden Bits; int height -> die Hoehe des versteckten Bildes
            int width -> die Breite des versteckten Bildes
  Ausgabe: -1 -> Fehler
            1 -> ok
  */
int Steganography::insertPictureHeader(int bits, int height, int width){
    if(insertSizeInHeader(bits) != 1) return -1; //Fehler
    //in pixel 32 eintragen, dass Bild versteckt
    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));
    int red = qRed(pixel[32]);
    red = BitChanger::changeLastBit(red,'0');
    pixel[32] = qRgb(red,qGreen(pixel[32]),qBlue(pixel[32]));
    if(insertFirstAttribute(height)!= 1) return -1; //Fehler
    if(insertSecondAttribute(width)!= 1) return -1;
    return 1;
}

/*
  fuegt den uebergebenen Wert in Attributfeld_1 des Headers ein
  Eingabe: i -> der einzutragende Wert
  Ausgabe: int -> falls 1, kein Fehler
            sonst Fehler
  */
int Steganography::insertFirstAttribute(int i){
    return insertIntInHeader(i, 33);
}

/*
  fuegt den uebergebenen Wert in Attributfeld_2 des Headers ein
  Eingabe: i -> der einzutragende Wert
  Ausgabe: int -> falls 1, kein Fehler
            sonst Fehler
  */
int Steganography::insertSecondAttribute(int i){
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
  Ausgabe: int -> 1 == ok; -1 bedeutet Fehler
  */
int Steganography::insertSizeInHeader(int size){
    return insertIntInHeader(size,0);
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
    qDebug("Red bei Auslesen: %i", qRed(pixel[32]));
    return qRed(pixel[32])%2;
}

bool Steganography::imageIsNull(){
    return image.isNull();
}

bool Steganography::saveImage(QString file){
    return image.save(file, "PNG");
}






















