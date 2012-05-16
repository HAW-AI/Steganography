#include "addpicdialog.h"
#include "ui_addpicdialog.h"
#include "difftests.h"
#include "intermediary.h"

AddPicDialog::AddPicDialog(Intermediary *im,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPicDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add pictures");

    //ui->picTextArea->setText(); //erster bildpath in der map

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
    QString path="hallo";
    ui->picTextArea->append(path);
}

void AddPicDialog::del()
{

}

void AddPicDialog::ok()
{
    //im zurückgeben
}

int AddPicDialog::readDensity()
{
    switch(ui->bpPComboBox->currentIndex())
    {
    case 0: return 1;
    case 1: return 3;
    case 2: return 6;
    default: return 1;
    }

}

void AddPicDialog::showOK()
{

}
