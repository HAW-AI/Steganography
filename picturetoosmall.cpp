#include "picturetoosmall.h"
#include "ui_picturetoosmall.h"

//********************************************************************
//
// Class: PictureTooSmall
// Author: Fenja Harbke
//
// Purpose: called by MainWindow,
//          when the picture, chosen by chosePicture is to small to hold header
//          inform the user, that the picture can not be used as host
//
//********************************************************************

PictureTooSmall::PictureTooSmall(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PictureTooSmall)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowTitle("Stegosaur");

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()) );
}

PictureTooSmall::~PictureTooSmall()
{
    delete ui;
}
