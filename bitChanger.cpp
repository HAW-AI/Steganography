#include "bitChanger.h"
#include "QString"
#include "iterator"
#include "qimage.h"
#include "qmath.h"

#define UNICODE_SIZE 16
#define ASCII_SIZE 8
#define PIXEL_SIZE 24
#define INT_SIZE 32
#define MAX_INT 4294967295
#define RED 0
#define GREEN 1
#define BLUE 2

BitChanger::BitChanger()
{
}
/*
   berechnet das Bitmuster des uebergebenen Integers i; bei size kann eine groesse angegeben werden, auf die bei der Umrechnung mit fuehrenden
   Nullen aufgefuellt wird. Default-Wert fuer Size == BITS_PER_LETTER
   Eingabe: Int i -> umzurechnender Wert
        Int size -> Groesse, auf die mit fuerenden Nullen aufgefuellt wird (default == 8)
   Ausgabe: QString -> Bitmuster
   Bsp: toBits(3)  =>  	00000011
        toBits(3,4)   => 0011
 */

QString* BitChanger::toBits(int i,int size){
    QString* result = new QString();
    while(i > 0){
        if(i % 2 == 0){
            result->prepend("0");
        }else{
            result->prepend("1");
            i -= 1;
        }
        i = i /2;
    }


    // auffuellen mit fuehrenden Nullen, falls noetig

    while(result->size() < size){
        result->prepend("0");
    }
    return result;
}

/*
  bekommt einen uint value uebergeben und gibt den Wert des bits an Position position zurueck
  Eingabe: value -> der uint wert, aus dem ein bit extrahiert werden soll
            position -> [0..31] die Position des zu extrahierenden Pixels
  Ausgabe: QChar -> der extrahierte Pixel ('1' oder '0')

  Anmerkung -> wird bei position ein Wert != [0..31] angegeben, fuehrt dies zu fehlern und wird nicht abgefangen
  */
QChar BitChanger::getBitAt(uint value, uint position){
    uint temp = (uint) qPow(2,position);
    if((value & temp) == 0){
        return '0';
    }else{
        return '1';
    }
}

int BitChanger::getBits(int value,int from, int numberOfBits){
    int temp = 0;
    for(int i = 0;i < numberOfBits-1; i++){ // maske erstellen, in maske werden die zu extrahierenden Bits auf 1 gesetzt, rest auf 0
        temp += 1;                          //in for-schleife in den niederwertigen bits so viele einsen erzeugen, wie bist extrahiert werden sollen
        temp = temp << 1;
    }
    if(numberOfBits > 0){   //gehoert mit zu maske erstellen, niederwertigtes bit auf 1 setzen, falls min. 1 bit extrahiert werden soll
        temp+=1;
    }

    //einsen in maske an die stelle shiften, wo bits extrahiert werden sollen
       temp = temp << from;

    // maske fertig erstellt

    //alle bits loeschen ausser den gewuenschten
    value = value & temp;

    //extrahierte bits wieder zurueck auf niederwertigste stelle shiften
   if((from + numberOfBits >= 31) && (numberOfBits > 0) && (numberOfBits < 31)){
       value = value >> 1;
       value = changeBitAt(value,31,'0'); //damit 0en nachgezogen werden
       return value >> from-1;
   }else{
       return value >> from;
   }

}



QString* BitChanger::toBits(uint i,int size){
    QString* result = new QString();
    while(i > 0){
        if(i % 2 == 0){
            result->prepend("0");
        }else{
            result->prepend("1");
            i -= 1;
        }
        i = i /2;
    }


    // auffuellen mit fuehrenden Nullen, falls noetig

    while(result->size() < size){
        result->prepend("0");
    }
    return result;
}


/*
  berechnet aus einem Bitstream einen Text, wobei 16 Bit zu einem Unicode-Zeichen zusammengefasst werden
  Eingabe: QString bitstream => der Bitstream aus dem ein Text gebildet werden soll. Z.B "100000011000010"
  Ausgabe: QString -> der Text, dem der Bitstream entspricht
  Beispiel: "bitStreamToText("0000000001000001") => "A", da "0000000010000001" = 65 = "A"
  */
QString* BitChanger::bitStreamToText_16Bit(QString* bitstream){

    /*if(bitstream->size() % UNICODE != 0){
        qDebug("Fehler im Bitstream");

    }*/


    QString* result = new QString();
    for(int i = 0; i < bitstream->size(); i+=UNICODE_SIZE){
        QString s = bitstream->mid(i, UNICODE_SIZE);
        result->append(QChar(BitChanger::toIntVal(&s)));

    }
    return result;
}

/*
  berechnet aus einem Bitstream einen Text, wobei 16 Bit zu einem Unicode-Zeichen zusammengefasst werden
  Eingabe: QList<uint> bitstream => der Bitstream aus dem ein Text gebildet werden soll.
  Ausgabe: QString -> der Text, dem der Bitstream entspricht

  */
QString* BitChanger::bitStreamToText_16Bit(QList<uint>* bitstream, int characters){
    QString* result = new QString();
    QString* temp;
    QList<uint>::const_iterator iter = bitstream->begin();
    while(iter != bitstream->end()){
        temp = BitChanger::toBits(*iter,32);
        result->append(QChar((BitChanger::toIntVal(&(temp->mid(0,UNICODE_SIZE))))));
        result->append(QChar((BitChanger::toIntVal(&(temp->mid(16,UNICODE_SIZE))))));
        temp->clear();
        iter++;
    }
    if(result->size() > characters){
        result->chop(result->size() - characters);
    }
    delete temp;
    return result;
}

/*
  berechnet aus einem Bitstream einen Text, wobei 8 Bit zu einem Zeichen zusammengefasst werden
  Eingabe: QString bitstream => der Bitstream aus dem ein Text gebildet werden soll. Z.B "100000011000010"
  Ausgabe: QString -> der Text, dem der Bitstream entspricht
  Beispiel: "bitStreamToText("1000000110000010") => "AB", da "10000001" = 65 = "A", da 10000010 = 66 = "B"
  */
QString* BitChanger::bitStreamToText_8Bit(QString* bitstream){

    if(bitstream->size() % ASCII_SIZE != 0){
        qDebug("Fehler im Bitstream");

    }


    QString* result = new QString();
    for(int i = 0; i < bitstream->size(); i+=ASCII_SIZE){
        QString s = bitstream->mid(i, ASCII_SIZE);
        result->append(QChar(BitChanger::toIntVal(&s)));
    }
    return result;
}




/*
   berechnet das Bitmuster eines Textes. Dabei werden ASCII-Werte benutzt
   Eingabe: QString* s -> Pointer auf den String, der umgewandelt werden soll

   Ausgabe: QString* -> Pointer auf den Bitstream
   Bsp: QString str = "A";
        QString* pointer = textToBits_8Bit(&str)
        *pointer -> "01000001"

 */
QString* BitChanger::textToBits_8Bit(QString* s){
    QString::const_iterator i = s->begin();
    QString* result = new QString();
    while(i != s->end()){
        result->append(BitChanger::toBits((*i).toAscii(),8));
        i++;
    }
    return result;
}

/*
   berechnet das Bitmuster eines Textes. Dabei werden ASCII-Werte benutzt
   Eingabe: QString* s -> Pointer auf den String, der umgewandelt werden soll

   Ausgabe: QString* -> Pointer auf den Bitstream
   Bsp: QString str = "A";
        QString* pointer = textToBits_8Bit(&str)
        *pointer -> "01000001"

 */
QList<uint>* BitChanger::textToBitsInIntList_8Bit(QString* s){
    QString::const_iterator i = s->begin();
    QList<uint>* result = new QList<uint>();
    QString temp ="";
    while(i != s->end()){
        temp.append(BitChanger::toBits((*i).toAscii(),8));
        i++;
    }
    return result;
}

int BitChanger::changeLastBits(int value, uint lastBits){
    /*uint lastBitsInverted = MAX_INT;
    qDebug("lastbits: %u", lastBitsInverted);
    value = value & lastBitsInverted;
    qDebug("neuer value: %i", value);
    return value^lastBits;*/
    return 0;
}

/*
   berechnet das Bitmuster eines Textes. Dabei werden Unicode-Werte benutzt
   Eingabe: QString* s -> Pointer auf den String, der umgewandelt werden soll

   Ausgabe: QString* -> Pointer auf den Bitstream
   Bsp: QString str = "A";
        QString* pointer = textToBits_16Bit(&str)
        *pointer -> "0000000001000001"

 */
QString* BitChanger::textToBits_16Bit(QString* s){
    QString* result = new QString();
    QString::const_iterator i = s->begin();
    while(i != s->end()){
        result->append(BitChanger::toBits((*i).unicode()));
        i++;
    }
    return result;
}

/*
   berechnet das Bitmuster eines Textes. Dabei werden Unicode-Werte benutzt
   Eingabe: QString* s -> Pointer auf den String, der umgewandelt werden soll

   Ausgabe: QString* -> Pointer auf den Bitstream
   Bsp: QString str = "A";
        QString* pointer = textToBits_16Bit(&str)
        *pointer -> "0000000001000001"

 */
QList<uint>* BitChanger::textToBitsInIntList_16Bit(QString* s){
    QList<uint>* result = new QList<uint>();
    QString::const_iterator i = s->begin();
    QString* temp = new QString();
    while(i != s->end()){
        temp->append(BitChanger::toBits((*i).unicode()));
        if(temp->size() == 32){
            result->append(BitChanger::toIntVal(temp));
            temp->clear();
        }
        i++;
    }
    if(temp->size() == UNICODE_SIZE){
        temp->append("0000000000000000");
        result->append(BitChanger::toIntVal(temp));

    }
    delete temp;
    return result;
}



/*
  Berechnet aus einem gegebenen Bitmuster den Integerwert (vorzeichenlos -> positiv)
  Eingabe: QString s -> Das Bitmuster
  Ausgabe: int -> der Wert des Bitmusters (vorzeichenlos)
  Bsp.:  toIntVal("1001")  =>  9
     toIntVal("000")   => 0

 */
uint BitChanger::toIntVal(QString* s){
    int result = 0;
    int faktor = 1;
    for(int i = s->size() -1; i >= 0;i--){
        if(s->at(i) == '1'){
            result += faktor;
        }
        faktor *=2;
    }
    return result;

}




/*   ersetzt die letzten n Bits des eingegenen Wertes durch die angegebenen Bits aus QString c
   Eingabe: int val -> der Wert, dessen letzte n Bits geaendert werden
            QString c -> das Bitmuster, aus dem Teile die letzen n bits von val ersetzen sollen
            int start -> Startposition der zu uebernehmenden Bits aus QString
            int end -> Endposition der zu uebernehmenden Bits aus QString

    Rueuckgabe: int -> der geaenderte Wert von val
    Bsp.:  00001111 = 15
    changeLastBits(15,10101010,4,7) ->  10, weil Bitmuster == 00001010

int BitChanger::changeLastBits(int val,QString c, int start, int end){
    QString bits = c.remove(0,start-1).remove(end-start+1,bitsPerLetter -end);
    QString* valBits = BitChanger::toBits(val);
    //anzahl der zu aendernden bits = end- start +1
    QString str = valBits->replace(bitsPerLetter-(end-start+1),end-start+1,bits);
    return BitChanger::toIntVal(&str);

}*/

int BitChanger::changeLastBits(int val, QString* str){
    QString* valBits = BitChanger::toBits(val);
    if(valBits->size() < str->size()) return -1; //ersetzen nicht moeglich, da mehr Zeichen ersetzt werden sollen, als vorhanden sind

    QString result = valBits->replace(valBits->size()-str->size(), str->size(),*str);
    return BitChanger::toIntVal(&result);

}


/*
  ersetzt das letzte Bit von value durch das angegebenen bit
  Bsp.: changeLastBit(15,'0') => 14
        changeLastBit(6,1) => 7
        changeLastBit(7,1) => 7
  */
int BitChanger::changeLastBit(int value, QChar bit){
    if(bit == '1'){
       return value | 1;
    }else{
      return value & (MAX_INT-1);
    }
}

/*
  veraendert bei int val das bit an position pos auf c, c aus {'0','1'}
  */
int BitChanger::changeBitAt(int val ,int pos, QChar c){
    uint  temp = (uint) qPow(2,pos);
    if(c == '1'){
        return val | temp;
    }else{
        return val & (MAX_INT - temp);
    }
}


QString BitChanger::getLastBits(int value, int bits){
    QString* valBits = BitChanger::toBits(value);

    if((valBits->size() < bits) || bits <= 0){
        delete valBits;
        return ""; //Fehler, emptyString zurueck
    }
    QString result = valBits->mid(valBits->size() - bits, bits);

    delete valBits;
    return result;

}

QString* BitChanger::pictureToBitstream(QImage* image){
    qDebug("In Methode");
    QString* result = new QString();
    int red;
    int green;
    int blue;
    qDebug("wenigstens bis hier");
    qDebug("image_height : %i", image->height());

    for(int line = 0; line < image->height(); line++){
        qDebug("line: %i", line);
        QRgb* Pixel = reinterpret_cast<QRgb*>(image->scanLine(line));

        for(int pos = 0; pos < image->width(); pos++){

            red = qRed(Pixel[pos]);
            green = qGreen(Pixel[pos]);
            blue = qBlue(Pixel[pos]);
            result->append(*(BitChanger::toBits(red,8)));
            result->append(*(BitChanger::toBits(green,8)));
            result->append(*(BitChanger::toBits(blue,8)));
        }
    }

    return result;

}

QList<uint>* BitChanger::pictureToBitstreamAsIntList(QImage* image){
    QList<uint>* result = new QList<uint>();
    QString* temp = new QString();
    int red;
    int green;
    int blue;

    for(int line = 0; line < image->height(); line++){
        QRgb* Pixel = reinterpret_cast<QRgb*>(image->scanLine(line));

        for(int pos = 0; pos < image->width(); pos++){

            red = qRed(Pixel[pos]);
            green = qGreen(Pixel[pos]);
            blue = qBlue(Pixel[pos]);
            temp->append(*(BitChanger::toBits(red,8)));
            if(temp->size() == 32){
                result->append(BitChanger::toIntVal(temp));
                temp->clear();
            }
            temp->append(*(BitChanger::toBits(green,8)));
            if(temp->size() == 32){
                result->append(BitChanger::toIntVal(temp));
                temp->clear();
            }
            temp->append(*(BitChanger::toBits(blue,8)));
            if(temp->size() == 32){
                result->append(BitChanger::toIntVal(temp));
                temp->clear();
            }
        }
    }
    delete temp;
    return result;

}
QImage* BitChanger::bitStreamToPicture(QString* stream, int height, int width){
    QImage* result = new QImage(width, height,QImage::Format_RGB32);



    if(stream->size() % 24 != 0){
        return new QImage();

    }
    if((stream->size() / 24) != (result->width() * result->height())){
        return new QImage();
    }

    int line = 0;
    int pos = 0;

    int red;
    int green;
    int blue;
    QRgb* Pixel = reinterpret_cast<QRgb*>(result->scanLine(line));

    for(int i = 0; i < stream->size(); i+=PIXEL_SIZE){
        if(pos >= result->width()){
            pos = 0;
            line++;
            Pixel = reinterpret_cast<QRgb*>(result->scanLine(line));
        }
        if(line < result->height()){
            QString s = stream->mid(i, PIXEL_SIZE);
            red = BitChanger::toIntVal(&(s.mid(0,8)));
            green = BitChanger::toIntVal(&(s.mid(8,8)));
            blue = BitChanger::toIntVal(&(s.mid(16,8)));
            Pixel[pos] = qRgb(red, green, blue);
            pos++;
        }


    }
    return result;


}

QImage* BitChanger::bitStreamToPicture(QList<uint>* stream, int height, int width){
    QImage* result = new QImage(width, height,QImage::Format_RGB32);
    result->fill(0);

    int pixels = height*width;
    int paintedPixels = 0;
    int currentColor = RED;
    QString* temp;

    int line = 0;
    int pos = 0;

    int red;
    int green;
    int blue;

    QRgb* Pixel = reinterpret_cast<QRgb*>(result->scanLine(line));
    QList<uint>::const_iterator iter = stream->begin();

    while(iter != stream->end()){
        if(pos >= result->width()){
            pos = 0;
            line++;
            Pixel = reinterpret_cast<QRgb*>(result->scanLine(line));
        }
        if(line < result->height()){
            temp = BitChanger::toBits(*iter,32);
            for(int i = 0; i < 32; i+= 8){
                if(currentColor == RED){
                    red = BitChanger::toIntVal(&(temp->mid(i,8)));
                    currentColor = GREEN;
                }else if(currentColor == GREEN){
                    green = BitChanger::toIntVal(&(temp->mid(i,8)));
                    currentColor = BLUE;
                }else if(currentColor == BLUE){
                    blue = BitChanger::toIntVal(&(temp->mid(i,8)));
                    currentColor = RED;
                    Pixel[pos] = qRgb(red, green, blue);
                    pos++;
                    paintedPixels++;
                    if(paintedPixels >= pixels){
                        break;
                    }
                }
            }
            temp->clear();
        }
        iter++;
    }

    delete temp;
    return result;


}




