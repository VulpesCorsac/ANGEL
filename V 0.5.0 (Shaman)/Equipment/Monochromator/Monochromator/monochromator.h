#ifndef MONOCHROMATOR_H
#define MONOCHROMATOR_H

#include "../ANGEL/Equipment/Monochromator/MDG_23_by_Geo/MDG_23_by_Geo.h"

class Monochromator : public QObject {
    Q_OBJECT

private:
    QString monochromator_type = "MDG_23_by_Geo";
    MDG_23_by_Geo* monochromator_MDG_23_by_Geo;

public:
    explicit Monochromator();
    ~Monochromator();

    bool setMonochromatorType(const QString new_monochromator_type);
    QString getMonochromatorType() const;

    MDG_23_by_Geo* getMDG23ByGeoMonochromator() const;
    void setMDG23ByGeoMonochromator(MDG_23_by_Geo* new_monochromator_MDG_23_by_Geo);

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

    void connectSS() const;

    bool move(const bool &right, const int &steps);
    bool move(const double &nm);

    double displayNumberToWavelength(const double &temp_displayNumber, const int &temp_grating) const;
    double displayNumberToWavelength(const double &temp_displayNumber) const;
    double displayNumberToWavelength() const;

private slots:
    void getCurrentDisplayNumberChanged(const double &new_wavelength) const;

signals:
    void Monochromator_currentDisplayNumberChanged(const double &new_displaynumber) const;
};

#endif // MONOCHROMATOR_H
