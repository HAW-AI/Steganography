#ifndef ADDPICDIALOG_H
#define ADDPICDIALOG_H

#include <QDialog>

namespace Ui {
class AddPicDialog;
}

class AddPicDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddPicDialog(QWidget *parent = 0);
    ~AddPicDialog();
    
private:
    Ui::AddPicDialog *ui;

private slots:
    void add();
    void del();
    void ok();
    void showOK();
};

#endif // ADDPICDIALOG_H
