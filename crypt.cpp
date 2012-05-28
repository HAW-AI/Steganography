#include "crypt.h"
#include "QString"
#include "QTextCodec"
#include <QDebug>

#define UNICODE 1
#define ASCII 0
#define ENCRYPT 1
#define DECRYPT 0

Crypt::Crypt(QString* text, QString* key, int format)
    :text(text),key(key),format(format)
{}

void Crypt::caesar(int mode)
{
    int k;
    if (mode == ENCRYPT)k = key->at(0).toAscii();
    else k = (key->at(0).toAscii())*-1; //DECRYPT
    qDebug()<<format;
    qDebug()<<text->toUtf8();
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
                    //ausschluss "komischer zeichen" (z.B. DEL)
                while (l < 0) l += 94;
                while (l > 94) l -= 94;
                l += ' ';
            }
            text->replace(i,1, QChar(l).toAscii());
        }
    }
    qDebug()<<text->toUtf8();
}

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

