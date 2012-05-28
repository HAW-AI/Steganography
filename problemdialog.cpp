#include "problemdialog.h"
#include "ui_problemdialog.h"
#include <QDialog>

#define CANCEL 0
#define DENSITY 1
#define PICS 2

ProblemDialog::ProblemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProblemDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog);
    connect(ui->newPicButton, SIGNAL(clicked()), this, SLOT(choseNewPic()) );
    connect(ui->moreDensityButton, SIGNAL(clicked()), this, SLOT(moreDensity()) );
    connect(ui->morePicsButton, SIGNAL(clicked()), this, SLOT(choseMorePics()) );
    //TODO: help-button belegen
}

ProblemDialog::~ProblemDialog()
{
    delete ui;
}

void ProblemDialog::choseNewPic()
{
    close();
    setResult(CANCEL);
}

void ProblemDialog::moreDensity()
{
    close();
    setResult(DENSITY);
}

void ProblemDialog::choseMorePics()
{
    close();
    setResult(PICS);
}

