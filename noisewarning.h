#ifndef NOISEWARNING_H
#define NOISEWARNING_H

#include <QDialog>

namespace Ui {
class NoiseWarning;
}

class NoiseWarning : public QDialog
{
    Q_OBJECT
    
public:
    explicit NoiseWarning(QWidget *parent = 0);
    ~NoiseWarning();
    
private:
    Ui::NoiseWarning *ui;
};

#endif // NOISEWARNING_H
