#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "problemdialog.h"
#include "intermediary.h"
#include "noisewarning.h"
#include "addpicdialog.h"
#include "notenoughinfo.h"
#include "succesfulhiding.h"
#include "picturetoosmall.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

//Constructor
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//Attribute
private:
    Ui::MainWindow *ui;
    ProblemDialog *pd;
    NoiseWarning *nw;
    Intermediary *im;
    AddPicDialog *apd;
    NotEnoughInfo *ne;
    SuccesfulHiding *sh;
    PictureTooSmall *ts;

//Methods
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
    void picToSmall();
    void foundNothing();
};

#endif // MAINWINDOW_H
