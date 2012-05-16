#include "addpicdialog.h"
#include "ui_addpicdialog.h"
#include "difftests.h"
#include "intermediary.h"

#include <QtGui>

Intermediary* intmed;

AddPicDialog::AddPicDialog(Intermediary *im,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPicDialog)
{
    intmed=im;
    ui->setupUi(this);
    setWindowTitle("Add pictures");

    ui->okButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    //ui->picList->addItem(im->images->keys().first());
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(add()) );
    connect(ui->delButton, SIGNAL(clicked()), this, SLOT(del()) );
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(ok()) );
    connect(ui->cancleButton, SIGNAL(clicked()), this, SLOT(reject()) );
    connect(ui->bpPComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showOK()) );
    connect(ui->picList, SIGNAL(clicked(QModelIndex)), this, SLOT(showDEL()) );
}

AddPicDialog::~AddPicDialog()
{
    delete ui;
}

void AddPicDialog::add()
{
    QString newPath = QFileDialog::getOpenFileName(this, tr("Add picture"), QString::null, tr("*.png"));
    intmed->addImage(newPath);
    //im->addImage(newPath);
    ui->picList->addItem(new QListWidgetItem(newPath));
    showOK();
}

void AddPicDialog::del()
{
    QString path = ui->picList->item(ui->picList->currentRow())->text();
    delete ui->picList->item(ui->picList->currentRow());
    //intmed->removeImage(path);
    intmed->images->take(ui->picList->item(ui->picList->currentRow())->text());
    ui->delButton->setEnabled(false);
    showOK();
}

void AddPicDialog::ok()
{
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save as"), QString::null, tr("*.png"));
    intmed->hide_1Bit(newPath);
    setResult(1);
    close();
}

void AddPicDialog::showDEL(){ ui->delButton->setEnabled(true);}

void AddPicDialog::showOK()
{
    bool show = false;
    switch(ui->bpPComboBox->currentIndex())
    {
    case 0: if(intmed->isReady_1Bit()){show=true;}
        break;
    case 1: if(intmed->isReady_3Bit()) {show=true;}
        break;
    case 2: if(intmed->isReady_6Bit()) {show=true;}
        break;
    default:;
    }
    ui->okButton->setEnabled(show);
}
