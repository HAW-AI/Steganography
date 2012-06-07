#include "picturetoosmall.h"
#include "ui_picturetoosmall.h"

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
