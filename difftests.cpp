#include "difftests.h"
#include "ui_difftests.h"
#include "QTime"
#include "bitChanger.h"
#include "steganography.h"
#include "stringiterator.h"
#include "iostream"
#include "QImage"

#include <QtGui>

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

    //find-Buttons
    connect( ui->picBrowseButton_2, SIGNAL( clicked() ), this, SLOT( chosePicture() ) );
    connect( ui->findButton, SIGNAL( clicked() ), this, SLOT( find() ) );
    connect( ui->keyBrowseButton_2, SIGNAL( clicked() ), this, SLOT( browseOneTimePad() ) );
}

DiffTests::~DiffTests()
{
    delete ui;
}

void DiffTests::chosePicture()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                QDir::current().path(),
                "JPEG Files(*.jpg *.png)");  //JPEG files (*.jpg *.png);; Gif files (*.gif)
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
    if(ui->textFromDocRadio->isChecked()) plain = ui->textPathTextField->toPlainText();
    else plain = ui->textEdit->toPlainText();  //if(textFromFieldRadio)

    stego.insertText(plain);

    QString newPath = QFileDialog::getSaveFileName(this, tr("Save File"), picPath, tr("*.png *.jpg"));

    stego.saveImage(newPath);
    qDebug("Fertig!");
}

void DiffTests::find()
{
    QString picPath = ui->picPathTextField_2->toPlainText();
    Steganography stego(picPath);

    QString plain = stego.getText();
    ui->textEdit_2->setText(plain);
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



/*
void DiffTests::on_pushButton_clicked()
{
    QString t = ui->textEdit->document()->toPlainText();
    qDebug(t.toLatin1());
   Steganography s("C:\\Users\\Sebastian\\Desktop\\Testbild.png");
    qDebug("Bild geladen");
    QTime time;

    /*
    int needTime = 0;
    for(int i = 0; i < 20; i++){
        time.start();
        s.insertText(t);
        needTime += time.elapsed();
    }
    qDebug("Durchschnittlich %d ms bei alle pixel",needTime/20);
    needTime = 0;
    for(int i = 0; i < 20; i++){
        time.start();
        s.insertText2(t);
        needTime += time.elapsed();
    }
    qDebug("Durchschnittlich %d ms bei besondere pixel", needTime / 20);
    */


    //Test, ob es funktioniert
 /*   qDebug("Vor einfuegen");
    QString x = s.insertText(ui->textEdit->document()->toPlainText());
    qDebug("Einfuegen");
    qDebug(x.toLatin1());


    qDebug("nach einfuegen");
    s.saveImage("C:\\Users\\Sebastian\\Desktop\\Versteckt");
    qDebug("gespeichert");
    Steganography s1("C:\\Users\\Sebastian\\Desktop\\Versteckt.png");
    QString ausgabe = "Ausgabe: ";

    ausgabe.append(s1.getText());
    ui->textEdit->setText(ausgabe);




}*/

