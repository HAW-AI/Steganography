#include "noisewarning.h"
#include "ui_noisewarning.h"

//********************************************************************
//
// Class: NoiseWarning
// Author: Fenja Harbke
//
// Purpose: called by ProblemDialog, when "more information-density" is chosen
//          informs the user, that the host-picture might become noisy
//
//********************************************************************

NoiseWarning::NoiseWarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoiseWarning)
{
    ui->setupUi(this);
    setWindowTitle("Stegosaur");
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

NoiseWarning::~NoiseWarning()
{
    delete ui;
}
