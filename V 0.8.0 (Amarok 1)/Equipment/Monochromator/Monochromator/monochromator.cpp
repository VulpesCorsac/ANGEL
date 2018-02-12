#include "Monochromator.h"

Monochromator::Monochromator()
{
    this->monochromator_MDG_23_by_Geo = nullptr;

    this->monochromator_MDG_23_by_Geo = new MDG_23_by_Geo();
    SMSD_4_2* stepMotor = new SMSD_4_2();
    this->monochromator_MDG_23_by_Geo->setStepMotorController(stepMotor);
}

Monochromator::~Monochromator()
{
    if (this->monochromator_MDG_23_by_Geo != nullptr) {
        this->monochromator_MDG_23_by_Geo->~MDG_23_by_Geo();
    }
}

bool Monochromator::setMonochromatorType(const QString new_monochromator_type)
{
    if (new_monochromator_type == "MDG_23_by_Geo") {
        this->monochromator_type = new_monochromator_type;
        if (this->monochromator_MDG_23_by_Geo == nullptr)
            this->monochromator_MDG_23_by_Geo = new MDG_23_by_Geo();
        return true;
    }
    return false;
}

QString Monochromator::getMonochromatorType() const
{
    return this->monochromator_type;
}

MDG_23_by_Geo* Monochromator::getMDG23ByGeoMonochromator() const
{
    return this->monochromator_MDG_23_by_Geo;
}

void Monochromator::setMDG23ByGeoMonochromator(MDG_23_by_Geo *new_monochromator_MDG_23_by_Geo)
{
    this->monochromator_MDG_23_by_Geo = new_monochromator_MDG_23_by_Geo;

    this->setMonochromatorType("MDG_23_by_Geo");
}

SMSD_4_2* Monochromator::getMDG23ByGeoStepMotorController() const
{
    return this->monochromator_MDG_23_by_Geo->getStepMotorController();
}

void Monochromator::setMDG23ByGeoStepMotorController(SMSD_4_2 *stepMotorController)
{
    return this->monochromator_MDG_23_by_Geo->setStepMotorController(stepMotorController);
}

bool Monochromator::setMDG23ByGeoConnection(const QString &portName, int new_waitTimeout)
{
    return this->monochromator_MDG_23_by_Geo->setConnection(portName, new_waitTimeout);
}

bool Monochromator::disconnectMDG23ByGeo()
{
    return this->monochromator_MDG_23_by_Geo->disconnect();
}

bool Monochromator::setMDG23ByGeoStepDenominator(const int &new_stepDenominator)
{
    return this->monochromator_MDG_23_by_Geo->setStepDenominator(new_stepDenominator);
}

int  Monochromator::getMDG23ByGeoStepDenominator() const
{
    return this->monochromator_MDG_23_by_Geo->getStepDenominator();
}

double Monochromator::getMinDisplay() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getMinDisplay();
    return -1;
}

double Monochromator::getMaxDisplay() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getMaxDisplay();
    return -1;
}

double Monochromator::getDisplaySteps() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getDisplaySteps();
    return -1;
}

int Monochromator::getDisplayDecimals() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getDisplayDecimals();
    return -1;
}

int Monochromator::getCoarseStep() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getCoarseStep();
    return -1;
}

int Monochromator::getFineStep() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getFineStep();
    return -1;
}

int Monochromator::getExtraFineStep() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getExtraFineStep();
    return -1;
}

int Monochromator::getUltraFineStep() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getUltraFineStep();
    return -1;
}

bool Monochromator::isActive() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->isActive();
    return false;
}

bool Monochromator::checkDisplayNumber(const double &displayData) const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->checkDisplayNumber(displayData);
    return false;
}

bool Monochromator::setCurrentDisplayNumber(const double &displayNumber)
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->setCurrentDisplayNumber(displayNumber);
    return false;
}

double Monochromator::getCurrentDisplayNumber() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getCurrentDisplayNumber();
    return -1;

}

bool Monochromator::setGrating(const int &new_grating)
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->setGrating(new_grating);
    return false;
}

int Monochromator::getGrating() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getGrating();
    return 0;
}

bool Monochromator::setStepDenominator(const int &new_stepdenominator)
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->setStepDenominator(new_stepdenominator);
    return false;
}

int Monochromator::getStepDenominator() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->getStepDenominator();
    return 0;
}

int Monochromator::signByRotation(const bool &is_right) const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->signByRotation(is_right);
    return 0;
}

bool Monochromator::isRightRotationBySign(const double &d) const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->isRightRotationBySign(d);
    return false;
}

bool Monochromator::isRightRotationBySign(const int &d) const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->isRightRotationBySign(d);
    return false;
}

void Monochromator::connectSS() const
{
    connect(this->monochromator_MDG_23_by_Geo, SIGNAL(MDG_23_by_Geo_currentDisplayNumberChanged(double)),
            this, SLOT(getCurrentDisplayNumberChanged(double)), Qt::DirectConnection);
}

bool Monochromator::move(const bool &right, const int &steps)
{
    connectSS();

    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->move(right, steps);
    return false;
}

bool Monochromator::move(const double &nm)
{
    connectSS();

    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->move(nm);
    return false;
}

double Monochromator::displayNumberToWavelength(const double &temp_displayNumber, const int &temp_grating) const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->displayNumberToWavelength(temp_displayNumber, temp_grating);
    return -1;
}

double Monochromator::displayNumberToWavelength(const double &temp_displayNumber) const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->displayNumberToWavelength(temp_displayNumber);
    return -1;
}

double Monochromator::displayNumberToWavelength() const
{
    if (this->monochromator_type == "MDG_23_by_Geo")
        return this->monochromator_MDG_23_by_Geo->displayNumberToWavelength();
    return -1;
}

void Monochromator::getCurrentDisplayNumberChanged(const double &new_displaynumber) const
{
    emit this->Monochromator_currentDisplayNumberChanged(new_displaynumber);
}
