#include "GeneratorUI.h"
#include "ui_GeneratorUI.h"

GeneratorUI::GeneratorUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeneratorUI)
{
    ui->setupUi(this);
}

GeneratorUI::~GeneratorUI()
{
    delete ui;
}
