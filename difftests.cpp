#include "difftests.h"
#include "ui_difftests.h"
#include "QTime"
#include "bitChanger.h"
#include "steganography.h"
#include "stringiterator.h"
#include "iostream"
#include "QImage"

#include <QtGui>
#define UNICODE 1
#define ASCII 0

DiffTests::DiffTests(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiffTests)
{
    ui->setupUi(this);
    
    //hide-Buttons
    connect( ui->picBrowseButton, SIGNAL( clicked() ), this, SLOT( chosePicture() ) );
    connect( ui->textBrowseButton, SIGNAL( clicked() ), this, SLOT( choseText() ) );
    connect( ui->hideButton, SIGNAL( clicked() ), this, SLOT( hide() ) );
    connect( ui->keyBrowseButton, SIGNAL( clicked() ), this, SLOT( browseOneTimePad() ) );
    ui->hideButton->setEnabled(false);
    ui->cryptFrame->hide();
    connect( ui->encryptCheckBox, SIGNAL(toggled(bool)), this, SLOT(showCryptFrame(bool)) );
    connect( ui->picPathTextField, SIGNAL(textChanged()), this, SLOT(showButtonHide()) );
    connect( ui->textPathTextField, SIGNAL(textChanged()), this, SLOT(showButtonHide()) );
    connect( ui->textEdit, SIGNAL(textChanged()), this, SLOT(showButtonHide()) );
    connect( ui->textFromDocRadio, SIGNAL(toggled(bool)), this, SLOT(showButtonHide()) );
    connect( ui->textFromFieldRadio, SIGNAL(toggled(bool)), this, SLOT(showButtonHide()) );

    //find-Buttons
    connect( ui->picBrowseButton_2, SIGNAL( clicked() ), this, SLOT( chosePicture() ) );
    connect( ui->findButton, SIGNAL( clicked() ), this, SLOT( find() ) );
    connect( ui->keyBrowseButton_2, SIGNAL( clicked() ), this, SLOT( browseOneTimePad() ) );
    ui->findButton->setEnabled(false); //TODO: run clicked RadioButtons
    ui->cryptFrame_2->hide();
    ui->knowledgeKeyFrame->hide();
    ui->attackFrame->hide();
    connect( ui->encryptCheckBox_2, SIGNAL(toggled(bool)), this, SLOT(showCryptCheckFrame_2(bool)) );
    connect( ui->knownKeyRadio, SIGNAL(toggled(bool)), this, SLOT(showCryptFrame_2) );
    connect( ui->unknownKeyRadio, SIGNAL(toggled(bool)), this, SLOT(showAttackFrame) );
    connect( ui->picPathTextField_2, SIGNAL(textChanged()), this, SLOT(showFindButton()) );
    connect( ui->textToDocRadio, SIGNAL(toggled(bool)), this, SLOT(showFindButton()) );
    connect( ui->textToFieldRadio, SIGNAL(toggled(bool)), this, SLOT(showFindButton()) );
}

DiffTests::~DiffTests()
{
    delete ui;
}

void DiffTests::showCryptCheckFrame_2(bool show){
    if(show){
        ui->knowledgeKeyFrame->show();
    }else{
        ui->knowledgeKeyFrame->hide();
    }
}

void DiffTests::showCryptFrame_2(){
    if(ui->knownKeyRadio->isChecked()){
        ui->cryptFrame_2->show();
    }else{
        ui->cryptFrame_2->hide();
    }
}

void DiffTests::showAttackFrame(){
    if(ui->unknownKeyRadio->isChecked()){
        ui->attackFrame->show();
    }else{
        ui->attackFrame->hide();
    }
}

void DiffTests::showCryptFrame(bool show){
    if(show){
        ui->cryptFrame->show();
    }else{
        ui->cryptFrame->hide();
    }
}

void DiffTests::showButtonHide(){
    if(ui->textFromDocRadio->isChecked() && isPath(ui->textPathTextField->toPlainText()) && isPath(ui->picPathTextField->toPlainText())){
            ui->hideButton->setEnabled(true);
    }else if(ui->textFromFieldRadio->isChecked() && !(ui->textEdit->toPlainText().isEmpty())&& isPath(ui->picPathTextField->toPlainText())){
            ui->hideButton->setEnabled(true);
    }else{
            ui->hideButton->setEnabled(false);
        }
}

void DiffTests::showFindButton(){
    if((ui->textToDocRadio->isChecked() || ui->textToFieldRadio->isChecked()) && isPath(ui->picPathTextField_2->toPlainText())){
        ui->findButton->setEnabled(true);
    }else{
        ui->findButton->setEnabled(false);
    }
}

bool DiffTests::isPath(QString path){
    QFileInfo file= path;
    return file.exists();
}

void DiffTests::chosePicture()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                QDir::current().path(),
                "PNG Files(*.png)");  //JPEG files (*.jpg *.png);; Gif files (*.gif)
    ui->picPathTextField->setText( path );
    ui->picPathTextField_2->setText( path );
}

void DiffTests::choseText()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                QString::null,
                "Text Files(*.txt)");
    ui->textPathTextField->setText( path );
}

void DiffTests::hide()
{
    QString picPath = ui->picPathTextField->toPlainText();
    Steganography stego(picPath);

    QString plain;
    if(ui->textFromDocRadio->isChecked()){
        QString plainPath = ui->textPathTextField->toPlainText();
        QFile file(plainPath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        plain = in.readAll();
        file.close();
    }else if(ui->textFromFieldRadio->isChecked()) plain = ui->textEdit->toPlainText(); //if(textFromFieldRadio)

    stego.insertText(&plain, UNICODE);

    QString newPath = QFileDialog::getSaveFileName(this, tr("Save File"), picPath, tr("*.png *.jpg"));

    stego.saveImage(newPath);
    qDebug("Fertig!");
}

void DiffTests::find()
{
    QString picPath = ui->picPathTextField_2->toPlainText();
    Steganography stego(picPath);

    QString plain = stego.getText();
    if(ui->textToFieldRadio->isChecked()){
        ui->textEdit_2->setText(plain);
    }else if(ui->textToDocRadio->isChecked()){
        QString newPath = QFileDialog::getSaveFileName(
                    this,
                    "Save Textfile",
                    QString::null,
                    "Text Files(*.txt)");
        QFile file(newPath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << plain;
        file.close();
    }

    QString* plain = stego.getHiddenText();
    ui->textEdit_2->setText(*plain);

}

void DiffTests::browseOneTimePad()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                QString::null,
                "Text Files(*.txt)");
    ui->keyTextField->setText( path );
    ui->keyTextField_2->setText( path );
}

void DiffTests::decrypt()
{

}

void DiffTests::encrypt()
{

}
