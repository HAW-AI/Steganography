#ifndef NOTENOUGHINFO_H
#define NOTENOUGHINFO_H

#include <QDialog>

namespace Ui {
class NotEnoughInfo;
}

class NotEnoughInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit NotEnoughInfo(QWidget *parent = 0);
    ~NotEnoughInfo();
    
private:
    Ui::NotEnoughInfo *ui;
};

#endif // NOTENOUGHINFO_H
