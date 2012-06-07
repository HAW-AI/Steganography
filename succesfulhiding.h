#ifndef SUCCESFULHIDING_H
#define SUCCESFULHIDING_H

#include <QDialog>

namespace Ui {
class SuccesfulHiding;
}

class SuccesfulHiding : public QDialog
{
    Q_OBJECT
    
public:
    explicit SuccesfulHiding(QString savePath, QWidget *parent = 0);
    ~SuccesfulHiding();
    
private:
    Ui::SuccesfulHiding *ui;
};

#endif // SUCCESFULHIDING_H

