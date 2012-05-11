#ifndef PROBLEMDIALOG_H
#define PROBLEMDIALOG_H

#include <QDialog>

namespace Ui {
class ProblemDialog;
}

class ProblemDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProblemDialog(QWidget *parent = 0);
    ~ProblemDialog();
    
private:
    Ui::ProblemDialog *ui;

private slots:
    void choseNewPic();
    void moreDensity();
    void choseMorePics();

};

#endif // PROBLEMDIALOG_H
