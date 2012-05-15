#include "addpicdialog.h"
#include "ui_addpicdialog.h"

AddPicDialog::AddPicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPicDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add pictures");

    ui->okButton->setEnabled(false);
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(add()) );
    connect(ui->delButton, SIGNAL(clicked()), this, SLOT(del()) );
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(ok()) );
    connect(ui->cancleButton, SIGNAL(clicked()), this, SLOT(close()) );
    connect(ui->bpPComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showOK(int)) );
}

AddPicDialog::~AddPicDialog()
{
    delete ui;
}

void AddPicDialog::add()
{

}

void AddPicDialog::del()
{

}

void AddPicDialog::ok()
{

}

void AddPicDialog::showOK()
{
    //ui->label->setText();
}
