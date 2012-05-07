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
    ui->encryptFrame->hide();
    connect( ui->encryptCheckBox, SIGNAL(toggled(bool)), this, SLOT(showEncryptFrame(bool)) );
    connect( ui->picPathTextField, SIGNAL(textChanged()), this, SLOT(showHideButton()) );
    connect( ui->textPathTextField, SIGNAL(textChanged()), this, SLOT(showHideButton()) );
    connect( ui->textEdit, SIGNAL(textChanged()), this, SLOT(showHideButton()) );
    connect( ui->textFromDocRadio, SIGNAL(toggled(bool)), this, SLOT(showHideButton()) );
    connect( ui->textFromFieldRadio, SIGNAL(toggled(bool)), this, SLOT(showHideButton()) );
    connect( ui->keyTextField, SIGNAL(textChanged()), this, SLOT(checkKey()) );

    //find-Buttons
    connect( ui->picBrowseButton_2, SIGNAL( clicked() ), this, SLOT( chosePicture() ) );
    connect( ui->findButton, SIGNAL( clicked() ), this, SLOT( find() ) );
    connect( ui->keyBrowseButton_2, SIGNAL( clicked() ), this, SLOT( browseOneTimePad() ) );
    ui->findButton->setEnabled(false);
    ui->decryptFrame->hide();
    connect( ui->decryptCheckBox, SIGNAL(toggled(bool)), this, SLOT(showDecryptFrame(bool)) );
    connect( ui->picPathTextField_2, SIGNAL(textChanged()), this, SLOT(showFindButton()) );
    connect( ui->textToDocRadio, SIGNAL(toggled(bool)), this, SLOT(showFindButton()) );
    connect( ui->textToFieldRadio, SIGNAL(toggled(bool)), this, SLOT(showFindButton()) );
    connect( ui->keyTextField_2, SIGNAL(textChanged()), this, SLOT(checkKey_2()) );
}

DiffTests::~DiffTests()
{
    delete ui;
}

void DiffTests::showEncryptFrame(bool show){
    if(show){
        ui->encryptFrame->show();
        checkKey();
    }else{
        ui->encryptFrame->hide();
        showHideButton();
    }
}

void DiffTests::showDecryptFrame(bool show){
    if(show){
        ui->decryptFrame->show();
        checkKey_2();
    }else{
        ui->decryptFrame->hide();
        showFindButton();
    }
}

void DiffTests::showHideButton(){
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

void DiffTests::checkKey()
{
    int keyLength = ui->keyTextField->toPlainText().size();
    switch (ui->techniqueComboBox->currentIndex())
    {
        case 0:
            //Caesar
            if(keyLength == 1){
                ui->hideButton->setEnabled(true);
            }else{
                ui->hideButton->setEnabled(false);
            };
            break;
        case 1:
            //AES
            if(keyLength <= 32 && keyLength >= 16){
                ui->hideButton->setEnabled(true);
            }else{
                ui->hideButton->setEnabled(false);
            }
            break;
        default:
            ui->hideButton->setEnabled(false);
    }
}

void DiffTests::checkKey_2()
{
    int keyLength = ui->keyTextField->toPlainText().size();
    switch (ui->techniqueComboBox_2->currentIndex())
    {
        case 0:
            //Caesar
            if(keyLength == 1){
                ui->findButton->setEnabled(true);
            }else{
                ui->findButton->setEnabled(false);
            };
            break;
        case 1:
            //AES
            if(keyLength <= 32 && keyLength >= 16){
                ui->findButton->setEnabled(true);
            }else{
                ui->findButton->setEnabled(false);
            }
            break;
        default:
            ui->findButton->setEnabled(false);
    }
}

int DiffTests::getFormat(QString text)
{
    //TODO: find out format
    //return UNICODE, if text contains any non-ascii
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

    //encrypt
    if(ui->encryptCheckBox->isChecked()){
        plain = encrypt(plain);
    }

    //ui->textEdit->setText(plain);

    stego.insertText_1BitPerPixel(&plain, UNICODE);
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save File"), picPath, tr("*.png *.jpg"));
    ui->picPathTextField_2->setText(newPath);
    stego.saveImage(newPath);
    qDebug("Fertig!");
}

void DiffTests::find()
{
    QString picPath = ui->picPathTextField_2->toPlainText();
    Steganography stego(picPath);

    QString* plain = stego.getHiddenText();

    //decrypt
    if(ui->decryptCheckBox->isChecked()){
        plain = &(decrypt(*plain));
    }

    if(ui->textToFieldRadio->isChecked()){
        ui->textEdit_2->setText(*plain);
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

QString DiffTests::encrypt(QString plain)
{
    QString key = ui->keyTextField->toPlainText();
    //int format = getFormat(plain);
    /*
    switch (ui->techniqueComboBox->currentIndex())
    {
        case 0: //Caesar
            //caesar(plain, -key, format); //wichtig: key neg
            break;
        case 1: //AES
            //aes();
            //do something else
            break;
        default:
            //do nothing
    }
    */
    return "ciphertext";
}

QString DiffTests::decrypt(QString cipher)
{
    return "plaintext";
}
