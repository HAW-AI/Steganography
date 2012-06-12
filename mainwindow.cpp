#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTime"
#include "bitChanger.h"
#include "steganography.h"
#include "stringiterator.h"
#include "iostream"
#include "QImage"
#include "crypt.h"
#include "problemdialog.h"
#include "intermediary.h"

#include <QtGui>
#define UNICODE 1
#define ASCII 0
#define ENCRYPT 1
#define DECRYPT 0

#define CANCEL 0
#define DENSITY 1
#define PICS 2
#define NEWPIC 3

int format;
QString ascii = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\n";
QDir actDir = QDir::homePath();
int headerSize = 194;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Stegosaur");

    //hide
    connect( ui->picBrowseButton, SIGNAL( clicked() ), this, SLOT( chosePicture() ) );
    connect( ui->textBrowseButton, SIGNAL( clicked() ), this, SLOT( choseText() ) );
    connect( ui->hideButton, SIGNAL( clicked() ), this, SLOT( hide() ) );
    ui->hideButton->setEnabled(false);
    ui->encryptFrame->hide();
    connect( ui->encryptCheckBox, SIGNAL(toggled(bool)), this, SLOT(showEncryptFrame(bool)) );
    connect( ui->picPathTextField, SIGNAL(textChanged(QString)), this, SLOT(showHideButton()) );
    connect( ui->textPathTextField, SIGNAL(textChanged(QString)), this, SLOT(showHideButton()) );
    connect( ui->textEdit, SIGNAL(textChanged()), this, SLOT(clickRadio()) );
    connect( ui->textFromDocRadio, SIGNAL(toggled(bool)), this, SLOT(showHideButton()) );
    connect( ui->textFromFieldRadio, SIGNAL(toggled(bool)), this, SLOT(showHideButton()) );
    connect( ui->keyTextField, SIGNAL(textChanged(QString)), this, SLOT(showHideButton()) );
    connect( ui->techniqueComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showHideButton()));

    //find
    connect( ui->picBrowseButton_2, SIGNAL( clicked() ), this, SLOT( chosePicture_2() ) );
    connect( ui->picRemoveButton, SIGNAL(clicked()), this, SLOT( removePicture_2() ) );
    connect( ui->picPathTextField_2, SIGNAL(clicked(QModelIndex)), this, SLOT(showRemove()));
    connect( ui->findButton, SIGNAL( clicked() ), this, SLOT( find() ) );
    ui->picRemoveButton->setEnabled(false);
    ui->findButton->setEnabled(false);
    ui->decryptFrame->hide();
    ui->textEdit_2->setReadOnly(true);
    connect( ui->decryptCheckBox, SIGNAL(toggled(bool)), this, SLOT(showDecryptFrame(bool)) );
    connect( ui->textToDocRadio, SIGNAL(toggled(bool)), this, SLOT(showFindButton()) );
    connect( ui->textToFieldRadio, SIGNAL(toggled(bool)), this, SLOT(showFindButton()) );
    connect( ui->keyTextField_2, SIGNAL(textChanged(QString)), this, SLOT(showFindButton()) );
    connect( ui->techniqueComboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(showFindButton()));
    ui->picPathTextField->text();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// private slots ------------------------------------------------------------------------------------
int MainWindow::getFormat(QString text)
{
    for(int i = 0; i<text.size();i++)
    {
        if(!(ascii.contains(text.at(i))))
        {
            return UNICODE;
        }
    }
    return ASCII;
}

bool MainWindow::isPath(QString path)
{
    QFileInfo file= path;
    return file.exists();
}


//HideTab.........................................................................
void MainWindow::chosePicture()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                actDir.absolutePath(),
                "PNG Files(*.png)");
    actDir.setPath(path);
    QImage* im = new QImage(path);
    if(im->width() > headerSize)
    {
        ui->picPathTextField->setText( path );
    }else if(!im->isNull()){
        picToSmall();
    }
}

void MainWindow::choseText()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                actDir.absolutePath(),
                "Text or PNG Files(*.txt *.png)");
    actDir.setPath(path);
    ui->textPathTextField->setText( path );
    if(!path.isEmpty()) ui->textFromDocRadio->click();
    showHideButton();
}

void MainWindow::clickRadio()
{
    if(!ui->textEdit->toPlainText().isEmpty())ui->textFromFieldRadio->click();
    showHideButton();
}

void MainWindow::showEncryptFrame(bool show)
{
    if(show)ui->encryptFrame->show();
    else ui->encryptFrame->hide();
    showHideButton();
}

void MainWindow::showHideButton()
{
    //TODO ascii/unicode label auch setzten, wenn wirtbild noch nicht gewaehlt
    if(ui->textFromDocRadio->isChecked() && isPath(ui->textPathTextField->text()) && isPath(ui->picPathTextField->text()))
    {
            ui->hideButton->setEnabled(true);
            if(ui->textPathTextField->text().endsWith(".png"))
            {
                ui->encryptCheckBox->setChecked(false);
                ui->encryptCheckBox->setEnabled(false);
                ui->asciiUnicodeLabel->setText("picture");
            }else{
                ui->encryptCheckBox->setEnabled(true);
                QFile file(ui->textPathTextField->text());
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QTextStream in(&file);
                QString plain = in.readAll();
              format = getFormat(plain);
                file.close();
                if(format == UNICODE) ui->asciiUnicodeLabel->setText("Unicode format");
                else ui->asciiUnicodeLabel->setText("Ascii format");
            }
    }else if(ui->textFromFieldRadio->isChecked() && !(ui->textEdit->toPlainText().isEmpty()))
    {
        ui->encryptCheckBox->setEnabled(true);
        format = getFormat(ui->textEdit->toPlainText());
        if(format == UNICODE) ui->asciiUnicodeLabel->setText("Unicode format");
        else ui->asciiUnicodeLabel->setText("Ascii format");
        if(isPath(ui->picPathTextField->text()))
        {
            ui->hideButton->setEnabled(true);
        }
    }else{
            ui->hideButton->setEnabled(false);
            ui->asciiUnicodeLabel->setText("");
    }

    bool show = ui->hideButton->isEnabled();

    if(ui->encryptCheckBox->isChecked())
    {
        int keyLength = ui->keyTextField->text().size();
        int oldFormat = format;
        if(getFormat(ui->keyTextField->text()) == ASCII)
        {
            switch (ui->techniqueComboBox->currentIndex())
            {
                case 0:
                    //Caesar
                    if(keyLength == 1)
                    {
                        ui->hideButton->setEnabled(true);
                        ui->keyTipLabel->setText("ok");
                    }else{
                        ui->hideButton->setEnabled(false);
                        ui->keyTipLabel->setText("only one letter key");
                    }
                    break;
                case 1:
                    //Vigen�re
                    if(keyLength > 0)
                    {
                        ui->keyTipLabel->setText("ok");
                        ui->hideButton->setEnabled(true);
                    }else{
                        ui->keyTipLabel->setText("one letter minimum");
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
        if(show != ui->hideButton->isEnabled()) ui->hideButton->setEnabled(false);
    }
}

void MainWindow::hide()
{
    ui->saveLabel->clear();
    //TODO plain to QString*
    //-> problem bei im->hide_1Bit
    QString plain;
    if(ui->textFromDocRadio->isChecked())
    {
        QString plainPath = ui->textPathTextField->text();
        QFile file(plainPath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        plain = in.readAll();
        file.close();
    }else if(ui->textFromFieldRadio->isChecked()) plain = ui->textEdit->toPlainText(); //if(textFromFieldRadio)

    //encrypt
    if(ui->encryptCheckBox->isChecked())
    {
        ui->saveLabel->setText("Encrypting...");
        plain = *(encrypt(&plain));
        ui->saveLabel->clear();
    }

    QString oldPath = ui->picPathTextField->text();
    im = new Intermediary(oldPath);
    if(ui->textFromDocRadio->isChecked() && ui->textPathTextField->text().endsWith(".png"))
    {
        im->setImage(ui->textPathTextField->text());
    }else{
        im->setText(&plain,format);
    }
    QString savePath;

    ui->saveLabel->setText("Hiding...");
    if(im->isReady_1Bit())
    {
        savePath = QFileDialog::getSaveFileName(this, tr("Save File"), actDir.absolutePath(), tr("*.png *.jpg"));
        im->hide_1Bit(savePath);
    }else{
        int action = popupProblemDialog();
        qDebug()<<action;
        while(action != CANCEL)
        {

            if( action == DENSITY){
                int w = noiseWarningDialog();
                if(im->isReady_3Bit() && w == 1)
                {
                    savePath = QFileDialog::getSaveFileName(this, tr("Save File"), actDir.absolutePath(), tr("*.png *.jpg"));
                    im->hide_3Bit(savePath);
                    action = CANCEL;
                }else{
                    action = popupProblemDialog();
                    if( action == DENSITY)
                    {
                        w = noiseWarningDialog();
                        if(im->isReady_3Bit() && w == 1)
                        {
                            savePath = QFileDialog::getSaveFileName(this, tr("Save File"), actDir.absolutePath(), tr("*.png *.jpg"));
                            im->hide_6Bit(savePath);
                            action = CANCEL;
                        }else{
                            notEnough();
                            action = CANCEL;
                        }
                    }
                }
            }
            else if(action == PICS)
            {
                apd = new AddPicDialog(im);
                apd->setActDir(actDir.absolutePath());
                apd->exec();
                action = CANCEL;
            }else if(action == NEWPIC)
            {
                im->images->remove(savePath);
                chosePicture();
                im->addImage(ui->picPathTextField->text());
                if(im->isReady_1Bit()){
                    savePath = QFileDialog::getSaveFileName(this, tr("Save File"), actDir.absolutePath(), tr("*.png *.jpg"));
                    im->hide_1Bit(savePath);
                    action = CANCEL;
                }else{
                    action = popupProblemDialog();
                }
            }else{action=CANCEL;}
        }
    }
    ui->saveLabel->clear();
    ui->picPathTextField_2->clear();
    if(!savePath.isEmpty())
    {
        actDir.setPath(savePath);
        ui->picPathTextField_2->addItem(savePath);
        showSuccessfulHiding(savePath);
    }
}


//FindTab.............................................................
void MainWindow::chosePicture_2()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                actDir.absolutePath(),
                "PNG Files(*.png)");
    actDir.setPath(path);
    ui->picPathTextField_2->addItem(path);
    showFindButton();
}

void MainWindow::showRemove()
{
    ui->picRemoveButton->setEnabled(true);
    showFindButton();
}

void MainWindow::removePicture_2()
{
    delete ui->picPathTextField_2->item(ui->picPathTextField_2->currentRow());
    ui->picRemoveButton->setEnabled(false);
    showFindButton();
}

void MainWindow::showDecryptFrame(bool show)
{
    if(show)ui->decryptFrame->show();
    else ui->decryptFrame->hide();
    showFindButton();
}

void MainWindow::showFindButton()
{
    if((ui->textToDocRadio->isChecked() || ui->textToFieldRadio->isChecked())&& (ui->picPathTextField_2->count() != 0))
    {
        ui->findButton->setEnabled(true);
    }else{
        ui->findButton->setEnabled(false);
    }
    bool show = ui->findButton->isEnabled();
    if(ui->decryptCheckBox->isChecked())
    {
        int keyLength = ui->keyTextField_2->text().size();
        int oldFormat = format;
        if(getFormat(ui->keyTextField_2->text()) == ASCII)
        {
            switch (ui->techniqueComboBox_2->currentIndex())
            {
                case 0:
                    //Caesar
                    if(keyLength == 1)
                    {
                        ui->findButton->setEnabled(true);
                        ui->keyTipLabel_2->setText("ok");
                    }else{
                        ui->findButton->setEnabled(false);
                        ui->keyTipLabel_2->setText("only one letter key");
                    }
                    break;
                case 1:
                    //Vigen�re
                    if(keyLength > 0)
                    {
                        ui->keyTipLabel_2->setText("ok");
                        ui->findButton->setEnabled(true);
                    }else{
                        ui->keyTipLabel_2->setText("one letter minimum");
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
    }
    if(show != ui->findButton->isEnabled()) ui->findButton->setEnabled(false);
}


void MainWindow::find()
{
    QString path = ui->picPathTextField_2->item(0)->text();
    im = new Intermediary(path);
    for(int index = 1; index < (ui->picPathTextField_2->count());index++)
    {
        im->addImage(ui->picPathTextField_2->item(index)->text());
    }

    if(im->imageOrTextHidden() == 1) //1 => text, 0 => picture
    { //gibt bei fehler (nix drin) 1 zur�ck
        qDebug("Hidden text found.");
        QString* plain = im->getHiddenText();
        //decrypt
        if(ui->decryptCheckBox->isChecked())
        {
            plain = decrypt(plain);
        }
        if(ui->textToFieldRadio->isChecked())
        {
            ui->picField->clear();
            ui->textEdit_2->setText(*plain);
        }else if(ui->textToDocRadio->isChecked())
        {
            QString newPath = QFileDialog::getSaveFileName(
                        this,
                        "Save Textfile",
                        actDir.absolutePath(),
                        "Text Files(*.txt)");
            actDir.setPath(newPath);
            QFile fileOut(newPath);
            if (fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream streamFileOut(&fileOut);
                streamFileOut.setCodec("UTF-8");
                streamFileOut.setGenerateByteOrderMark(true);
                streamFileOut << *plain;
                streamFileOut.flush();
                fileOut.close();
            }
        }
    }
    else if(im->imageOrTextHidden() == 0)
    {
        qDebug("Hidden image found.");
        QImage* image;
        image = im->getHiddenImage();
        if(ui->textToFieldRadio->isChecked())
        {
            ui->textEdit_2->clear();
            QPixmap img = QPixmap::fromImage(*image);
            if (img.height() > ui->picField->height() && img.width() > ui->picField->width())
            {
                img = img.scaled(ui->picField->width(), ui->picField->height());
            }
            else if(img.height() > ui->picField->height())
            {
                img = img.scaledToHeight(ui->picField->height());
            }
            else if(img.width() > ui->picField->width())
            {
                img = img.scaledToWidth(ui->picField->width());
            }
            ui->picField->setPixmap(img);
        }
        else{
            QString newPath = QFileDialog::getSaveFileName(
                        this,
                        "Save Image",
                        actDir.absolutePath(),
                        "Image Files(*.png)");
            actDir.setPath(newPath);
            image->save(newPath,"PNG",100);
        }
    }
    else{
        foundNothing();
    }
    ui->saveLabel->clear();
}



//others --------------------------------------------------------------------------------------

QString* MainWindow::decrypt(QString* cipher)
{
    Crypt c (cipher,&(ui->keyTextField_2->text()),format);
    switch (ui->techniqueComboBox_2->currentIndex())
    {
        case 0: //Caesar
            c.caesar(DECRYPT);
            break;
        case 1: //Vigen�re
            c.vigenere(DECRYPT);
            break;
        default:
            ui->keyTipLabel_2->setText("Decryption failed");
    }
    return cipher;
}

QString* MainWindow::encrypt(QString* plain)
{
    Crypt c (plain,&(ui->keyTextField->text()),format);
    switch (ui->techniqueComboBox->currentIndex())
    {
        case 0: //Caesar
            c.caesar(ENCRYPT);
            break;
        case 1: //Vigen�re
            c.vigenere(ENCRYPT);
            break;
        default:
            ui->keyTipLabel->setText("Encryption failed");
    }
    return plain;
}


int MainWindow::popupProblemDialog()
{
    pd = new ProblemDialog();
    pd->exec();
    return pd->result();
}

int MainWindow::noiseWarningDialog()
{
    nw = new NoiseWarning();
    nw->exec();
    return nw->result();
}

void MainWindow::notEnough()
{
    ne = new NotEnoughInfo();
    ne->exec();
}

void MainWindow::showSuccessfulHiding(QString savePath)
{
    sh = new SuccesfulHiding(savePath);
    sh->exec();
}

void MainWindow::picToSmall()
{
    ts = new PictureTooSmall();
    ts->exec();
}

void MainWindow::foundNothing()
{
    fn = new FoundNothing();
    fn->exec();
}