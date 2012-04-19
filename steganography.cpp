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
QString Steganography::insertText(QString text){
    insertHeader(text.size());

    int width = image.width();
    int height = image.height();

    int inkrement = (width * (height-1)) / text.size() / BITS_PER_LETTER;
    qDebug("Inkrement einfuegen: %i",inkrement);

    if(inkrement >= 1){
        insertHeader(text.size());

        Stringiterator textIterator(text); //iteriert ueber den zu versteckenden texts
        Stringiterator bitIterator("");    //iteriert ueber das bitmuster des zu versteckenden buchstabens

        int currentColor = RED;
        int line = -1;



        for(int pixelindex = inkrement-1; pixelindex < text.size()*BITS_PER_LETTER * inkrement; pixelindex += inkrement){
            if((pixelindex / width) + 1 != line){
                line = (pixelindex / width) +1;
            }
            QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(line));

            int pos = (pixelindex % width);


            if(textIterator.hasNext() || bitIterator.hasNext()){
                QChar currentChar;

                if(!(bitIterator.hasNext())){
                    currentChar = textIterator.next();
                    QString bits = BitChanger::toBits(currentChar.unicode());
                    bitIterator = Stringiterator(bits);
                }

                int red = qRed(pixel[pos]);
                int green = qGreen(pixel[pos]);
                int blue = qBlue(pixel[pos]);

                QChar aktBit = bitIterator.next();
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
        }






        return "Alles supi";
    }else{
        return "Inkrement falsch";
   }
}

/*
  fuegt die Headerdatei in das image ein. In der Headerdatei steht die laenge des versteckten Textes (Zeichenzahl),
  die fuer die Extrahierung des Textes wichtig ist. Die Zahl der Zeichen wird in den ersten 32 Pixeln des Bildes versteckt
  (1 Pixel pro bit)
  Eingabe: int textsize -> die laenge des zu versteckenden Strings
  Ausgabe: QString -> gibt evtl. Fehlercodes aus
  */
QString Steganography::insertHeader(int textsize){
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



/*
  liest die versteckte Zahl (Zeichenzahl des versteckten Texts) aus dem Header aus
  Eingabe: /
  Ausgabe: int -> die laenge des versteckten Strings
  */
int Steganography::getHeader(){
    QString lastBits = "";

    QRgb* pixel = reinterpret_cast<QRgb*>(image.scanLine(0));

    int currentColor = RED; // es wird abwechselnd im Rot, Gruen oder Blau-Anteil eines Pixels versteckt

    for(int pos=0; pos <32; pos++){  //die ersten 32 Pixel
        int color;

        /*
          hier wird das letzte Bit aus dem Rot/Gruen/Blau-Anteil des aktuellen Pixels ausgelesen
          und current Color auf die naechste farbe gesetzt
          */
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
            return -2;
        }
        if(color == 0){
            lastBits.append("0");
        }
        else if(color == 1){
            lastBits.append("1");
        }
        else{
            return -1;
        }
    }
    return BitChanger::toIntVal(lastBits);
}

/*
  extrahiert den enthaltenen Text aus dem Bild
  Eingabe: /
  Ausgabe: QString -> der extrahierte Text
  */
QString Steganography::getText(){
    int width = image.width();
    int height = image.height();

    int inkrement = (width * (height-1)) / getHeader() / BITS_PER_LETTER;
    qDebug("Auslesen inkrement: %i", inkrement);

    int currentColor = RED;
    int color;
    int line = -1;
    QString lastBits = "";

    for(int pixelindex = inkrement-1; pixelindex < getHeader()*BITS_PER_LETTER * inkrement; pixelindex += inkrement){
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
                    return "ERROR";
                }
                if(color == 0){
                    lastBits.append("0");
                }
                else if(color == 1){
                    lastBits.append("1");
                }
                else{
                    return "ERROR";
                }
            }
    return bitStreamToText(lastBits);


    }


/*
  berechnet aus einem Bitstream einen Text
  Eingabe: QString bitstream => der Bitstream aus dem ein Text gebildet werden soll. Z.B "100000011000010"
  Ausgabe: QString -> der Text, dem der Bitstream entspricht
  Beispiel: "bitStreamToText("1000000110000010") => "AB", da "10000001" = 65 = "A", da 10000010 = 66 = "B"
  */
QString Steganography::bitStreamToText(QString bitstream){
    qDebug(bitstream.toLatin1());
    if(bitstream.size() % BITS_PER_LETTER != 0){
        qDebug("Fehler im Bitstream");

    }


    QString result = "";
    for(int i = 0; i < bitstream.size(); i+=BITS_PER_LETTER){
        result.append(QChar(BitChanger::toIntVal(bitstream.mid(i,BITS_PER_LETTER))));
    }
    return result;
}

bool Steganography::imageIsNull(){
    return image.isNull();
}

bool Steganography::saveImage(QString file){
    return image.save(file, "PNG");
}






















