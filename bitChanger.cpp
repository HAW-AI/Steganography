#include "bitChanger.h"
#include "QString"
#include "stringiterator.h"
#include "iterator"

#define UNICODE 16
#define ASCII 8

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
    for(int i = 0; i < bitstream->size(); i+=UNICODE){
        QString s = bitstream->mid(i, UNICODE);
        result->append(QChar(BitChanger::toIntVal(&s)));

    }
    return result;
}

/*
  berechnet aus einem Bitstream einen Text, wobei 8 Bit zu einem Zeichen zusammengefasst werden
  Eingabe: QString bitstream => der Bitstream aus dem ein Text gebildet werden soll. Z.B "100000011000010"
  Ausgabe: QString -> der Text, dem der Bitstream entspricht
  Beispiel: "bitStreamToText("1000000110000010") => "AB", da "10000001" = 65 = "A", da 10000010 = 66 = "B"
  */
QString* BitChanger::bitStreamToText_8Bit(QString* bitstream){

    if(bitstream->size() % ASCII != 0){
        qDebug("Fehler im Bitstream");

    }


    QString* result = new QString();
    for(int i = 0; i < bitstream->size(); i+=ASCII){
        QString s = bitstream->mid(i, ASCII);
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



/*
   ersetzt die letzten n Bits des eingegenen Wertes durch die angegebenen Bits aus QString c
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

}
*/

/*
  ersetzt das letzte Bit von value durch das angegebenen bit
  Bsp.: changeLastBit(15,'0') => 14
        changeLastBit(6,1) => 7
        changeLastBit(7,1) => 7
  */
int BitChanger::changeLastBit(int value, QChar bit){
    QString *valBits = BitChanger::toBits(value);
    QString str = valBits->replace(valBits->size()-1,1,bit);
    return BitChanger::toIntVal(&str);
}



