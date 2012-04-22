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
    void decrypt();
    void encrypt();

    void showCryptFrame(bool);
    void showCryptFrame_2(bool);
    void showCryptCheckFrame_2(bool);
    void showAttackFrame(bool);
    void showButtonHide();
    void showFindButton();

    bool isPath(QString);

private:
    Ui::DiffTests *ui;
};

#endif // DIFFTESTS_H
