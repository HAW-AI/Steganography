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
int Steganography::insertText_1BitPerPixel(QString* text, int format){
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

    int inkrement = calcInkrement(width * (height-1),s->size(),1);

    if(inkrement >= 1){


        QString::const_iterator bitIterator = s->begin();    //iteriert ueber die uebergebenen Bits

        int currentColor = RED;
        int line = -1;



        for(int pixelindex = inkrement-1; pixelindex < s->size()* inkrement; pixelindex += inkrement){
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
 fuegt eine Bitfolge in das Bild ein

 Eingabe: QString* text -> zu versteckender Text (muss eine Bitfolge sein)
 Ausgabe: Integer
           1 -> alles ok
           -1 -> bei der Farbauswahl ist ein Fehler aufgetreten
           -2 -> der text ist zu lang fuer das Bild und die Versteck_methode


*/
int Steganography::insertBitstream(QList<uint>* list){
    int width = image.width();
    int height = image.height();

    int inkrement = calcInkrement(width * (height-1),list->size()*32,1);

    if(inkrement >= 1){

        QList<uint>::const_iterator outerIterator = list->begin(); //iteriert ueber die uebergebene liste
        int currentColor = RED;
        int line = -1;


        QString* temp = BitChanger::toBits(*outerIterator,32);    // der aktuelle uint als bitfolge
        QString::const_iterator bitIterator = temp->begin();    //iteriert ueber die aktuelle Bitfolge


        for(int pixelindex = inkrement-1; pixelindex < list->size()*32* inkrement; pixelindex += inkrement){
            if((pixelindex / width) + 1 != line){
                line = (pixelindex / width) +1;
            }

            if(bitIterator == temp->end()){
                if(outerIterator != list->end()){
                    outerIterator++;
                    temp = BitChanger::toBits(*outerIterator,32);
                    bitIterator = temp->begin();
                }else{
                    delete temp;
                    return -2;
                }
            }
            QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

            int pos = (pixelindex % width);



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
                delete temp;
                return -1;   //Fehler bei der Farbauswahl
            }

                pixel[pos] = qRgb(red, green, blue);

            bitIterator++;

        }
        delete temp;
        return 1; //Alles ok
    }else{
        return -2; //Text zu lang
   }
}


/*
 fuegt eine Bitfolge in das Bild ein und nutzt dabei 3 Bit pro Pixel

 Eingabe: QString* text -> zu versteckender Text (muss eine Bitfolge sein)
 Ausgabe: Integer
           1 -> alles ok
           -1 -> bei der Farbauswahl ist ein Fehler aufgetreten
           -2 -> der text ist zu lang fuer das Bild und die Versteck_methode


*/
int Steganography::insertBitstream_3BitsPerPixel(QString* s){
    int width = image.width();
    int height = image.height();
    int bitsPerPixel = 3;

    int inkrement = calcInkrement(width * (height-1),s->size(),bitsPerPixel);

    if(inkrement >= 1){

        QString::const_iterator bitIterator = s->begin();    //iteriert ueber die uebergebenen Bits

        int line = -1;

        for(int pixelindex = inkrement-1; pixelindex < s->size()* inkrement; pixelindex += inkrement){
            if((pixelindex / width) + 1 != line){
                line = (pixelindex / width) +1;
            }
            QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

            int pos = (pixelindex % width);


            if(bitIterator != s->end()){
                int red = qRed(pixel[pos]);
                int green = qGreen(pixel[pos]);
                int blue = qBlue(pixel[pos]);

                red = BitChanger::changeLastBit(red,(*bitIterator));
                bitIterator++;

                if(bitIterator != s->end()){
                    green = BitChanger::changeLastBit(green,(*bitIterator));
                    bitIterator++;
                }

                if(bitIterator != s->end()){
                    blue = BitChanger::changeLastBit(blue,(*bitIterator));
                    bitIterator++;
                }

                pixel[pos] = qRgb(red, green, blue);
            }else{
                return -2; // Fehler bei der Laenge; inkrement falsch berechnet
            }
        }
        return 1; //Alles ok
    }else{
        return -2; //Text zu lang
   }
}

int Steganography::insertBitstream_6BitsPerPixel(QString* s){
    int width = image.width();
    int height = image.height();
    int bitsPerPixel = 6;

    int inkrement = calcInkrement(width * (height-1),s->size(),bitsPerPixel);

    if(inkrement >= 1){

        QString::const_iterator bitIterator = s->begin();    //iteriert ueber die uebergebenen Bits
        QString temp = "";

        int line = -1;

        for(int pixelindex = inkrement-1; pixelindex < s->size()* inkrement; pixelindex += inkrement){
            if((pixelindex / width) + 1 != line){
                line = (pixelindex / width) +1;
            }
            QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

            int pos = (pixelindex % width);


            if(bitIterator != s->end()){
                int red = qRed(pixel[pos]);
                int green = qGreen(pixel[pos]);
                int blue = qBlue(pixel[pos]);

                temp.clear();
                temp.append(*bitIterator);
                bitIterator++;
                temp.append(*bitIterator);

                red = BitChanger::changeLastBits(red, &temp);
                bitIterator++;

                if(bitIterator != s->end()){
                    temp.clear();
                    temp.append(*bitIterator);
                    bitIterator++;
                    temp.append(*bitIterator);
                    green = BitChanger::changeLastBits(green, &temp);
                    bitIterator++;
                }

                if(bitIterator != s->end()){
                    temp.clear();
                    temp.append(*bitIterator);
                    bitIterator++;
                    temp.append(*bitIterator);
                    blue = BitChanger::changeLastBits(blue, &temp);
                    bitIterator++;
                }

                pixel[pos] = qRgb(red, green, blue);
            }else{
                return -2; // Fehler bei der Laenge; inkrement falsch berechnet
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
        inkrement = inkrement = pixels / (bits / bitsPerPixel);
    }
    return inkrement;
}

/*
 fuegt eine Bitfolge in das Bild ein und nutzt dabei 3 Bit pro Pixel

 Eingabe: QString* text -> zu versteckender Text (muss eine Bitfolge sein)
 Ausgabe: Integer
           1 -> alles ok
           -1 -> bei der Farbauswahl ist ein Fehler aufgetreten
           -2 -> der text ist zu lang fuer das Bild und die Versteck_methode


*/
QString* Steganography::getBitstream_3BitsPerPixel(){
    int width = image.width();
    int height = image.height();
    int bitsPerPixel = 3;
    QString* result = new QString();
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
                result->append(lastBit(red));
                extractedLetters++;
            }
            if(extractedLetters < hiddenLetters){
                result->append(lastBit(green));
                extractedLetters++;
            }
            if(extractedLetters < hiddenLetters){
                result->append(lastBit(blue));
                extractedLetters++;
            }else{
                return result;
            }

        }
        return result;

    }
}

QString* Steganography::getBitstream_6BitsPerPixel(){
    int width = image.width();
    int height = image.height();
    int bitsPerPixel = 6;
    QString* result = new QString();
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
                result->append(BitChanger::getLastBits(red, 2));
                extractedLetters+=2;
            }
            if(extractedLetters < hiddenLetters){
                result->append(BitChanger::getLastBits(green, 2));
                extractedLetters+=2;
            }
            if(extractedLetters < hiddenLetters){
                result->append(BitChanger::getLastBits(blue, 2));
                extractedLetters+=2;
            }else{
                return result;
            }

        }

        return result;

    }
}


QChar Steganography::lastBit(int i){
    if(i % 2 == 0){
        return '0';
    }else{
        return '1';
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

    int inkrement = calcInkrement(width * (height-1), getIntFromHeader(0),1);

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
                    (*lastBits) = "Error";
                    return lastBits;
                }
    }
    return lastBits;
}

/*
  extrahiert die enthaltene Bitfolge aus dem Bild
  Eingabe: /
  Ausgabe: QString* -> die extrahierte Bitfolge
  */
QList<uint>* Steganography::getBitStreamAsIntList(){
    int width = image.width();
    int height = image.height();

    int inkrement = calcInkrement(width * (height-1), getIntFromHeader(0),1);

    int currentColor = RED;
    int color;
    int line = -1;
    QString* lastBits = new QString();
    QList<uint>* result = new QList<uint>;

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
                    return new QList<uint>();
                }
                if(color == 0){
                    lastBits->append("0"); 
                }
                else if(color == 1){
                    lastBits->append("1");
                }
                else{
                    return new QList<uint>();
                }
                if(lastBits->size() >= 32){
                    result->append(BitChanger::toIntVal(lastBits));
                    lastBits->clear();
                }
    }
    delete lastBits;
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
  fuegt den uebergebenen Wert in das T/B Pixel des Headers ein
  Eingabe: int --> Text == 1, Bild == 0
  Ausgabe: int -> 1 == ok;
            -1 illegal value
            -2 -> Bild nicht breit genug fuer Header
  */
int Steganography::insertTBFieldInHeader(int size){
    if(image.width() < 160) return -2;
    if(size != 1 || size != 0) return -1;

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
  fuegt einen Header in das Bild ein, wobei das Bit, das angibt, ob es sich bei der versteckten Datei um ein Bild oder einen Text handelt,
  auf "Text" gesetzt wird
  Eingabe: int bits -> die Anzahl der zu versteckenden Bits; int format -> 1 == unicode, 0 == ASCII
  Ausgabe: irrelevant
  */
int Steganography::insertTextHeader(int bits, int format){
    if(image.width() < 160) return -1; //Bild zu klein

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






















