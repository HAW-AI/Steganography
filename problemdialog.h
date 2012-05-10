#ifndef PROBLEMDIALOG_H
#define PROBLEMDIALOG_H

#include <QWidget>

namespace Ui {
class ProblemDialog;
}

class ProblemDialog : public QWidget
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
