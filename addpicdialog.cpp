#include "addpicdialog.h"
#include "ui_addpicdialog.h"
#include "intermediary.h"
#include <QtGui>

//********************************************************************
//
// Class: AddPicDialog
// Author: Fenja Harbke
//
// Purpose: called by ProblemDialog
//          chose several pictures to split the information
//
//********************************************************************

QDir actDir2 = QDir::homePath();

AddPicDialog::AddPicDialog(Intermediary *im,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPicDialog),
    im(im)
{
    ui->setupUi(this);
    setWindowTitle("Stegosaur");

    ui->okButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(add()) );
    connect(ui->delButton, SIGNAL(clicked()), this, SLOT(del()) );
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(ok()) );
    connect(ui->cancleButton, SIGNAL(clicked()), this, SLOT(reject()) );
    connect(ui->bpPComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showOK()) );
    connect(ui->picList, SIGNAL(clicked(QModelIndex)), this, SLOT(showDEL()) );
    actualizeList();
}

AddPicDialog::~AddPicDialog()
{
    delete ui;
}

void AddPicDialog::setActDir(QString dir){
    actDir2.setPath(dir);
}

void AddPicDialog::actualizeList()
{
    ui->picList->clear();
    QMap<QString, QImage>::const_iterator it = im->images->constBegin();
    while (it != im->images->constEnd()) {
        ui->picList->addItem(it.key());
        it++;
    }
}

void AddPicDialog::add()
{
    QString newPath = QFileDialog::getOpenFileName(this, tr("Add picture"), actDir2.absolutePath(), tr("*.png"));
    im->addImage(newPath);
    actDir2.setPath(newPath);
    ui->picList->addItem(new QListWidgetItem(newPath));
    actualizeList();
    showOK();
}

void AddPicDialog::del()
{
    QString path = ui->picList->item(ui->picList->currentRow())->text();
    im->images->remove(path);
    ui->delButton->setEnabled(false);
    actualizeList();
    showOK();
}

void AddPicDialog::ok()
{
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save as"), actDir2.absolutePath(), tr("*.png"));
    actDir2.setPath(newPath);
    switch(ui->bpPComboBox->currentIndex())
    {
    case 0: im->hide_1Bit(newPath);
        break;
    case 1: im->hide_3Bit(newPath);
        break;
    case 2: im->hide_6Bit(newPath);
        break;
    default:;
    }
    setResult(1);
    close();
}

void AddPicDialog::showDEL(){ ui->delButton->setEnabled(true);}

void AddPicDialog::showOK()
{
    bool show = false;
    switch(ui->bpPComboBox->currentIndex())
    {
    case 0: if(im->isReady_1Bit()){show=true;}
        break;
    case 1: if(im->isReady_3Bit()) {show=true;}
        break;
    case 2: if(im->isReady_6Bit()) {show=true;}
        break;
    default:;
    }
    ui->okButton->setEnabled(show);
}
