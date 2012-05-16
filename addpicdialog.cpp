#include "addpicdialog.h"
#include "ui_addpicdialog.h"
#include "difftests.h"
#include "intermediary.h"

#include <QtGui>

Intermediary* im;

AddPicDialog::AddPicDialog(Intermediary *im,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPicDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add pictures");

    ui->okButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    //ui->picList->addItem(im->images->keys().first());
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(add()) );
    connect(ui->delButton, SIGNAL(clicked()), this, SLOT(del()) );
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(ok()) );
    connect(ui->cancleButton, SIGNAL(clicked()), this, SLOT(close()) );
    connect(ui->bpPComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showOK(int)) );
    connect(ui->picList, SIGNAL(clicked(QModelIndex)), this, SLOT(showDEL()) );
}

AddPicDialog::~AddPicDialog()
{
    delete ui;
}

void AddPicDialog::add()
{
    QString newPath = QFileDialog::getOpenFileName(this, tr("Add picture"), QString::null, tr("*.png *.jpg"));
    //im->addImage(newPath);
    ui->picList->addItem(new QListWidgetItem(newPath));
}

void AddPicDialog::del()
{
    delete ui->picList->item(ui->picList->currentRow());
    //ui->picList->takeItem(ui->picList->currentRow());
    ui->delButton->setEnabled(false);
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

void AddPicDialog::showDEL(){ ui->delButton->setEnabled(true);}

void AddPicDialog::showOK()
{
    ui->okButton->show();
}
