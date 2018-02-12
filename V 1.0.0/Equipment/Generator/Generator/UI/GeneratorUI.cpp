#include "GeneratorUI.h"
#include "ui_GeneratorUI.h"

GeneratorUI::GeneratorUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeneratorUI)
{
    ui->setupUi(this);

    this->generator = new Generator();

    ui->groupBoxMainSettings->setVisible(false);
    ui->groupBoxModulationSettings->setVisible(false);

    filling = true;

    on_actionComListRefresh_triggered();

    ui->comboBoxGeneratorModel->addItems(this->generator->getSupportedModels());
    ui->comboBoxGeneratorModel->setCurrentText("AUTO");

    ui->comboBoxGeneratorBaud->addItem("9600");
    ui->comboBoxGeneratorBaud->addItem("19200");
    ui->comboBoxGeneratorBaud->addItem("AUTO");
    ui->comboBoxGeneratorBaud->setCurrentText("AUTO");

    ui->comboBoxGeneratorStopBit->addItem("1");
    ui->comboBoxGeneratorStopBit->addItem("2");
    ui->comboBoxGeneratorStopBit->addItem("AUTO");
    ui->comboBoxGeneratorStopBit->setCurrentText("AUTO");

    filling = false;

    ui->pushButtonComAction->setText("Сonnect");
}

GeneratorUI::GeneratorUI(Generator_State *currentState)
{
    // TODO
}

GeneratorUI::~GeneratorUI()
{
    delete ui;
    delete this->generator;
}

void GeneratorUI::on_actionComListRefresh_triggered()
{
    ui->comboBoxGeneratorPort->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBoxGeneratorPort->addItem(info.portName());
    }

}

void GeneratorUI::on_comboBoxGeneratorModel_currentTextChanged(const QString &arg1)
{
    if (this->filling)
        return;

    if (arg1 == "AUTO") {
        ui->comboBoxGeneratorStopBit->setCurrentText("AUTO");
        ui->comboBoxGeneratorStopBit->setEnabled(true);
    } else {
        if (arg1 == "DS335")
            ui->comboBoxGeneratorStopBit->setCurrentText("2");
        if (arg1 == "DS345")
            ui->comboBoxGeneratorStopBit->setCurrentText("1");
        ui->comboBoxGeneratorStopBit->setEnabled(false);
    }
}

void GeneratorUI::on_pushButtonComAction_clicked()
{
    if (ui->pushButtonComAction->text() == "Connect")
    {

    } else {

    }

    return;
}
