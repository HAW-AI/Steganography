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
    void chosePicture();
    void choseText();
    void hide();
    void find();
    void browseOneTimePad();

    void showEncryptFrame(bool);
    void showDecryptFrame(bool);
    void showButtonHide();
    void showFindButton();

    bool isPath(QString);

public slots:
    QString decrypt(QString);
    QString encrypt(QString);

private:
    Ui::DiffTests *ui;
};

#endif // DIFFTESTS_H
