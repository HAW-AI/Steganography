#include "problemdialog.h"
#include "ui_problemdialog.h"
#include <QDialog>

#define CANCLE 0
#define DENSITY 1
#define PICS 2

ProblemDialog::ProblemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProblemDialog)
{
    ui->setupUi(this);
    setWindowTitle("File does not fit into picture");
    setWindowFlags(Qt::Dialog);
    connect(ui->newPicButton, SIGNAL(clicked()), this, SLOT(choseNewPic()) );
    connect(ui->moreDensityButton, SIGNAL(clicked()), this, SLOT(moreDensity()) );
    connect(ui->morePicsButton, SIGNAL(clicked()), this, SLOT(choseMorePics()) );
}

ProblemDialog::~ProblemDialog()
{
    delete ui;
}

void ProblemDialog::choseNewPic()
{
    close(); //OK
    setResult(0);
}

void ProblemDialog::moreDensity()
{
    close();
    setResult(1); //Rejected
}

void ProblemDialog::choseMorePics()
{
    close();
    setResult(2); //Accepted
}

