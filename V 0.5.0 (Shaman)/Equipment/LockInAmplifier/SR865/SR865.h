#ifndef SR865_H
#define SR865_H

#include <QObject>
#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

#include "../../CommonFunctionsForSRS/SRS.h"

class SR865 : public QObject
{
    Q_OBJECT

private:
    const double minInternalFrequency = 1e-3;
    const double maxInternalFrequency = 2.5e6;

    const double minSineOutputAmplitude = 1e-9;
    const double maxSineOutputAmplitude = 2;

    const double minSineOffset = -5;
    const double maxSineOffset = 5;

    const int minHarmonic = 1;
    const int maxHarmonic = 99;

    const int minHarmonicDual = 1;
    const int maxHarmonicDual = 99;

    SRS* srs;
    QSerialPort* serial;

    QStringList frequencyPostfixList;
    QStringList voltagePostfixList;

    QStringList baudrateList;
    QStringList referenceSourceList;
    QStringList referenceTriggerModeList;
    QStringList referenceTriggerOutputZList;
    QStringList signalInputList;
    QStringList voltageInputModeList;
    QStringList voltageInputCouplingList;
    QStringList voltageInputShieldsList;
    QStringList voltageInputRangeList;
    QStringList currrentInputGainList;
    QStringList sensivityList;
    QStringList timeConstantList;
    QStringList filterList;
    QStringList outputNumberListLongName;
    QStringList outputNumberListShortName;

public:
    explicit SR865(QObject *parent = 0);
    explicit SR865(const QString &portName, const int &new_baudrate);
    ~SR865();

    void init();

    void setSerial(QSerialPort* new_serial);
    QSerialPort* getSerial() const;

    bool setConnection(const QString &portName, const int &new_baudrate);
    void disconnect() const;

    void initBaudrateList();
    QStringList getBaudrateList() const;
    bool isValidBaudrate(const int &new_baudrate) const;
    bool changeBaudrate(const int &new_baudrate);

    bool isActive() const;

    bool send(const QString &command, QString &response, const bool &wait_for_response) const;
    bool sendCommand(const QString &command) const;
    bool sendQuery(const QString &command, QString &response) const;
    QString ask(const QString &command) const;

    bool getIDN(QString &idn) const;

    void initFrequencyPostfixList();
    QStringList getFrequencyPostfixList() const;

    void initVoltagePostfixList();
    QStringList getVoltagePostfixList() const;

    bool isValidInternalFrequency(const double &new_frequency, const QString &postfix = "") const;
    double getMinInternalFrequency() const;
    double getMaxInternalFrequency() const;
    bool setInternalFrequency(const double &new_frequency, const QString &postfix = "") const;
    double getInternalFrequency() const;
    double getExternalFrequency() const;
    double getFrequency() const;
    double getFrequencyDetect() const;

    int getMinHarmonic() const;
    int getMaxHarmonic() const;
    bool isValidHarmonic(const int &i) const;
    bool setHarmonic(const int &i) const;
    int getHarmonic() const;

    int getMinHarmonicDual() const;
    int getMaxHarmonicDual() const;
    bool isValidHarmonicDual(const int &i) const;
    bool setHarmonicDual(const int &i) const;
    int getHarmonicDual() const;

    bool isValidSineOutAmplitude(const double &new_sineAmplitude, const QString &postfix = "") const;
    double getMinSineOutAmplitude() const;
    double getMaxSineOutAmplitude() const;
    bool setSineOutAmplitude(const double &new_sineAmplitude, const QString &postfix = "") const;
    double getSineOutAmplitude() const;

    bool isValidSineOffset(const double &new_offset, const QString &postfix = "") const;
    double getMinSineOffset() const;
    double getMaxSineOffset() const;
    bool setSineOffset(const double &new_offset, const QString &postfix = "") const;
    double getSineOffset() const;

    void initReferenceSourceList();
    QStringList getReferenceSourceList() const;
    int refSourceNumberFromString(const QString &ref_string) const;
    QString refSourceStringFromNumber(const int &ref_number) const;
    bool setReferenceSource(const int &new_source) const;
    bool setReferenceSource(const QString &new_source) const;
    QString getReferenceSource() const;

    void initReferenceTriggerModeList();
    QStringList getReferenceTriggerModeList() const;
    int refModeNumberFromString(const QString &mode_string) const;
    QString refModeStringFromNumber(const int &mode_number) const;
    bool setReferenceTriggerMode(const int &new_mode) const;
    bool setReferenceTriggerMode(const QString &new_mode) const;
    QString getReferenceTriggerMode() const;

    void initReferenceTriggerOutputZList();
    QStringList getReferenceTriggerOutputZList() const;
    int refOutputNumberFromString(const QString &outpZ_string) const;
    QString refOutputStringFromNumber(const int &outpZ_number) const;
    bool setReferenceTriggerOutputZ(const int &new_outpZ) const;
    bool setReferenceTriggerOutputZ(const QString &new_outpZ) const;
    QString getReferenceTriggerOutputZ() const;

    void initSignalInputList();
    QStringList getSignalInputList() const;
    int signalInputNumberFromString(const QString &signalInput_string) const;
    QString signalInputStringFromNumber(const int &signalInput_number) const;
    bool setSignalInput(const int &new_signalInput) const;
    bool setSignalInput(const QString &new_signalInput) const;
    QString getSignalInput() const;

    void initVoltageInputModeList();
    QStringList getVoltageInputModeList() const;
    int voltageInputModeNumberFromString(const QString &voltageInputMode_string) const;
    QString voltageInputModeStringFromNumber(const int &voltageInputMode_number) const;
    bool setVoltageInputMode(const int &new_voltageInputMode) const;
    bool setVoltageInputMode(const QString &new_voltageInputMode) const;
    QString getVoltageInputMode() const;

    void initVoltageInputCouplingList();
    QStringList getVoltageInputCouplingList() const;
    int voltageInputCouplingNumberFromString(const QString &voltageInputCoupling_string) const;
    QString voltageInputCouplingStringFromNumber(const int &voltageInputCoupling_number) const;
    bool setVoltageInputCoupling(const int &new_voltageInputCoupling) const;
    bool setVoltageInputCoupling(const QString &new_voltageInputCoupling) const;
    QString getVoltageInputCoupling() const;

    void initVoltageInputShieldsList();
    QStringList getVoltageInputShieldsList() const;
    int voltageInputShieldsNumberFromString(const QString &voltageInputShields_string) const;
    QString voltageInputShieldsStringFromNumber(const int &voltageInputShields_number) const;
    bool setVoltageInputShields(const int &new_voltageInputShields) const;
    bool setVoltageInputShields(const QString &new_voltageInputShields) const;
    QString getVoltageInputShields() const;

    void initVoltageInputRangeList();
    QStringList getVoltageInputRangeList() const;
    int voltageInputRangeNumberFromString(const QString &voltageInputRange_string) const;
    QString voltageInputRangeStringFromNumber(const int &voltageInputRange_number) const;
    bool setVoltageInputRange(const int &new_voltageInputRange) const;
    bool setVoltageInputRange(const QString &new_voltageInputRange) const;
    QString getVoltageInputRange() const;

    void initCurrentInputGainList();
    QStringList getCurrentInputGainList() const;
    int currentInputGainNumberFromString(const QString &currentInputGain_string) const;
    QString currentInputGainStringFromNumber(const int &currentInputGain_number) const;
    bool setCurrentInputGain(const int &new_currentInputGain) const;
    bool setCurrentInputGain(const QString &new_currentInputGain) const;
    QString getCurrentInputGain() const;

    int getSignalStrength() const;

    void initSensivityList();
    QStringList getSensivityList() const;
    int sensivityNumberFromString(const QString &sensivity_string) const;
    QString sensivityStringFromNumber(const int &sensivity_number) const;
    bool setSensivity(const int &new_sensivity) const;
    bool setSensivity(const QString &new_sensivity) const;
    QString getSensivity() const;

    void initTimeConstantList();
    QStringList getTimeConstantList() const;
    int timeConstantNumberFromString(const QString &timeConstant_string) const;
    QString timeConstantStringFromNumber(const int &timeConstant_number) const;
    bool setTimeConstant(const int &new_timeConstant) const;
    bool setTimeConstant(const QString &new_timeConstant) const;
    QString getTimeConstant() const;

    void initFilterList();
    QStringList getFilterList() const;
    int filterNumberFromString(const QString &filter_string) const;
    QString filterStringFromNumber(const int &filter_number) const;
    bool setFilter(const int &new_filter) const;
    bool setFilter(const QString &new_filter) const;
    QString getFilter() const;

    bool enableSychronousFilter(const bool &enable) const;
    bool isSychronousFilterEnabled() const;

    bool enableAdvancedFilterFilter(const bool &enable) const;
    bool isAdvancedFilterEnabled() const;

    void initOutputNumberList();
    QStringList getOutputNumberList() const;
    int outpNumberFromString(const QString &outp_string) const;
    QString outpStringFromNumber(const int &outp_number) const;
    double getOUTP(const int &i) const;
    double getOUTP(const QString &i) const;
    double getX() const;
    double getY() const;
    double getR() const;
    double getTheta() const;
    bool getAB(const int &Apos, double &A, const int &Bpos, double &B) const;
    bool getABC(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C) const;
    bool getXY(double &X, double &Y) const;
    bool getRTheta(double &R, double &Theta) const;
    bool getRThetaFint(double &R, double &Theta, double &F) const;
    bool getRThetaFext(double &R, double &Theta, double &F) const;
    bool getRThetaF(double &R, double &Theta, double &F) const;

public slots:
    void catchErrorSignal(const QString &s) const;
    void catchResponseSignal(const QString &s) const;
    void catchCommandSignal(const QString &s) const;
    void catchTimeoutSignal(const QString &s) const;

signals:
    void errorSignal(const QString &s) const;
    void responseSignal(const QString &s) const;
    void commandSignal(const QString &s) const;
    void timeoutSignal(const QString &s) const;
};

#endif // SR865_H
