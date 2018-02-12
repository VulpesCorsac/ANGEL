#include "LockInAmplifierUI.h"
#include "ui_LockInAmplifierUI.h"

LockInAmplifierUI::LockInAmplifierUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LockInAmplifierUI)
{
    ui->setupUi(this);
}

LockInAmplifierUI::~LockInAmplifierUI()
{
    delete ui;
}
