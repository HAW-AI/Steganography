#ifndef STRINGITERATOR_H
#define STRINGITERATOR_H
#include "QChar"
#include "QString"

class Stringiterator
{
private:
    QString internString;
    int index;
public:
    Stringiterator(QString s, int i = -1);
    bool hasNext();
    QChar next();
    int getIndex();
    QString getInternString();
    QChar previous();
    bool hasPrevious();
};

#endif // STRINGITERATOR_H
