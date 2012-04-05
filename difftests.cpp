#include "difftests.h"
#include "ui_difftests.h"
#include "steganography.h"
#include "QTime"

DiffTests::DiffTests(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiffTests)
{
    ui->setupUi(this);
}

DiffTests::~DiffTests()
{
    delete ui;
}

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
    qDebug("Vor einfuegen");
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




}

