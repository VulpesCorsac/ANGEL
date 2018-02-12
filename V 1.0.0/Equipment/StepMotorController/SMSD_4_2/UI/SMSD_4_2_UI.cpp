#include "SMSD_4_2_UI.h"
#include "ui_SMSD_4_2_UI.h"

SMSD_4_2_UI::SMSD_4_2_UI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SMSD_4_2_UI)
{
    qDebug() << "SMSD 4.2 UI constructor is called.";

    this->stepMotor = nullptr;

    ui->setupUi(this);

    ui->checkBoxAccelerationAccepted->setEnabled(false);
    ui->checkBoxSpeedAccepted->setEnabled(false);
    ui->checkBoxStartingSpeedAccepted->setEnabled(false);
    ui->comboBoxMode->setEnabled(true);
    ui->comboBoxMovementDirection->setEnabled(false);
    ui->lineEditCommand->setEnabled(false);
    ui->lineEditResponse->setEnabled(false);
    ui->lineEditSent->setEnabled(false);
    ui->pushButtonAbort->setEnabled(false);
    ui->pushButtonAccelerationAccept->setEnabled(false);
    ui->pushButtonAccelerationDefault->setEnabled(false);
    ui->pushButtonComAction->setEnabled(true);
    ui->pushButtonDisable->setEnabled(false);
    ui->pushButtonEnable->setEnabled(false);
    ui->pushButtonExit->setEnabled(true);
    ui->pushButtonMove->setEnabled(false);
    ui->pushButtonMoveUntillZero->setEnabled(false);
    ui->pushButtonPauseAccept->setEnabled(false);
    ui->pushButtonReverse->setEnabled(false);
    ui->pushButtonRun->setEnabled(false);
    ui->pushButtonSpeedAccept->setEnabled(false);
    ui->pushButtonSpeedDefault->setEnabled(false);
    ui->pushButtonStartingSpeedAccept->setEnabled(false);
    ui->pushButtonStartingSpeedDefault->setEnabled(false);
    ui->pushButtonStepAccept->setEnabled(false);
    ui->spinBoxAcceleration->setEnabled(false);
    ui->spinBoxPause->setEnabled(false);
    ui->spinBoxSpeed->setEnabled(false);
    ui->spinBoxStartingSpeed->setEnabled(false);
    ui->spinBoxStep->setEnabled(false);

    ui->pushButtonComAction->setText("Connect");

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->comboBoxCom->addItem(info.portName());

    ui->checkBoxAccelerationAccepted->setChecked(false);
    ui->checkBoxSpeedAccepted->setChecked(false);
    ui->checkBoxStartingSpeedAccepted->setChecked(false);

    ui->comboBoxMode->addItem("Text Commands");
    ui->comboBoxMode->setCurrentText("Text Commands");
    ui->comboBoxMode->addItem("Button Commands");

    ui->comboBoxMovementDirection->addItem("Left");
    ui->comboBoxMovementDirection->setCurrentText("Left");
    ui->comboBoxMovementDirection->addItem("Right");

    ui->spinBoxAcceleration->setMinimum(-1000);
    ui->spinBoxAcceleration->setMaximum(1000);
    ui->spinBoxAcceleration->setValue(this->defaultAcceleration);
    ui->spinBoxAcceleration->setSingleStep(1);

    ui->spinBoxPause->setMinimum(0);
    ui->spinBoxAcceleration->setMaximum(10000000);
    ui->spinBoxAcceleration->setValue(this->defaultPause);
    ui->spinBoxAcceleration->setSingleStep(1);

    ui->spinBoxSpeed->setMinimum(1);
    ui->spinBoxSpeed->setMaximum(10000);
    ui->spinBoxSpeed->setValue(this->defaultSpeed);
    ui->spinBoxSpeed->setSingleStep(1);

    ui->spinBoxStartingSpeed->setMinimum(1);
    ui->spinBoxStartingSpeed->setMaximum(2000);
    ui->spinBoxStartingSpeed->setValue(this->defaultStartingSpeed);
    ui->spinBoxStartingSpeed->setSingleStep(1);

    ui->spinBoxStep->setMinimum(1);
    ui->spinBoxStep->setMaximum(1000000);
    ui->spinBoxStep->setValue(this->defaultStep);
    ui->spinBoxStep->setSingleStep(1);

    qDebug() << "SMSD 4.2 UI constructor is closing.";
}

SMSD_4_2_UI::~SMSD_4_2_UI()
{
    qDebug() << "SMSD 4.2 UI destructor is called.";

//    this->closing();
    delete ui;

    qDebug() << "SMSD 4.2 UI destructor is closing.";
}

bool SMSD_4_2_UI::isNULL() const
{
    if (this->stepMotor == nullptr)
    {
        qDebug() << "SMSD 4.2 UI step motor pointer is NULL.";

        return true;
    }
    return false;
}

void SMSD_4_2_UI::closing()
{
    qDebug() << "SMSD 4.2 UI closing.";

    if (this->stepMotor != nullptr)
    {
        this->stepMotor->deactivate();
    }

    qDebug() << "SMSD 4.2 UI closed.";

    return;
}

void SMSD_4_2_UI::setStepMotor(SMSD_4_2* new_stepMotor)
{
    qDebug() << "SMSD 4.2 UI is setting to new step motor and it's pointer is " << new_stepMotor;

    this->stepMotor = new_stepMotor;

    if (new_stepMotor != nullptr)
    {
        connect(this->stepMotor, SIGNAL(response(QString)), this, SLOT(showResponse(QString)), Qt::DirectConnection);
        connect(this->stepMotor, SIGNAL(command(QString)), this, SLOT(showCommand(QString)), Qt::DirectConnection);

        if (this->stepMotor == nullptr || !this->stepMotor->isActive())
        {
            qDebug() << "NOT CONNECTED!!!";
        }
        else
        {
            QSerialPort *temp = this->stepMotor->getSerial();
            ui->comboBoxCom->setCurrentText(temp->portName());
            ui->pushButtonComAction->setText("Disconnect");
        }

        qDebug() << "SMSD 4.2 UI step motor is set to " << this->stepMotor;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI step motor is set to NULL";
    }

    return;
}

SMSD_4_2* SMSD_4_2_UI::getStepMotor() const
{
    qDebug() << "SMSD 4.2 UI step motor pointer was asked for and is " << this->stepMotor;

    return this->stepMotor;
}

void SMSD_4_2_UI::showResponse(const QString &s)
{
    qDebug() << "SMSD 4.2 UI sesponse for command is " << s;

    ui->lineEditResponse->setText(s);
}

void SMSD_4_2_UI::showCommand(const QString &s)
{
    qDebug() << "SMSD 4.2 UI command is " << s;

    ui->lineEditSent->setText(s);
}

void SMSD_4_2_UI::on_comboBoxMode_currentTextChanged(const QString &arg1)
{
    qDebug() << "SMSD 4.2 UI current mode is changing to " << ui->comboBoxMode->currentText();

    if (this->stepMotor == nullptr)
    {
        qDebug() << "SMSD 4.2 is not set yet, pointer is NULL";

        ui->comboBoxMode->setEnabled(true);
        ui->comboBoxMovementDirection->setEnabled(false);
        ui->lineEditCommand->setEnabled(false);
        ui->lineEditResponse->setEnabled(false);
        ui->lineEditSent->setEnabled(false);
        ui->pushButtonAbort->setEnabled(false);
        ui->pushButtonAccelerationAccept->setEnabled(false);
        ui->pushButtonAccelerationDefault->setEnabled(false);
        ui->pushButtonDisable->setEnabled(false);
        ui->pushButtonEnable->setEnabled(false);
        ui->pushButtonExit->setEnabled(true);
        ui->pushButtonMove->setEnabled(false);
        ui->pushButtonMoveUntillZero->setEnabled(false);
        ui->pushButtonPauseAccept->setEnabled(false);
        ui->pushButtonReverse->setEnabled(false);
        ui->pushButtonRun->setEnabled(false);
        ui->pushButtonSpeedAccept->setEnabled(false);
        ui->pushButtonSpeedDefault->setEnabled(false);
        ui->pushButtonStartingSpeedAccept->setEnabled(false);
        ui->pushButtonStartingSpeedDefault->setEnabled(false);
        ui->pushButtonStepAccept->setEnabled(false);
        ui->spinBoxAcceleration->setEnabled(false);
        ui->spinBoxPause->setEnabled(false);
        ui->spinBoxSpeed->setEnabled(false);
        ui->spinBoxStartingSpeed->setEnabled(false);
        ui->spinBoxStep->setEnabled(false);

        return;
    }

    if (ui->comboBoxMode->currentText() == "Button Commands")
    {
        if (this->stepMotor->isActive())
        {
            qDebug() << "SMSD 4.2 is active and all is OK";

            ui->comboBoxMode->setEnabled(true);
            ui->comboBoxMovementDirection->setEnabled(true);
            ui->lineEditCommand->setEnabled(false);
            ui->lineEditResponse->setEnabled(false);
            ui->lineEditSent->setEnabled(false);
            ui->pushButtonAbort->setEnabled(true);
            ui->pushButtonAccelerationAccept->setEnabled(true);
            ui->pushButtonAccelerationDefault->setEnabled(true);
            ui->pushButtonDisable->setEnabled(true);
            ui->pushButtonEnable->setEnabled(false);
            ui->pushButtonExit->setEnabled(true);
            ui->pushButtonMove->setEnabled(true);
            ui->pushButtonMoveUntillZero->setEnabled(true);
            ui->pushButtonPauseAccept->setEnabled(true);
            ui->pushButtonReverse->setEnabled(true);
            ui->pushButtonRun->setEnabled(false);
            ui->pushButtonSpeedAccept->setEnabled(true);
            ui->pushButtonSpeedDefault->setEnabled(true);
            ui->pushButtonStartingSpeedAccept->setEnabled(true);
            ui->pushButtonStartingSpeedDefault->setEnabled(true);
            ui->pushButtonStepAccept->setEnabled(true);
            ui->spinBoxAcceleration->setEnabled(true);
            ui->spinBoxPause->setEnabled(true);
            ui->spinBoxSpeed->setEnabled(true);
            ui->spinBoxStartingSpeed->setEnabled(true);
            ui->spinBoxStep->setEnabled(true);
        }
        else
        {
            qDebug() << "SMSD 4.2 is not active";

            ui->comboBoxMode->setEnabled(true);
            ui->comboBoxMovementDirection->setEnabled(false);
            ui->lineEditCommand->setEnabled(false);
            ui->lineEditResponse->setEnabled(false);
            ui->lineEditSent->setEnabled(false);
            ui->pushButtonAbort->setEnabled(false);
            ui->pushButtonAccelerationAccept->setEnabled(false);
            ui->pushButtonAccelerationDefault->setEnabled(false);
            ui->pushButtonDisable->setEnabled(false);
            ui->pushButtonEnable->setEnabled(true);
            ui->pushButtonExit->setEnabled(true);
            ui->pushButtonMove->setEnabled(false);
            ui->pushButtonMoveUntillZero->setEnabled(false);
            ui->pushButtonPauseAccept->setEnabled(false);
            ui->pushButtonReverse->setEnabled(false);
            ui->pushButtonRun->setEnabled(false);
            ui->pushButtonSpeedAccept->setEnabled(false);
            ui->pushButtonSpeedDefault->setEnabled(false);
            ui->pushButtonStartingSpeedAccept->setEnabled(false);
            ui->pushButtonStartingSpeedDefault->setEnabled(false);
            ui->pushButtonStepAccept->setEnabled(false);
            ui->spinBoxAcceleration->setEnabled(false);
            ui->spinBoxPause->setEnabled(false);
            ui->spinBoxSpeed->setEnabled(false);
            ui->spinBoxStartingSpeed->setEnabled(false);
            ui->spinBoxStep->setEnabled(false);
        }
    }
    if (ui->comboBoxMode->currentText() == "Text Commands")
    {
        qDebug() << "SMSD 4.2 UI text commands mode is activated.";

        ui->comboBoxMode->setEnabled(true);
        ui->comboBoxMovementDirection->setEnabled(false);
        ui->lineEditCommand->setEnabled(true);
        ui->lineEditResponse->setEnabled(false);
        ui->lineEditSent->setEnabled(false);
        ui->pushButtonAbort->setEnabled(true);
        ui->pushButtonAccelerationAccept->setEnabled(false);
        ui->pushButtonAccelerationDefault->setEnabled(false);
        ui->pushButtonDisable->setEnabled(false);
        ui->pushButtonEnable->setEnabled(false);
        ui->pushButtonExit->setEnabled(true);
        ui->pushButtonMove->setEnabled(false);
        ui->pushButtonMoveUntillZero->setEnabled(false);
        ui->pushButtonPauseAccept->setEnabled(false);
        ui->pushButtonReverse->setEnabled(false);
        ui->pushButtonRun->setEnabled(true);
        ui->pushButtonSpeedAccept->setEnabled(false);
        ui->pushButtonSpeedDefault->setEnabled(false);
        ui->pushButtonStartingSpeedAccept->setEnabled(false);
        ui->pushButtonStartingSpeedDefault->setEnabled(false);
        ui->pushButtonStepAccept->setEnabled(false);
        ui->spinBoxAcceleration->setEnabled(false);
        ui->spinBoxPause->setEnabled(false);
        ui->spinBoxSpeed->setEnabled(false);
        ui->spinBoxStartingSpeed->setEnabled(false);
        ui->spinBoxStep->setEnabled(false);
    }

    qDebug() << "SMSD 4.2 UI current mode changed to " << ui->comboBoxMode->currentText();

    return;
}

bool SMSD_4_2_UI::on_comboBoxMovementDirection_currentTextChanged(const QString &arg1)
{
    qDebug() << "SMSD 4.2 UI current movement direction is changing to " <<
                ui->comboBoxMovementDirection->currentText();

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->activate())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    if (this->stepMotor->setMovementDirection(ui->comboBoxMovementDirection->currentText() == "Right"))
    {
        qDebug() << "SMSD 4.2 UI current movement direction changed to " <<
                    ui->comboBoxMovementDirection->currentText();

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI current movement direction failed  to change to " <<
                    ui->comboBoxMovementDirection->currentText();

        return false;
    }

    return false;
}

void SMSD_4_2_UI::on_pushButtonExit_clicked()
{
    qDebug() << "SMSD 4.2 UI closing";

//    this->closing();
    this->close();

    return;
}

bool SMSD_4_2_UI::simpleCheating()
{
    qDebug() << "SMSD 4.2 UI cheating  round in disabling activated.";

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->activate())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    QString command = "MV1*RS*";
    QString response1, response2;
    this->stepMotor->sendCommand(command, response1);
    this->stepMotor->sendCommand(command, response2);
    if (response1 == "MV1*E10*RS*E10*" &&
        response2 == "MV1*E10*RS*E10*")
    {
        qDebug() << "SMSD 4.2 UI Cheating in deactivation succeeded.";

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI Cheating in deactivation failed.";

        return false;
    }

    return false;
}

bool SMSD_4_2_UI::cheating()
{
    qDebug() << "SMSD 4.2 UI cheating in disabling activated.";

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->activate())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    bool result = true;
    for (int i = 0; i < this->maxCheating; i++)
    {
        if (this->simpleCheating())
        {
            qDebug() << "Cheated in round " << i+1;

            result &= true;
        }
        else
        {
            qDebug() << "Failed in round " << i+1;

            result &= false;
        }
    }

    return result;
}

bool SMSD_4_2_UI::on_pushButtonEnable_clicked()
{
    qDebug() << "SMSD 4.2 UI enabling.";

    if (this->isNULL())
    {
        return false;
    }

    if (this->stepMotor->activate())
    {
        ui->comboBoxMode->setEnabled(true);
        ui->comboBoxMovementDirection->setEnabled(true);
        ui->lineEditCommand->setEnabled(false);
        ui->lineEditResponse->setEnabled(false);
        ui->pushButtonAbort->setEnabled(true);
        ui->pushButtonAccelerationAccept->setEnabled(true);
        ui->pushButtonAccelerationDefault->setEnabled(true);
        ui->pushButtonDisable->setEnabled(true);
        ui->pushButtonEnable->setEnabled(false);
        ui->pushButtonExit->setEnabled(true);
        ui->pushButtonMove->setEnabled(true);
        ui->pushButtonMoveUntillZero->setEnabled(true);
        ui->pushButtonPauseAccept->setEnabled(true);
        ui->pushButtonReverse->setEnabled(true);
        ui->pushButtonRun->setEnabled(true);
        ui->pushButtonSpeedAccept->setEnabled(true);
        ui->pushButtonSpeedDefault->setEnabled(true);
        ui->pushButtonStartingSpeedAccept->setEnabled(true);
        ui->pushButtonStartingSpeedDefault->setEnabled(true);
        ui->pushButtonStepAccept->setEnabled(true);
        ui->spinBoxAcceleration->setEnabled(true);
        ui->spinBoxPause->setEnabled(true);
        ui->spinBoxSpeed->setEnabled(true);
        ui->spinBoxStartingSpeed->setEnabled(true);
        ui->spinBoxStep->setEnabled(true);

        qDebug() << "SMSD 4.2 UI enabled.";

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI enabling failed.";

        return false;
    }

    return false;
}

bool SMSD_4_2_UI::on_pushButtonDisable_clicked()
{
    qDebug() << "SMSD 4.2 UI disabling.";

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->isActive())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    // it not to continue previous command!
    if (this->cheating())
    {
        qDebug() << "SMSD 4.2 UI cheating in deactivation succeeded.";
    }
    else
    {
        qDebug() << "SMSD 4.2 UI cheating in deactivation failed.";
    }

    if (this->stepMotor->deactivate())
    {
        ui->comboBoxMode->setEnabled(true);
        ui->comboBoxMovementDirection->setEnabled(false);
        ui->lineEditCommand->setEnabled(false);
        ui->lineEditResponse->setEnabled(false);
        ui->pushButtonAbort->setEnabled(false);
        ui->pushButtonAccelerationAccept->setEnabled(false);
        ui->pushButtonAccelerationDefault->setEnabled(false);
        ui->pushButtonDisable->setEnabled(false);
        ui->pushButtonEnable->setEnabled(true);
        ui->pushButtonExit->setEnabled(true);
        ui->pushButtonMove->setEnabled(false);
        ui->pushButtonMoveUntillZero->setEnabled(false);
        ui->pushButtonPauseAccept->setEnabled(false);
        ui->pushButtonReverse->setEnabled(false);
        ui->pushButtonRun->setEnabled(false);
        ui->pushButtonSpeedAccept->setEnabled(false);
        ui->pushButtonSpeedDefault->setEnabled(false);
        ui->pushButtonStartingSpeedAccept->setEnabled(false);
        ui->pushButtonStartingSpeedDefault->setEnabled(false);
        ui->pushButtonStepAccept->setEnabled(false);
        ui->spinBoxAcceleration->setEnabled(false);
        ui->spinBoxPause->setEnabled(false);
        ui->spinBoxSpeed->setEnabled(false);
        ui->spinBoxStartingSpeed->setEnabled(false);
        ui->spinBoxStep->setEnabled(false);

        ui->spinBoxAcceleration->setValue(this->defaultAcceleration);
        ui->spinBoxPause->setValue(this->defaultPause);
        ui->spinBoxSpeed->setValue(this->defaultSpeed);
        ui->spinBoxStartingSpeed->setValue(this->defaultStartingSpeed);
        ui->spinBoxStep->setValue(this->defaultStep);

        ui->comboBoxMovementDirection->setCurrentText("Left");

        ui->checkBoxAccelerationAccepted->setChecked(false);
        ui->checkBoxSpeedAccepted->setChecked(false);
        ui->checkBoxStartingSpeedAccepted->setChecked(false);

        qDebug() << "SMSD 4.2 UI disabled.";

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI disabling failed.";

        return false;
    }

    return false;
}

bool SMSD_4_2_UI::on_pushButtonMoveUntillZero_clicked()
{
    qDebug() << "SMSD 4.2 UI move untill zero was called.";

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->activate())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    if (this->stepMotor->setMovementDirection(ui->comboBoxMovementDirection->currentText() ==
                                              "Right") && this->stepMotor->moveUntillZero())
    {
        qDebug() << "SMSD 4.2 moving untill Zero is activated.";

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 moving untill Zero failed.";

        return false;
    }

    return false;
}

bool SMSD_4_2_UI::on_pushButtonMove_clicked()
{
    qDebug() << "SMSD 4.2 UI move button is pressed.";

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->activate())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    if (this->stepMotor->setMovementDirection(ui->comboBoxMovementDirection->currentText() ==
                                              "Right") && this->stepMotor->movement())
    {
        qDebug() << "SMSD 4.2 UI movement started.";

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI movement failed.";

        return false;
    }

    return false;
}

bool SMSD_4_2_UI::on_pushButtonReverse_clicked()
{
    qDebug() << "SMSD 4.2 UI reverse button is pressed.";

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->activate())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    if (this->stepMotor->setReverse())
    {
        qDebug() << "SMSD 4.2 UI reverse started.";

        if (ui->comboBoxMovementDirection->currentText() == "Left")
        {
            ui->comboBoxMovementDirection->setCurrentText("Right");
        }
        else
        {
            ui->comboBoxMovementDirection->setCurrentText("Left");
        }

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI reverse failed.";

        return false;
    }

    return false;
}

void SMSD_4_2_UI::on_spinBoxAcceleration_valueChanged(int arg1)
{
    qDebug() << "SMSD 4.2 UI acceleration spin box changed to " << ui->spinBoxAcceleration->value();

    ui->checkBoxAccelerationAccepted->setChecked(false);

    return;
}

void SMSD_4_2_UI::on_spinBoxPause_valueChanged(int arg1)
{
    qDebug() << "SMSD 4.2 UI pause spin box changed to " << ui->spinBoxPause->value();

    return;
}

void SMSD_4_2_UI::on_spinBoxSpeed_valueChanged(int arg1)
{
    qDebug() << "SMSD 4.2 UI speed spin box changed to " << ui->spinBoxSpeed->value();

    ui->checkBoxSpeedAccepted->setChecked(false);

    return;
}

void SMSD_4_2_UI::on_spinBoxStartingSpeed_valueChanged(int arg1)
{
    qDebug() << "SMSD 4.2 UI starting speed spin box changed to " << ui->spinBoxStartingSpeed->value();

    ui->checkBoxStartingSpeedAccepted->setChecked(false);

    return;
}

void SMSD_4_2_UI::on_spinBoxStep_valueChanged(int arg1)
{
    qDebug() << "SMSD 4.2 UI step spin box changed to " << ui->spinBoxStep->value();

    return;
}

bool SMSD_4_2_UI::on_pushButtonAccelerationAccept_clicked()
{
    qDebug() << "SMSD 4.2 UI setting acceleration to " << ui->spinBoxAcceleration->value();

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->isActive())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    if (this->stepMotor->setAcceleration(ui->spinBoxAcceleration->value()))
    {
        qDebug() << "SMSD 4.2 UI acceleration successfully set to " <<
                    this->stepMotor->getAcceleration();

        ui->checkBoxAccelerationAccepted->setChecked(true);

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI acceleration failed to set to " <<
                    ui->spinBoxAcceleration->value() <<
                    " and it continues to be " << this->stepMotor->getAcceleration();

        ui->checkBoxAccelerationAccepted->setChecked(false);

        return false;
    }

    return false;
}

bool SMSD_4_2_UI::on_pushButtonAccelerationDefault_clicked()
{
    qDebug() << "SMSD 4.2 UI acceleration is setting to it's default value: " <<
                this->defaultAcceleration;

    ui->spinBoxAcceleration->setValue(this->defaultAcceleration);

    return this->on_pushButtonAccelerationAccept_clicked();
}

bool SMSD_4_2_UI::on_pushButtonPauseAccept_clicked()
{
    qDebug() << "SMSD 4.2 UI setting pause to " << ui->spinBoxPause->value();

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->activate())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    if (this->stepMotor->setPause(ui->spinBoxPause->value()))
    {
        qDebug() << "SMSD 4.2 UI acceleration successfully set to " << ui->spinBoxPause->value();

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI acceleration failed to set to " << ui->spinBoxPause->value();

        return false;
    }

    return false;
}

bool SMSD_4_2_UI::on_pushButtonSpeedAccept_clicked()
{
    qDebug() << "SMSD 4.2 UI setting speed to " << ui->spinBoxSpeed->value();

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->isActive())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    if (this->stepMotor->setSpeed(ui->spinBoxSpeed->value()))
    {
        qDebug() << "SMSD 4.2 UI speed successfully set to " << this->stepMotor->getSpeed();

        ui->checkBoxSpeedAccepted->setChecked(true);

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI speed failed to set to " << ui->spinBoxSpeed->value() <<
                    " and it continues to be " << this->stepMotor->getSpeed();

        ui->checkBoxSpeedAccepted->setChecked(false);

        return false;
    }

    return false;
}

bool SMSD_4_2_UI::on_pushButtonSpeedDefault_clicked()
{
    qDebug() << "SMSD 4.2 UI speed is setting to it's default value: " << this->defaultSpeed;

    ui->spinBoxSpeed->setValue(this->defaultSpeed);

    return this->on_pushButtonSpeedAccept_clicked();
}

bool SMSD_4_2_UI::on_pushButtonStartingSpeedAccept_clicked()
{
    qDebug() << "SMSD 4.2 UI setting starting speed to " << ui->spinBoxStartingSpeed->value();

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->isActive())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    if (this->stepMotor->setStartingSpeed(ui->spinBoxStartingSpeed->value()))
    {
        if (this->stepMotor != nullptr)
        {
            qDebug() << "SMSD 4.2 UI starting speed successfully set to " <<
                        this->stepMotor->getStartingSpeed();
        }

        ui->checkBoxStartingSpeedAccepted->setChecked(true);

        return true;
    }
    else
    {
        if (this->stepMotor != nullptr)
        {
            qDebug() << "SMSD 4.2 UI starting speed failed to set to " <<
                        ui->spinBoxStartingSpeed->value() <<
                        " and it continues to be " << this->stepMotor->getStartingSpeed();
        }

        ui->checkBoxStartingSpeedAccepted->setChecked(false);

        return false;
    }

    return false;
}

bool SMSD_4_2_UI::on_pushButtonStartingSpeedDefault_clicked()
{
    qDebug() << "SMSD 4.2 UI starting speed is setting to it's default value: " <<
                this->defaultStartingSpeed;

    ui->spinBoxStartingSpeed->setValue(this->defaultStartingSpeed);

    return this->on_pushButtonStartingSpeedAccept_clicked();
}

bool SMSD_4_2_UI::on_pushButtonStepAccept_clicked()
{
    qDebug() << "SMSD 4.2 UI moving for " << ui->spinBoxStep->value() << " steps";

    if (this->isNULL())
    {
        return false;
    }

    if (!this->stepMotor->activate())
    {
        qDebug() << "SMSD 4.2 is not active.";

        return false;
    }

    if (this->stepMotor->moveStep(ui->spinBoxSpeed->value(),
                                  ui->comboBoxMovementDirection->currentText() == "Right",
                                  ui->spinBoxStep->value()))
    {
        qDebug() << "SMSD 4.2 succeeded in moving for " << ui->spinBoxStep->value() << " steps";

        return true;
    }
    else
    {
        qDebug() << "SMSD 4.2 failed in moving for " << ui->spinBoxStep->value() << " steps";

        return false;
    }

    return false;
}

void SMSD_4_2_UI::on_pushButtonRun_clicked()
{
    QString command = "EN*" + ui->lineEditCommand->text();
    QString response;

    if (ui->comboBoxMode->currentText() == "Text Commands")
    {
        qDebug() << "SMSD 4.2 UI running program from user.";

        this->stepMotor->sendCommand(command, response);

        qDebug() << "SMSD 4.2 UI command was sent.";
    }

    if(ui->comboBoxMode->currentText() == "Button Commands")
    {
        qDebug() << "SMSD 4.2 UI running program from button settings.";

        if (ui->comboBoxMovementDirection->currentText() == "Right")
        {
            command += "DR*";
        }
        else
        {
            command += "DL*";
        }
        command += "SS" + QString::number(ui->spinBoxStartingSpeed->value()) + "*";
        command += "AL" + QString::number(ui->spinBoxAcceleration->value()) + "*";
        command += "SD" + QString::number(ui->spinBoxSpeed->value()) + "*";
        command += "MV" + QString::number(ui->spinBoxStep->value()) + "*";

        this->stepMotor->sendCommand(command, response);

        qDebug() << "SMSD 4.2 UI command was sent.";
    }

    return;
}

bool SMSD_4_2_UI::on_pushButtonAbort_clicked()
{
    qDebug() << "SMSD 4.2 UI aborting current programm.";

    bool result = true;

    result &= this->on_pushButtonDisable_clicked();
    result &= this->on_pushButtonEnable_clicked();

    qDebug() << "SMSD 4.2 UI current program aborted.";

    return result;
}

void SMSD_4_2_UI::on_pushButtonComAction_clicked()
{
    qDebug() << "SMSD 4.2 UI COM Action";

    if (this->stepMotor == nullptr)
    {
        qDebug() <<"SMSD 4.2 pointer is NULL and needs to be created";

        this->stepMotor = new SMSD_4_2();
    }

    if (ui->pushButtonComAction->text() == "Connect")
    {
        qDebug() << "SMSD 4.2 UI connecting to stepmotor";

        this->stepMotor->setConnection(ui->comboBoxCom->currentText());
        ui->pushButtonComAction->setText("Disconnect");
        return;
    }
    else
    {
        qDebug() << "SMSD 4.2 UI disconnecting from stepmotor";

        this->stepMotor->disconnect();
        ui->pushButtonComAction->setText("Connect");
        return;
    }
}
