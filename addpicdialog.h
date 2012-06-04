#ifndef ADDPICDIALOG_H
#define ADDPICDIALOG_H

#include <QDialog>
#include <QDir>
#include "intermediary.h"

namespace Ui {
class AddPicDialog;
}

class AddPicDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddPicDialog(Intermediary *im, QWidget *parent = 0);
    ~AddPicDialog();
    
public slots:
    void setActDir(QString dir);

private:
    Ui::AddPicDialog *ui;
    Intermediary *im;

private slots:
    void actualizeList();
    void add();
    void del();
    void ok();
    void showOK();
    void showDEL();
};

#endif // ADDPICDIALOG_H
