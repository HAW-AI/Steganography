#include "succesfulhiding.h"
#include "ui_succesfulhiding.h"

//********************************************************************
//
// Class: SuccesfulHiding
// Author: Fenja Harbke
//
// Purpose: called by Mainwindow,
//          inform the user, that hiding was succesfull
//          output savepath
//
//********************************************************************

SuccesfulHiding::SuccesfulHiding(QString savePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SuccesfulHiding)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowTitle("Stegosaur");

    ui->label_3->setText(savePath);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));
}

SuccesfulHiding::~SuccesfulHiding()
{
    delete ui;
}
