#ifndef MDG_23_BY_GEO_H
#define MDG_23_BY_GEO_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTest>
#include <QTime>
#include <QDebug>

#include "../../StepMotorController/SMSD_4_2/SMSD_4_2.h"
#include "../../../Functions/AllFunctions.h"

class MDG_23_by_Geo : public QObject {
    Q_OBJECT

private:
    SMSD_4_2 *stepMotor;

    double currentDiaplayNumber;
    double minDisplay = 1*192.000;
    double maxDisplay = 2*1020.000;
    double displayStep = 0.005;
    double displayDesimals = 3;

    int coarseStep    = 1000;
    int fineStep      = 100;
    int extraFineStep = 10;
    int ultraFineStep = 1;

    int maxSpeed1 = 1000;
    int maxSpeed2 = 1000;
    int maxSpeed4 = 1000;
    int maxSpeed16 = 10000;

    int grating;
    double steps_in_nm = 1000;
    int iterationSteps = 20000;

public:
    explicit MDG_23_by_Geo();
    explicit MDG_23_by_Geo(const int &new_grating);
    explicit MDG_23_by_Geo(SMSD_4_2 *new_stepMotor);
    explicit MDG_23_by_Geo(SMSD_4_2 *new_stepMotor, const int &new_grating);
    ~MDG_23_by_Geo();

    SMSD_4_2* getStepMotorController() const;
    void setStepMotorController(SMSD_4_2 *stepMotorController);

    double getMinDisplay() const;
    double getMaxDisplay() const;
    double getDisplaySteps() const;
    int getDisplayDecimals() const;

    int getCoarseStep() const;
    int getFineStep() const;
    int getExtraFineStep() const;
    int getUltraFineStep() const;

    bool isActive() const;

    bool checkDisplayNumber(const double &displayData) const;
    bool setCurrentDisplayNumber(const double &displayNumber);
    double getCurrentDisplayNumber() const;

    bool setGrating(const int &new_grating);
    int getGrating() const;

    bool setStepDenominator(const int &new_stepdenominator);
    int getStepDenominator() const;

    int getMaxSpeedWithCurrentStepDenominator() const;

    int signByRotation(const bool &is_right) const;
    bool isRightRotationBySign(const double &d) const;
    bool isRightRotationBySign(const int &d) const;

    bool move(const bool &right, const int &steps);
    bool move(const double &nm);

    double displayNumberToWavelength(const double &temp_displayNumber, const int &temp_grating) const;
    double displayNumberToWavelength(const double &temp_displayNumber) const;
    double displayNumberToWavelength() const;

signals:
    void MDG_23_by_Geo_currentDisplayNumberChanged(const double &new_displaynumber) const;
};

#endif // MDG_23_BY_GEO_H
