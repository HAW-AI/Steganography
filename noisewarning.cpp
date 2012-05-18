#include "noisewarning.h"
#include "ui_noisewarning.h"

NoiseWarning::NoiseWarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoiseWarning)
{
    ui->setupUi(this);
    setWindowTitle("Noise warning");
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

NoiseWarning::~NoiseWarning()
{
    delete ui;
}
