#include "MDG_23_by_Geo.h"

MDG_23_by_Geo::MDG_23_by_Geo()
{
    qDebug() << "MDG 23 constructor was called without grating.";

    this->stepMotor = nullptr;

    qDebug() << "MDG 23 constructor is closing.";
}

MDG_23_by_Geo::MDG_23_by_Geo(const int &new_grating)
{
    qDebug() << "MDG 23 constructor was called with grating.";

    this->stepMotor = nullptr;
    this->setGrating(new_grating);

    qDebug() << "MDG 23 constructor is closing.";
}

MDG_23_by_Geo::MDG_23_by_Geo(SMSD_4_2 *new_stepMotor)
{
    qDebug() << "MDG 23 constructor was called with step motor.";

    this->stepMotor = nullptr;
    this->setStepMotorController(new_stepMotor);

    qDebug() << "MDG 23 constructor is closing.";
}

MDG_23_by_Geo::MDG_23_by_Geo(SMSD_4_2 *new_stepMotor, const int &new_grating)
{
    qDebug() << "MDG 23 constructor was called with step motor and grating.";

    this->setGrating(new_grating);
    this->setStepMotorController(new_stepMotor);

    qDebug() << "MDG 23 constructor is closing.";

}

MDG_23_by_Geo::~MDG_23_by_Geo()
{
    qDebug() << "MDG 23 destructor was called.";

    if (this->stepMotor != nullptr) {
        qDebug() << "Step motor destructor called and it's pointer is" << this->stepMotor;

        this->stepMotor->~SMSD_4_2();
//        delete this->stepMotor;
    }

    qDebug() << "MDG 23 destructor is closing.";
}

SMSD_4_2* MDG_23_by_Geo::getStepMotorController() const
{
    qDebug() << "MDG 23 step motor controller pointer was asked for, and is " << this->stepMotor;

    return this->stepMotor;
}

void MDG_23_by_Geo::setStepMotorController(SMSD_4_2 *stepMotorController)
{
    qDebug() << "MDG 23 step motor controller pointer was set to " << stepMotorController;

    this->stepMotor = stepMotorController;
}

double MDG_23_by_Geo::getMinDisplay() const
{
    qDebug() << "MDG 23 by Geo minimum display number is askef for and is " << this->minDisplay;

    return this->minDisplay;
}

double MDG_23_by_Geo::getMaxDisplay() const
{
    qDebug() << "MDG 23 by Geo maximum display number is askef for and is " << this->maxDisplay;

    return this->maxDisplay;
}

double  MDG_23_by_Geo::getDisplaySteps() const
{
    qDebug() << "MDG 23 by Geo is asked for display step and is " << this->displayStep;

    return this->displayStep;
}

int  MDG_23_by_Geo::getDisplayDecimals() const
{
    qDebug() << "MDG 23 by Geo is asked for display decimals and is " << this->displayDesimals;

    return this->displayDesimals;
}

int MDG_23_by_Geo::getCoarseStep() const
{
    qDebug() << "MDG 23 by Geo is asked for coarse step and is " << this->coarseStep;

    return this->coarseStep;
}

int MDG_23_by_Geo::getFineStep() const
{
    qDebug() << "MDG 23 by Geo is asked for fine step and is " << this->fineStep;

    return this->fineStep;
}

int MDG_23_by_Geo::getExtraFineStep() const
{
    qDebug() << "MDG 23 by Geo is asked for extra fine step and is " << this->extraFineStep;

    return this->extraFineStep;
}

int MDG_23_by_Geo::getUltraFineStep() const
{
    qDebug() << "MDG 23 by Geo is asked for ultra fine step and is " << this->ultraFineStep;

    return this->ultraFineStep;
}

bool MDG_23_by_Geo::isActive() const
{
    qDebug() << "MDG 23 activeness was checked and is "  << this->stepMotor->isActive();

    return this->stepMotor->isActive();
}

bool MDG_23_by_Geo::checkDisplayNumber(const double &displayData) const
{
    qDebug() << "MDG 23 checking display number. Candidate is " << displayData;

    if (displayData > this->maxDisplay) {
        qDebug() << "MDG 23 display data is too big.";

        return false;
    }
    if (displayData < this->minDisplay) {
        qDebug() << "MDG 23 display data is too small.";

        return false;
    }
//    if (_round(displayData * 100) % 10 != 0) {
//        qDebug() << "MDG 23 display data has too many numbers after decimal dot.";
//
//        return false;
//    }

    qDebug() << "MDG 23 display data is OK.";

    return true;
}

bool MDG_23_by_Geo::setCurrentDisplayNumber(const double &displayNumber)
{
    qDebug() << "MDG 23 setting current display data";

    if (this->checkDisplayNumber(displayNumber)) {
        this->currentDiaplayNumber = displayNumber;

        qDebug() << "MDG 23 current display number was succesfully set to " << this->currentDiaplayNumber;

        return true;
    } else {
        qDebug() << "MDG 23 wrong display data is trying to be set";

        return false;
    }
}

double MDG_23_by_Geo::getCurrentDisplayNumber() const
{
    qDebug() << "MDG 23 by Geo current display number was asked and is " << this->currentDiaplayNumber;

    return this->currentDiaplayNumber;
}

bool MDG_23_by_Geo::setGrating(const int &new_grating)
{
    qDebug() << "MDG 23 setting grating";

    if (new_grating == 600 || new_grating == 1200) {
        this->grating = new_grating;

        qDebug() << "MDG 23 grating was set to " << this->grating;

        return true;
    } else {
        qDebug() << "MDG 23 grating was not set to " << new_grating << ", no such grating";

        return true;
    }
}

int MDG_23_by_Geo::getGrating() const
{
    qDebug() << "MDG 23 grating was saked for and is " << this->grating;

    return this->grating;
}

bool MDG_23_by_Geo::setStepDenominator(const int &new_stepdenominator)
{
    qDebug() << "MDG 23 setting step denominator";

    return this->stepMotor->setStepDenominator(new_stepdenominator);
}

int MDG_23_by_Geo::getStepDenominator() const
{
    qDebug() << "MDG 23 step denominator was saked for and is " << this->stepMotor->getStepDenominator();

    return this->stepMotor->getStepDenominator();
}

int MDG_23_by_Geo::getMaxSpeedWithCurrentStepDenominator() const
{
    qDebug() << "MDG 23 max speed was asked for";

    if (this->stepMotor->getStepDenominator() == 16)
        return this->maxSpeed16;
    if (this->stepMotor->getStepDenominator() == 4)
        return this->maxSpeed4;
    if (this->stepMotor->getStepDenominator() == 2)
        return this->maxSpeed2;
    if (this->stepMotor->getStepDenominator() == 1)
        return this->maxSpeed1;
    return 0;
}

int MDG_23_by_Geo::signByRotation(const bool &is_right) const
{
	qDebug() << "MDG 23 changes in display sight from rotation direction called";
	
	if (is_right)
		return -1;
	else
		return 1;
}

bool MDG_23_by_Geo::isRightRotationBySign(const double &d) const
{
    qDebug() << "MDG 23 rotation direction from changes in display sight called";

    if (d < 0)
        return true;
    else
        return false;
}

bool MDG_23_by_Geo::isRightRotationBySign(const int &d) const
{
    return isRightRotationBySign(1.0*d);
}

bool MDG_23_by_Geo::move(const bool &right, const int &steps)
{
    qDebug() << "MDG 23 movement was called";
	
    long long temp_steps = steps;
    int speed = getMaxSpeedWithCurrentStepDenominator();
    int denominator = this->stepMotor->getStepDenominator();
    int sign = signByRotation(right);

    while (temp_steps > 0) {
        int current_step = temp_steps > this->iterationSteps ? this->iterationSteps : temp_steps;
        if (!this->stepMotor->moveStep(speed, right, current_step))
            return false;
        QTest::qWait(_round(1000.0*(current_step/speed + 0.5)));
        this->currentDiaplayNumber += sign*current_step/(1.0*this->steps_in_nm*denominator);
        temp_steps -= current_step;

        qDebug() << "MDG 23 by Geo emitting current display number changed";

        emit this->MDG_23_by_Geo_currentDisplayNumberChanged(this->currentDiaplayNumber);
	}
	return true;
}

bool MDG_23_by_Geo::move(const double &nm)
{
    double d_nm = nm - this->currentDiaplayNumber;

    qDebug() << "Need to move to " << d_nm << " nm";
    qDebug() << "Rotating to right: " << isRightRotationBySign(d_nm);
    qDebug() << "Steps in nm: " << this->steps_in_nm;
    qDebug() << "Current step denominator: " << this->getStepDenominator();
    qDebug() << "Steps: " << _round(_abs(d_nm)*this->steps_in_nm*this->stepMotor->getStepDenominator());

    return move(isRightRotationBySign(d_nm), _round(_abs(d_nm)*this->steps_in_nm*this->getStepDenominator()));
}

double MDG_23_by_Geo::displayNumberToWavelength(const double &temp_displayNumber, const int &temp_grating) const
{
    if (temp_grating == 1200)
        return temp_displayNumber;
    if (temp_grating == 600)
        return 2*temp_displayNumber;
    return -1;
}

double MDG_23_by_Geo::displayNumberToWavelength(const double &temp_displayNumber) const
{
    return displayNumberToWavelength(temp_displayNumber, this->grating);
}

double MDG_23_by_Geo::displayNumberToWavelength() const
{
    return displayNumberToWavelength(this->currentDiaplayNumber);
}
