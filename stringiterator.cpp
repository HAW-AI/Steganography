#include "stringiterator.h"
#include "QChar"



/*
  Description: creates a Stringitator to iterate over a String
  In: String s -> target String
      int i -> current position (default -1 -> .next() gives char at string[0]
  Out: Stringiterator
  */
Stringiterator::Stringiterator(QString s, int i ){
    if(s.size() != 0 && i >= -1 && i <= s.size()){
        internString = s;
        index = i;
    }else{
        internString = "";
        index = -2;
    }
}

/*
  Description: looks for an existing next element in the String
  void-> bool
  In: /
  Out: bool -> whether there is a next element
  */
bool Stringiterator::hasNext(){
    if(index != -2){
        if(index+1 < internString.size()){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/*
  Description: looks for an existing previous element in the String (if actual index > 0)
  void-> bool
  In: /
  Out: bool -> whether there is a previous element
  */
bool Stringiterator::hasPrevious(){
    if(index == -2) return false;
    if(index < 1) return false;
    return true;
}


/*
  Description: gives the previous element in the String; if index < 1 it returns the Null QChar
  void -> QChar
  In: /
  Out: QChar
  */
QChar Stringiterator::previous(){
    if(index < 1 || index == -2) return QChar();
    return internString.at(--index);
}


/*
  Description: gives the next element in the String; if index > String.soze() it returns the Null QChar
  void -> QChar
  In: /
  Out: QChar
  */
QChar Stringiterator::next(){
    if(index >= internString.size()) return QChar();
    else{
        return internString.at(++index);
    }
}

int Stringiterator::getIndex(){
    return index;
}



QString Stringiterator::getInternString(){
    return internString;
}


