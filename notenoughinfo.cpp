#include "notenoughinfo.h"
#include "ui_notenoughinfo.h"

//********************************************************************
//
// Class: NotEnoughInfo
// Author: Fenja Harbke
//
// Purpose: called by ProblemDialog,
//          when highest information-density is not enough practice hide
//          inform the user, to chose another solution
//
//********************************************************************

NotEnoughInfo::NotEnoughInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotEnoughInfo)
{
    ui->setupUi(this);
    setWindowTitle("Stegosaur");
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(close()) );
}

NotEnoughInfo::~NotEnoughInfo()
{
    delete ui;
}
