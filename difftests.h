#ifndef DIFFTESTS_H
#define DIFFTESTS_H

#include <QMainWindow>

namespace Ui {
class DiffTests;
}

class DiffTests : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DiffTests(QWidget *parent = 0);
    ~DiffTests();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::DiffTests *ui;
};

#endif // DIFFTESTS_H
