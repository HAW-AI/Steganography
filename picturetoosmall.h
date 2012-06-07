#ifndef PICTURETOOSMALL_H
#define PICTURETOOSMALL_H

#include <QDialog>

namespace Ui {
class PictureTooSmall;
}

class PictureTooSmall : public QDialog
{
    Q_OBJECT
    
public:
    explicit PictureTooSmall(QWidget *parent = 0);
    ~PictureTooSmall();
    
private:
    Ui::PictureTooSmall *ui;
};

#endif // PICTURETOOSMALL_H
