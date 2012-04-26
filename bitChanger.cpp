#include "bitChanger.h"
#include "QString"
#include "stringiterator.h"
#include "iterator"

#define BITS_PER_LETTER 16

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

QString BitChanger::toBits(int i,int size){
    QString result = "";
    while(i > 0){
        if(i % 2 == 0){
            result.prepend("0");
        }else{
            result.prepend("1");
            i -= 1;
        }
        i = i /2;
    }

    // auffuellen mit fuehrenden Nullen, falls noetig

    while(result.size() < size){
        result.prepend("0");
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
uint BitChanger::toIntVal(QString s){
    int result = 0;
    int faktor = 1;
    for(int i = s.size() -1; i >= 0;i--){
        if(s.at(i) == '1'){
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
*/
int BitChanger::changeLastBits(int val,QString c, int start, int end){
    QString bits = c.remove(0,start-1).remove(end-start+1,BITS_PER_LETTER -end);
    QString valBits = BitChanger::toBits(val);
    //anzahl der zu aendernden bits = end- start +1
    return BitChanger::toIntVal(valBits.replace(BITS_PER_LETTER-(end-start+1),end-start+1,bits));

}

/*
  ersetzt das letzte Bit von value durch das angegebenen bit
  Bsp.: changeLastBit(15,'0') => 14
        changeLastBit(6,1) => 7
        changeLastBit(7,1) => 7
  */
int BitChanger::changeLastBit(int value, QChar bit){
    QString valBits = BitChanger::toBits(value);
    return BitChanger::toIntVal(valBits.replace(BITS_PER_LETTER-1,1,bit));
}



