#include "difftests.h"
#include "ui_difftests.h"
#include "QTime"
#include "bitChanger.h"
#include "steganography.h"
#include "stringiterator.h"
#include "iostream"
#include "QImage"
#include "crypt.h"
#include "problemdialog.h"
#include "intermediary.h"
#include "noisewarning.h"
#include "addpicdialog.h"

#include <QtGui>
#define UNICODE 1
#define ASCII 0
#define ENCRYPT 1
#define DECRYPT 0

#define CANCLE 0
#define DENSITY 1
#define PICS 2

int format;
QString ascii = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";


DiffTests::DiffTests(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiffTests)
{
    ui->setupUi(this);
    setWindowTitle("Stego-saur");

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
    connect( ui->picBrowseButton_2, SIGNAL( clicked() ), this, SLOT( chosePicture_2() ) );
    connect( ui->findButton, SIGNAL( clicked() ), this, SLOT( find() ) );
    connect( ui->keyBrowseButton_2, SIGNAL( clicked() ), this, SLOT( browseOneTimePad() ) );
    ui->findButton->setEnabled(false);
    ui->decryptFrame->hide();
    ui->textEdit_2->setEnabled(false);
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

int DiffTests::getFormat(QString text)
{
    for(int i = 0; i<text.size();i++)
    {
        if(!(ascii.contains(text.at(i)))){
            return UNICODE;
        }
    }
    return ASCII;

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
            QFile file(ui->textPathTextField->toPlainText());
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&file);
            QString plain = in.readAll();
          format = getFormat(plain);
            file.close();
            if(format == UNICODE) ui->asciiUnicodeLabel->setText("Unicode format");
            else ui->asciiUnicodeLabel->setText("Ascii format");
    }else if(ui->textFromFieldRadio->isChecked() && !(ui->textEdit->toPlainText().isEmpty())&& isPath(ui->picPathTextField->toPlainText())){
            ui->hideButton->setEnabled(true);
            format = getFormat(ui->textEdit->toPlainText());
            if(format == UNICODE) ui->asciiUnicodeLabel->setText("Unicode format");
            else ui->asciiUnicodeLabel->setText("Ascii format");
    }else{
            ui->hideButton->setEnabled(false);
            ui->asciiUnicodeLabel->setText("");
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
                QDir::homePath(),
                "PNG Files(*.png)");  //JPEG files (*.jpg *.png);; Gif files (*.gif)
    ui->picPathTextField->setText( path );
}

void DiffTests::chosePicture_2()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                QDir::homePath(),
                "PNG Files(*.png)");  //JPEG files (*.jpg *.png);; Gif files (*.gif)
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
    int oldFormat = format;
    if(getFormat(ui->keyTextField->toPlainText()) == ASCII)
    {
        switch (ui->techniqueComboBox->currentIndex())
        {
            case 0:
                //Caesar
                if(keyLength == 1){
                    ui->hideButton->setEnabled(true);
                    ui->keyTipLabel->setText("ok");
                }else{
                    ui->hideButton->setEnabled(false);
                    ui->keyTipLabel->setText("only one letter key");
                };
                break;
            case 1:
                //Vigenère
                if(keyLength > 0){
                    ui->keyTipLabel->setText("ok");
                    ui->hideButton->setEnabled(true);
                }else{
                    ui->keyTipLabel->setText("one letter minimum");
                    ui->hideButton->setEnabled(false);
                }
                break;
            case 2:
                //AES
                if(keyLength <= 32 && keyLength >= 16){
                    ui->hideButton->setEnabled(true);
                }else{
                    ui->hideButton->setEnabled(false);
                }
                break;
            default:
                ui->hideButton->setEnabled(false);
                ui->keyTipLabel->setText("");
        }
    }else{
        ui->keyTipLabel->setText("key is not Ascii format");
        ui->hideButton->setEnabled(false);
    }
    format = oldFormat;
    showHideButton();
}

void DiffTests::checkKey_2()
{
    int keyLength = ui->keyTextField_2->toPlainText().size();
    int oldFormat = format;
    if(getFormat(ui->keyTextField_2->toPlainText()) == ASCII)
    {
        switch (ui->techniqueComboBox_2->currentIndex())
        {
            case 0:
                //Caesar
                if(keyLength == 1){
                    ui->findButton->setEnabled(true);
                    ui->keyTipLabel_2->setText("ok");
                }else{
                    ui->findButton->setEnabled(false);
                    ui->keyTipLabel_2->setText("only one letter key");
                };
                break;
            case 1:
                //Vigenère
                if(keyLength > 0){
                    ui->keyTipLabel_2->setText("ok");
                    ui->findButton->setEnabled(true);
                }else{
                    ui->keyTipLabel_2->setText("one letter minimum");
                    ui->findButton->setEnabled(false);
                }
                break;
            case 2:
                //AES
                if(keyLength <= 32 && keyLength >= 16){
                    ui->findButton->setEnabled(true);
                }else{
                    ui->findButton->setEnabled(false);
                }
                break;
            default:
                ui->findButton->setEnabled(false);
                ui->keyTipLabel->setText("");
        }
    }else{
        ui->keyTipLabel_2->setText("key is not Ascii format");
        ui->findButton->setEnabled(false);
    }
    format = oldFormat;
    showFindButton();
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
    Crypt c (plain,ui->keyTextField->toPlainText(),format);
    QString cipher;
    switch (ui->techniqueComboBox->currentIndex())
    {
        case 0: //Caesar
            cipher = c.caesar(ENCRYPT);
            break;
        case 1: //Vigenère
            cipher = c.vigenere(ENCRYPT);
            break;
        default:
            ui->keyTipLabel->setText("Encryption failed");
            return plain;
    }
    return cipher;
}

QString DiffTests::decrypt(QString cipher)
{
    Crypt c (cipher,ui->keyTextField_2->toPlainText(),format);
    QString plain;
    switch (ui->techniqueComboBox_2->currentIndex())
    {
        case 0: //Caesar
            plain = c.caesar(DECRYPT);
            break;
        case 1: //Vigenère
            plain = c.vigenere(DECRYPT);
            break;
        default:
            ui->keyTipLabel_2->setText("Decryption failed");
            return cipher;
    }
    return plain;
}

int DiffTests::popupProblemDialog()
{
    pd = new ProblemDialog();
    pd->exec();
    return pd->result();
}

int DiffTests::noiseWarningDialog()
{
    nw = new NoiseWarning();
    nw->exec();
    return nw->result();
}

int DiffTests::addPicDialog()
{
    apd = new AddPicDialog();
    apd->exec();
    return apd->result();
}

void DiffTests::hide()
{
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

    im = new Intermediary(&plain, format, ui->picPathTextField->toPlainText());
    QString newPath;

    bool ready = false;
    while(!ready){

    if(im->isReady_1Bit()){
        newPath = QFileDialog::getSaveFileName(this, tr("Save File"), ui->picPathTextField->toPlainText(), tr("*.png *.jpg"));
        im->hide_1Bit(newPath);
        ready = true;
    }else{
        int action = popupProblemDialog();
        if( action == CANCLE){ready = true;}
        else if (action == DENSITY)
        {
            //warning
            int w = noiseWarningDialog();
            if(im->isReady_3Bit() && w == 1){
                newPath = QFileDialog::getSaveFileName(this, tr("Save File"), ui->picPathTextField->toPlainText(), tr("*.png *.jpg"));
                im->hide_3Bit(newPath);
                ready = true;
            }else{
                action = popupProblemDialog();
                if( action == CANCLE){ready = true;}
                else if (action == DENSITY)
                {
                    //warning
                    w = noiseWarningDialog();
                    if(im->isReady_6Bit() && w == 1){
                        newPath = QFileDialog::getSaveFileName(this, tr("Save File"), ui->picPathTextField->toPlainText(), tr("*.png *.jpg"));
                        im->hide_6Bit(newPath);
                        ready = true;
                    }else{
                        qDebug("still not enough!");
                    }
                }else if (action == PICS)
                {
                    int i = addPicDialog();
                    ready = true;
                }else{}

            }
        }else if (action == PICS)
        {
            int i = addPicDialog();
            ready = true;
        }else{}
    }
    }
    ui->picPathTextField_2->setText(newPath);

}

void DiffTests::find()
{
    im = new Intermediary(ui->picPathTextField_2->toPlainText());
    QString* plain = im->getHiddenText();

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
/*
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
*/
