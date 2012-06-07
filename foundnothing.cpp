#include "foundnothing.h"
#include "ui_foundnothing.h"

FoundNothing::FoundNothing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FoundNothing)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowTitle("Stegosaur");

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()) );
}

FoundNothing::~FoundNothing()
{
    delete ui;
}
