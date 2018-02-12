#include "MDG_23_by_Geo_Stress_Test_Simple.h"
#include "ui_MDG_23_by_Geo_Stress_Test_Simple.h"

MDG_23_by_Geo_Stress_Test_Simple::MDG_23_by_Geo_Stress_Test_Simple(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MDG_23_by_Geo_Stress_Test_Simple)
{
    qDebug() << "MDG 23 by Geo simple stress test constructor is called.";

    this->monochromator = new MDG_23_by_Geo();

    ui->setupUi(this);

    ui->checkBoxExtraFine->setEnabled(false);
    ui->comboBoxMovementDirection->setEnabled(true);
    ui->doubleSpinBoxStartingPoint->setEnabled(true);
    ui->pushButtonAction->setEnabled(false);
    ui->pushButtonCoarseLeft->setEnabled(false);
    ui->pushButtonCoarseRight->setEnabled(false);
    ui->pushButtonFineLeft->setEnabled(false);
    ui->pushButtonFineRight->setEnabled(false);
    ui->pushButtonStartingPoint->setEnabled(true);
    ui->spinBoxRepeat->setEnabled(false);
    ui->spinBoxStep->setEnabled(false);
    ui->spinBoxTimes->setEnabled(false);

    ui->doubleSpinBoxStartingPoint->setMinimum(this->monochromator->getMinDisplay());
    ui->doubleSpinBoxStartingPoint->setMaximum(this->monochromator->getMaxDisplay());
    ui->doubleSpinBoxStartingPoint->setDecimals(2);
    ui->doubleSpinBoxStartingPoint->setValue(500.00);

    ui->spinBoxRepeat->setMaximum(50);
    ui->spinBoxRepeat->setMinimum(1);
    ui->spinBoxRepeat->setValue(1);

    ui->spinBoxStep->setMaximum(1000000);
    ui->spinBoxStep->setMinimum(1);
    ui->spinBoxStep->setValue(100000);

    ui->spinBoxTimes->setMaximum(20);
    ui->spinBoxTimes->setMinimum(1);
    ui->spinBoxTimes->setValue(1);

    ui->comboBoxMovementDirection->addItem("Left");
    ui->comboBoxMovementDirection->addItem("Right");
    ui->comboBoxMovementDirection->setCurrentText("Right");

    delete this->monochromator;
    this->monochromator = nullptr;

    qDebug() << "MDG 23 by Geo simple stress test constructor is closing.";
}

MDG_23_by_Geo_Stress_Test_Simple::~MDG_23_by_Geo_Stress_Test_Simple()
{
    qDebug() << "MDG 23 by Geo simple stress test destructor is called.";

//    this->closing();
    delete ui;

    qDebug() << "MDG 23 by Geo simple stress test destructor is closing.";
}

bool MDG_23_by_Geo_Stress_Test_Simple::isNULL() const
{
    if (this->monochromator == nullptr)
    {
        qDebug() << "MDG 23 by Geo simple stress test monochromator pointer is NULL.";

        return true;
    }
    if (this->monochromator->getStepMotorController() == nullptr)
    {
        qDebug() << "MDG 23 by Geo simple stress test step motor pointer is NULL.";

        return true;
    }
    return false;
}

void MDG_23_by_Geo_Stress_Test_Simple::closing()
{
    qDebug() << "MDG 23 by Geo simple stress test closing.";

    SMSD_4_2* stepMotor = nullptr;
    if (this->monochromator != nullptr)
    {
        stepMotor = this->monochromator->getStepMotorController();
    }
    if (stepMotor != nullptr)
    {
        stepMotor->deactivate();
    }

    qDebug() << "MDG 23 by Geo simple stress test closed.";

    return;
}

void MDG_23_by_Geo_Stress_Test_Simple::setMonochromator(MDG_23_by_Geo *new_Monochromator)
{
    qDebug() << "MDG 23 by Geo simple stress test is setting to new monochromator and it's pointer is " <<
                new_Monochromator;

    this->monochromator = new_Monochromator;

    if (new_Monochromator != nullptr)
    {
        qDebug() << "MDG 23 by Geo simple stress test monochromator is set to " << this->monochromator;

        SMSD_4_2* stepMotor = this->monochromator->getStepMotorController();
        if (stepMotor->getStepDenominator() != 16)
        {
            qDebug() << "MDG 23 by Geo simple stress test decreasing speed.";

            this->speed = 1000;
        }
    }
    else
    {
        qDebug() << "MDG 23 by Geo simple stress test monochromator is set to NULL";
    }

    return;
}

MDG_23_by_Geo* MDG_23_by_Geo_Stress_Test_Simple::getMonochromator() const
{
    qDebug() << "MDG 23 by Geo simple stress test MDG 23 pointer was asked for and is " << this->monochromator;

    return this->monochromator;

}

bool MDG_23_by_Geo_Stress_Test_Simple::movement()
{
    qDebug() << "MDG 23 by Geo simple stress test movement starting.";

    if (this->isNULL())
    {
        return false;
    }

    bool result = true;
    SMSD_4_2* stepMotor = this->monochromator->getStepMotorController();

    for (int i = 0; i < ui->spinBoxTimes->value(); i++)
    {
        qDebug() << "Phase number: " << i;

        if (i > 0)
        {
            qDebug() << "MDG 23 by Geo waiting pause between times.";

            QTest::qWait(1000.0*(this->times_pause_seconds));
        }

        ui->labelCurrentTimes->setText("Current times " + QString::number(i + 1));

        QTime passed = QTime(0, 0, 0, 0).addMSecs(timer.elapsed());
        ui->labelTimePassed->setText("Passed " + passed.toString("hh:mm:ss.zzz"));

        QTime remained = QTime(0, 0, 0, 0);
        if (1000*this->time_length - timer.elapsed() > 0)
        {
            remained = remained.addMSecs(1000*this->time_length - timer.elapsed());
            ui->labelTimeRemain->setText("Remained " + remained.toString("hh:mm:ss.zzz"));
        }
        else
        {
            ui->labelTimeRemain->setText("Stress test took longer, than expected...");
        }

        result &= stepMotor->moveStep(this->speed, ui->comboBoxMovementDirection->currentText() == "Right",
                                      ui->spinBoxStep->value());

        qDebug() << "MDG 23 by Geo waiting command to finish.";

        QTest::qWait(_round(1000.0*(1.0 * ui->spinBoxStep->value() / this->speed)));
    }

    qDebug() << "MDG 23 by Geo simple stress test movement finished.";

    return result;
}

bool MDG_23_by_Geo_Stress_Test_Simple::movement_and_swap()
{
    qDebug() << "MDG 23 by Geo simple stress test movement and swap starting.";

    if (this->isNULL())
    {
        return false;
    }

    bool result = this->movement();
    if (ui->comboBoxMovementDirection->currentText() == "Left")
    {
        ui->comboBoxMovementDirection->setCurrentText("Right");
    }
    else
    {
        ui->comboBoxMovementDirection->setCurrentText("Left");
    }

    qDebug() << "MDG 23 by Geo simple stress test movement and swap closing.";

    return result;
}

bool MDG_23_by_Geo_Stress_Test_Simple::round()
{
    qDebug() << "MDG 23 by Geo simple stress test simple round starting.";

    ui->labelCurrentDirection->setText("Forward");
    bool result = this->movement_and_swap();

    qDebug() << "MDG 23 by Geo waiting pause before reverse.";

    QTest::qWait(1000*this->reverse_pause_seconds);

    ui->labelCurrentDirection->setText("Backwards");
    result &= this->movement_and_swap();

    qDebug() << "MDG 23 by Geo simple stress test simple round closing.";

    return result;
}

void MDG_23_by_Geo_Stress_Test_Simple::on_pushButtonStartingPoint_clicked()
{
    qDebug() << "MDG 23 by Geo MDG 23 simple stress test starting point set.";

    ui->pushButtonAction->setText("Start");

    ui->checkBoxExtraFine->setEnabled(false);
    ui->comboBoxMovementDirection->setEnabled(true);
    ui->doubleSpinBoxStartingPoint->setEnabled(false);
    ui->pushButtonAction->setEnabled(true);
    ui->pushButtonCoarseLeft->setEnabled(false);
    ui->pushButtonCoarseRight->setEnabled(false);
    ui->pushButtonFineLeft->setEnabled(false);
    ui->pushButtonFineRight->setEnabled(false);
    ui->pushButtonStartingPoint->setEnabled(false);
    ui->spinBoxRepeat->setEnabled(true);
    ui->spinBoxStep->setEnabled(true);
    ui->spinBoxTimes->setEnabled(true);

    return;
}

void MDG_23_by_Geo_Stress_Test_Simple::on_pushButtonAction_clicked()
{
    qDebug() << "MDG 23 by Geo simple stress test action button clicked.";

    int steps = ui->spinBoxRepeat->text().toInt() * ui->spinBoxTimes->text().toInt() *
            2 * ui->spinBoxStep->text().toInt();

    if (steps < 100*1000*1000)
    {
        this->repeat_pause_seconds = 3;
        this->reverse_pause_seconds = 2;
    }
    if (steps < 10*1000*1000)
    {
        this->repeat_pause_seconds = 2;
        this->reverse_pause_seconds = 1;
    }

    this->time_length = (ui->spinBoxRepeat->text().toInt() - 1) * this->repeat_pause_seconds +
                        ui->spinBoxRepeat->text().toInt() *
                        (this->reverse_pause_seconds + 2 * (
                        ui->spinBoxTimes->text().toInt() - 1 +
                        ui->spinBoxTimes->text().toInt() *
                        ui->spinBoxStep->text().toInt() / this->speed));

    qDebug() << "MDG 23 by Geo simple stress test will approximately take " << this->time_length << " seconds";

    QTime time_length_time = QTime(0,0,0,0).addSecs(this->time_length);
    ui->labelTimeAll->setText("This will take approximately " + time_length_time.toString("hh:mm:ss.zzz"));

    if (this->isNULL())
    {
        return;
    }

    SMSD_4_2* stepMotor = this->monochromator->getStepMotorController();
    stepMotor->activate();

    if (ui->pushButtonAction->text() == "Start")
    {
        qDebug() << "MDG 23 by Geo simple stress test starting.";

        ui->comboBoxMovementDirection->setEnabled(false);
        ui->spinBoxRepeat->setEnabled(false);
        ui->spinBoxStep->setEnabled(false);
        ui->spinBoxTimes->setEnabled(false);
        ui->pushButtonAction->setEnabled(false);

        timer = QTime::currentTime();
        timer.start();

        for (int repeat = 0; repeat < ui->spinBoxRepeat->text().toInt(); repeat++)
        {
            qDebug() << "MDG 23 by Geo simple stress test repeat " << repeat + 1;

            if (repeat > 0)
            {
                qDebug() << "MDG 23 by Geo waiting pause between repeats.";

                QTest::qWait(1000*this->repeat_pause_seconds);
            }
            ui->labelCurrentRepeat->setText("Current repeat " + QString::number(repeat + 1));
            this->round();
        }

        ui->pushButtonAction->setText("Results");

        ui->checkBoxExtraFine->setEnabled(true);
        ui->pushButtonAction->setEnabled(true);
        ui->pushButtonCoarseLeft->setEnabled(true);
        ui->pushButtonCoarseRight->setEnabled(true);
        ui->pushButtonFineLeft->setEnabled(true);
        ui->pushButtonFineRight->setEnabled(true);

        return;
    }

    if (ui->pushButtonAction->text() == "Results")
    {
        qDebug() << "MDG 23 by Geo simple stress test showing results.";

        ui->checkBoxExtraFine->setEnabled(false);
        ui->pushButtonCoarseLeft->setEnabled(false);
        ui->pushButtonCoarseRight->setEnabled(false);
        ui->pushButtonFineLeft->setEnabled(false);
        ui->pushButtonFineRight->setEnabled(false);

        QString Results = "Mismatch for " +
                QString::number(100.0*this->additionalSteps/
                                (ui->spinBoxStep->text().toInt() *
                                 ui->spinBoxTimes->text().toInt())) +
                "%" +
                " from max and " +
                QString::number(100.0*this->additionalSteps/
                        (2 * ui->spinBoxStep->text().toInt() *
                         ui->spinBoxTimes->text().toInt()) *
                         ui->spinBoxRepeat->text().toInt()) +
                "%" +
                " from all";
        ui->labelResults->setText(Results);
        ui->pushButtonAction->setText("Exit");

        return;
    }

    if (ui->pushButtonAction->text() == "Exit")
    {
        qDebug() << "MDG 23 by Geo simple stress test exiting.";

//        this->closing();
        this->close();

        return;
    }

    qDebug() << "MDG 23 by Geo simple stress test unknown action command.";

    return;
}

bool MDG_23_by_Geo_Stress_Test_Simple::on_pushButtonCoarseLeft_clicked()
{
    qDebug() << "MDG 23 by Geo simple stress test coarse left was called.";

    if (this->isNULL())
    {
        return false;
    }

    SMSD_4_2* stepMotor = this->monochromator->getStepMotorController();

    if (ui->comboBoxMovementDirection->currentText() == "Left")
    {
        if (!ui->checkBoxExtraFine->isChecked())
        {
            if (stepMotor->moveStep(this->speed, false, this->monochromator->getCoarseStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test coarse left movement is Ok.";

                this->additionalSteps += this->monochromator->getCoarseStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test coarse left movement failed.";

                return false;
            }
        }
        else
        {
            if (stepMotor->moveStep(this->speed, false, this->monochromator->getExtraFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test extra fine left movement is Ok.";

                this->additionalSteps += this->monochromator->getExtraFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test extra fine left movement failed.";

                return false;
            }
        }
    }
    else
    {
        if (!ui->checkBoxExtraFine->isChecked())
        {
            if (stepMotor->moveStep(this->speed, false, this->monochromator->getCoarseStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test coarse left movement is Ok.";

                this->additionalSteps -= this->monochromator->getCoarseStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test coarse left movement failed.";

                return false;
            }
        }
        else
        {
            if (stepMotor->moveStep(this->speed, false, this->monochromator->getExtraFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test extra fine left movement is Ok.";

                this->additionalSteps -= this->monochromator->getExtraFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test extra fine left movement failed.";

                return false;
            }
        }
    }
    return false;
}

bool MDG_23_by_Geo_Stress_Test_Simple::on_pushButtonCoarseRight_clicked()
{
    qDebug() << "MDG 23 by Geo simple stress test coarse right was called.";

    if (this->isNULL())
    {
        return false;
    }

    SMSD_4_2* stepMotor = this->monochromator->getStepMotorController();

    if (ui->comboBoxMovementDirection->currentText() == "Right")
    {
        if (!ui->checkBoxExtraFine->isChecked())
        {
            if (stepMotor->moveStep(this->speed, true, this->monochromator->getCoarseStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test coarse right movement is Ok.";

                this->additionalSteps += this->monochromator->getCoarseStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test coarse right movement failed.";

                return false;
            }
        }
        else
        {
            if (stepMotor->moveStep(this->speed, true, this->monochromator->getExtraFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test extra fine right movement is Ok.";

                this->additionalSteps += this->monochromator->getExtraFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test extra fine right movement failed.";

                return false;
            }
        }
    }
    else
    {
        if (!ui->checkBoxExtraFine->isChecked())
        {
            if (stepMotor->moveStep(this->speed, true, this->monochromator->getCoarseStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test coarse right movement is Ok.";

                this->additionalSteps -= this->monochromator->getCoarseStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test coarse right movement failed.";

                return false;
            }
        }
        else
        {
            if (stepMotor->moveStep(this->speed, true, this->monochromator->getExtraFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test extra fine right movement is Ok.";

                this->additionalSteps -= this->monochromator->getExtraFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test extra fine right movement failed.";

                return false;
            }
        }
    }
    return false;
}

bool MDG_23_by_Geo_Stress_Test_Simple::on_pushButtonFineLeft_clicked()
{
    qDebug() << "MDG 23 by Geo simple stress test fine left was called.";

    if (this->isNULL())
    {
        return false;
    }

    SMSD_4_2* stepMotor = this->monochromator->getStepMotorController();

    if (ui->comboBoxMovementDirection->currentText() == "Left")
    {
        if (!ui->checkBoxExtraFine->isChecked())
        {
            if (stepMotor->moveStep(this->speed, false, this->monochromator->getFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test fine left movement is Ok.";

                this->additionalSteps += this->monochromator->getFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test fine left movement failed.";

                return false;
            }
        }
        else
        {
            if (stepMotor->moveStep(this->speed, false, this->monochromator->getUltraFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test ultra fine left movement is Ok.";

                this->additionalSteps += this->monochromator->getUltraFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test ultra fine left movement failed.";

                return false;
            }
        }
    }
    else
    {
        if (!ui->checkBoxExtraFine->isChecked())
        {
            if (stepMotor->moveStep(this->speed, false, this->monochromator->getFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test fine left movement is Ok.";

                this->additionalSteps -= this->monochromator->getFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test fine left movement failed.";

                return false;
            }
        }
        else
        {
            if (stepMotor->moveStep(this->speed, false, this->monochromator->getUltraFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test ultra fine left movement is Ok.";

                this->additionalSteps -= this->monochromator->getUltraFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test utra fine left movement failed.";

                return false;
            }
        }
    }
    return false;
}

bool MDG_23_by_Geo_Stress_Test_Simple::on_pushButtonFineRight_clicked()
{
    qDebug() << "MDG 23 by Geo simple stress test fine right was called.";

    if (this->isNULL())
    {
        return false;
    }

    SMSD_4_2* stepMotor = this->monochromator->getStepMotorController();

    if (ui->comboBoxMovementDirection->currentText() == "Right")
    {
        if (!ui->checkBoxExtraFine->isChecked())
        {
            if (stepMotor->moveStep(this->speed, true, this->monochromator->getFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test fine right movement is Ok.";

                this->additionalSteps += this->monochromator->getFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test fine right movement failed.";

                return false;
            }
        }
        else
        {
            if (stepMotor->moveStep(this->speed, true, this->monochromator->getUltraFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test ultra fine right movement is Ok.";

                this->additionalSteps += this->monochromator->getUltraFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test ultra fine right movement failed.";

                return false;
            }
        }
    }
    else
    {
        if (!ui->checkBoxExtraFine->isChecked())
        {
            if (stepMotor->moveStep(this->speed, true, this->monochromator->getFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test fine right movement is Ok.";

                this->additionalSteps -= this->monochromator->getFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test fine right movement failed.";

                return false;
            }
        }
        else
        {
            if (stepMotor->moveStep(this->speed, true, this->monochromator->getUltraFineStep()))
            {
                qDebug() << "MDG 23 by Geo simple stress test ultra fine right movement is Ok.";

                this->additionalSteps -= this->monochromator->getUltraFineStep();

                return true;
            }
            else
            {
                qDebug() << "MDG 23 by Geo simple stress test utra fine right movement failed.";

                return false;
            }
        }
    }
    return false;
}

