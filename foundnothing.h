#ifndef FOUNDNOTHING_H
#define FOUNDNOTHING_H

#include <QDialog>

namespace Ui {
class FoundNothing;
}

class FoundNothing : public QDialog
{
    Q_OBJECT
    
public:
    explicit FoundNothing(QWidget *parent = 0);
    ~FoundNothing();
    
private:
    Ui::FoundNothing *ui;
};

#endif // FOUNDNOTHING_H
