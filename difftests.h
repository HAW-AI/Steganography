#ifndef DIFFTESTS_H
#define DIFFTESTS_H

#include <QMainWindow>
#include "problemdialog.h"
#include "intermediary.h"
#include "noisewarning.h"
#include "addpicdialog.h"
#include "notenoughinfo.h"
#include "succesfulhiding.h"

namespace Ui {
class DiffTests;
}

class DiffTests : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DiffTests(QWidget *parent = 0);
    ~DiffTests();

private:
    Ui::DiffTests *ui;
    ProblemDialog *pd;
    NoiseWarning *nw;
    Intermediary *im;
    AddPicDialog *apd;
    NotEnoughInfo *ne;
    SuccesfulHiding *sh;

private slots:
    int getFormat(QString text);
    bool isPath(QString);

    void chosePicture();
    void choseText();
    void clickRadio();
    void showEncryptFrame(bool);
    void showHideButton();
    void hide();

    void chosePicture_2();
    void showRemove();
    void removePicture_2();
    void showDecryptFrame(bool);
    void showFindButton();
    void find();


    QString *decrypt(QString *);
    QString *encrypt(QString *);

    int popupProblemDialog();
    int noiseWarningDialog();
    void notEnough();
    void showSuccessfulHiding(QString);
};

#endif // DIFFTESTS_H
