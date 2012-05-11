#ifndef DIFFTESTS_H
#define DIFFTESTS_H

#include <QMainWindow>
#include "problemdialog.h"
#include "intermediary.h"

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
    void chosePicture();
    void choseText();
    void hide();
    void find();
    int getFormat(QString text);
    void browseOneTimePad();

    void showEncryptFrame(bool);
    void showDecryptFrame(bool);
    void showHideButton();
    void showFindButton();
    void checkKey();
    void checkKey_2();

    bool isPath(QString);

public slots:
    QString decrypt(QString);
    QString encrypt(QString);

    void popupProblemDialog();

private:
    Ui::DiffTests *ui;
    ProblemDialog *pd;
    Intermediary *im;
};

#endif // DIFFTESTS_H
