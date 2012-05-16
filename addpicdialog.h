#ifndef ADDPICDIALOG_H
#define ADDPICDIALOG_H

#include <QDialog>
#include "intermediary.h"

namespace Ui {
class AddPicDialog;
}

class AddPicDialog : public QDialog
{
    Q_OBJECT
    
public:
    //AddPicDialog(Intermediary*,QWidget *parent = 0);
    //explicit AddPicDialog(QWidget *parent = 0, Intermediary *im);
    explicit AddPicDialog(Intermediary *im, QWidget *parent = 0);
    ~AddPicDialog();
    
private:
    Ui::AddPicDialog *ui;

private slots:
    void add();
    void del();
    void ok();
    void showOK();
    void showDEL();
};

#endif // ADDPICDIALOG_H
