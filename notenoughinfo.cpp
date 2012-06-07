#include "notenoughinfo.h"
#include "ui_notenoughinfo.h"

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
