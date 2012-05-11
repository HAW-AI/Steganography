#include "problemdialog.h"
#include "ui_problemdialog.h"
#include <QDialog>

#define CANCLE 0
#define MOREDENSITY 1
#define MOREPICS 2

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
}

void ProblemDialog::moreDensity()
{
    close();
}

void ProblemDialog::choseMorePics()
{
    close();
}

