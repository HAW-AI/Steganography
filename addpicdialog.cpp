#include "addpicdialog.h"
#include "ui_addpicdialog.h"

AddPicDialog::AddPicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPicDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add pictures");
}

AddPicDialog::~AddPicDialog()
{
    delete ui;
}
