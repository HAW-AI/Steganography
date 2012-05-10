#include "problemdialog.h"
#include "ui_problemdialog.h"

ProblemDialog::ProblemDialog(QWidget *parent) :
    QWidget(parent),
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
    close();
    qDebug("cancle");
}

void ProblemDialog::moreDensity()
{
    qDebug("moreDensity");
}

void ProblemDialog::choseMorePics()
{
    qDebug("morePics");
}

