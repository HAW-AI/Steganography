#include "crypt.h"
#include "QString"
#include "QTextCodec"
#include <QDebug>

//********************************************************************
//
// Class: Crypt
// Author: Fenja Harbke
//
// Purpose: Encrypt or Decrypt a text, using popular cryptographic algorithms
//
//********************************************************************

#define UNICODE 1
#define ASCII 0
#define ENCRYPT 1
#define DECRYPT 0

//********************************************************************
//
// Method: Constructor
// Parameter: QString* text, QString* key, int format,
//          all needed information, to en- or decrypt a text
// Purpose: construction
//
//********************************************************************
Crypt::Crypt(QString* text, QString* key, int format)
    :text(text),key(key),format(format)
{}

//********************************************************************
//
// Method: caesar
// Parameter: int mode,
//            says, wheather a text should be en- od decrypted
// Purpose: changes text to a text, en-/decrypted with caesar-chiffre
//
//********************************************************************
void Crypt::caesar(int mode)
{
    int k;
    if (mode == ENCRYPT)k = key->at(0).toAscii();
    else k = (key->at(0).toAscii())*-1; //DECRYPT
    //qDebug()<<text->toUtf8();
    if(format == UNICODE){
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        for(int i = 0; i<text->size();i++)
        {
            int l = text->at(i).unicode()+k;
            text->replace(i,1,QChar(l).unicode());
        }
    }else{ //format == ASCII
        for(int i = 0; i<text->size();i++)
        {
            int l = text->at(i).toAscii();
            if (l >= 32 && l <= 126)
            {
                l -= ' ';
                l += k;
                    //eliminate "strange letters" (DEL, ...)
                while (l < 0) l += 94;
                while (l > 94) l -= 94;
                l += ' ';
            }
            text->replace(i,1, QChar(l).toAscii());
        }
    }
    //qDebug()<<text->toUtf8();
}

//********************************************************************
//
// Method: vigenere
// Parameter: int mode,
//            says, wheather a text should be en- od decrypted
// Purpose: changes text to a text, en-/decrypted with vigenere-chiffre
//
//********************************************************************
void Crypt::vigenere(int mode)
{
    int k;
    if( format == UNICODE){
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        for( int i = 0; i<text->size();i++)
        {
            int l = text->at(i).unicode();
            if(mode == ENCRYPT){
                k = key->at( i % key->size() ).toAscii();
            }else{
                k = (key->at( i % key->size() ).toAscii())*-1;
            }
            text->replace(i,1, QChar(l+k).unicode());
        }
    }else{ //format == ASCII
        for( int i = 0; i<text->size(); i++)
        {
            int l = text->at(i).toAscii();
            if( l >= 32 && l <= 126){
                l-=' ';
                if(mode == ENCRYPT){
                    k = key->at( i % key->size() ).toAscii();
                }else{
                    k = (key->at( i % key->size() ).toAscii())*-1;
                }
                l+= k;
                while (l<0) l += 94;
                while(l > 94) l -= 94;
                l+=' ';
            }
            text->replace(i,1, QChar(l).toAscii());
        }
    }
}

