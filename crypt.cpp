#include "crypt.h"
#include "QString"
#include "QTextCodec"

#define UNICODE 1
#define ASCII 0
#define ENCRYPT 1
#define DECRYPT 0

Crypt::Crypt(QString text, QString key, int format)
    :text(text),key(key),format(format)
{}

QString Crypt::caesar(int mode)
{
    QString plain=text;
    QString cipher="";
    int k;
    if (mode == ENCRYPT)k = key.at(0).toAscii();
    else k = (key.at(0).toAscii())*-1; //DECRYPT

    if(format == UNICODE){
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        for(int i = 0; i<plain.size();i++)
        {
            int l = plain.at(i).unicode();
            cipher[i] = (QChar(l+k).unicode());
        }
    }else{ //format == ASCII
        for(int i = 0; i<plain.size();i++)
        {
            int l = plain.at(i).toAscii();
            if (l >= 32 && l <= 126)
            {
                l -= ' ';
                l += k;
                    //ausschluss "komischer zeichen" (z.B. DEL)
                while (l < 0) l += 94;
                while (l > 94) l -= 94;
                l += ' ';
            }
            cipher[i] = (QChar(l).toAscii());
        }
    }
    return cipher;
}

QString Crypt::vigenere(int mode)
{
    QString plain = text;
    QString cipher = "";
    int k;

    if( format == UNICODE){
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        for( int i = 0; i<plain.size();i++)
        {
            int l = plain.at(i).unicode();
            if(mode == ENCRYPT){
                k = key.at( i % key.size() ).toAscii();
            }else{
                k = (key.at( i % key.size() ).toAscii())*-1;
            }
            cipher[i] = (QChar(l+k).unicode() );
        }
    }else{ //format == ASCII
        for( int i = 0; i<plain.size(); i++)
        {
            int l = plain.at(i).toAscii();
            if( l >= 32 && l <= 126){
                l-=' ';
                if(mode == ENCRYPT){
                    k = key.at( i % key.size() ).toAscii();
                }else{
                    k = (key.at( i % key.size() ).toAscii())*-1;
                }
                l+= k;
                while (l<0) l += 94;
                while(l > 94) l -= 94;
                l+=' ';
            }
            cipher[i] = (QChar(l).toAscii());
        }
    }
    return cipher;
}

