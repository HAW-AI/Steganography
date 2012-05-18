#ifndef DIFFTESTS_H
#define DIFFTESTS_H

#include <QMainWindow>
#include "problemdialog.h"
#include "intermediary.h"
#include "noisewarning.h"
#include "addpicdialog.h"
#include "notenoughinfo.h"

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
    void chosePicture_2();
    void removePicture_2();
    void choseText();
    void hide();
    void find();
    int getFormat(QString text);
    void browseOneTimePad();
    void showRemove();

    void showEncryptFrame(bool);
    void showDecryptFrame(bool);
    void showHideButton();
    void showFindButton();

    bool isPath(QString);

public slots:
    QString *decrypt(QString *);
    QString *encrypt(QString *);

    int popupProblemDialog();
    int noiseWarningDialog();
    void notEnough();

private:
    Ui::DiffTests *ui;
    ProblemDialog *pd;
    NoiseWarning *nw;
    Intermediary *im;
    AddPicDialog *apd;
    NotEnoughInfo *ne;
};

#endif // DIFFTESTS_H
