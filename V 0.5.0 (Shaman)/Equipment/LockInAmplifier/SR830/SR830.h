#ifndef SR830_H
#define SR830_H

#include <QObject>
#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

#include <vector>

#include "../../CommonFunctionsForSRS/SRS.h"

class SR830 : public QObject
{
    Q_OBJECT

private:
    const double minInternalFrequency = 1e-3;
    const double maxInternalFrequency = 1.02e5;

    const double minSineOutputAmplitude = 0.004;
    const double maxSineOutputAmplitude = 5;

    const int minHarmonic = 1;
    const int maxHarmonic = 19999;

    SRS* srs;
    QSerialPort* serial;

    QStringList baudrateList;
    QStringList referenceSourceList;
    QStringList referenceTriggerModeList;
    QStringList signalInputList;
    QStringList voltageInputModeList;
    QStringList voltageInputCouplingList;
    QStringList voltageInputShieldsList;
    QStringList notchFilterList;
    QStringList sensivityList;
    QStringList reserveModeList;
    QStringList timeConstantList;
    QStringList filterList;
    QStringList sampleRateList;
    QStringList bufferModeList;
    QStringList outputNumberList;

public:
    explicit SR830(QObject *parent = 0);
    explicit SR830(const QString &portName, const int &new_baudrate);
    ~SR830();

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

    bool isValidInternalFrequency(const double &new_frequency) const;
    double getMinInternalFrequency() const;
    double getMaxInternalFrequency() const;
    bool setInternalFrequency(const double &new_frequency) const;
    double getFrequency() const;

    int getMinHarmonic() const;
    int getMaxHarmonic() const;
    bool isValidHarmonic(const int &i) const;
    bool setHarmonic(const int &i) const;
    int getHarmonic() const;

    bool isValidSineOutAmplitude(const double &new_sineAmplitude) const;
    double getMinSineOutAmplitude() const;
    double getMaxSineOutAmplitude() const;
    bool setSineOutAmplitude(const double &new_sineAmplitude) const;
    double getSineOutAmplitude() const;

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

    void initSignalInputList();
    QStringList getSignalInputList() const;
    int signalInputNumberFromString(const QString &signalInput_string) const;
    QString signalInputStringFromNumber(const int &signalInput_number) const;
    bool setSignalInput(const int &new_signalInput) const;
    bool setSignalInput(const QString &new_signalInput) const;
    QString getSignalInput() const;

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

    void initInputLineNotchFilterList();
    QStringList getInputLineNotchFilterList() const;
    int inputLineNotchFilterNumberFromString(const QString &inputLineNotchFilter_string) const;
    QString inputLineNotchFilterStringFromNumber(const int &inputLineNotchFilter_number) const;
    bool setInputLineNotchFilter(const int &new_inputLineNotchFilter) const;
    bool setInputLineNotchFilter(const QString &new_inputLineNotchFilter) const;
    QString getInputLineNotchFilter() const;

    void initSensivityList();
    QStringList getSensivityList() const;
    int sensivityNumberFromString(const QString &sensivity_string) const;
    QString sensivityStringFromNumber(const int &sensivity_number) const;
    bool setSensivity(const int &new_sensivity) const;
    bool setSensivity(const QString &new_sensivity) const;
    QString getSensivity() const;

    void initReserveModeList();
    QStringList getReserveModeList() const;
    int reserveModeNumberFromString(const QString &reserveMode_string) const;
    QString reserveModeStringFromNumber(const int &reserveMode_number) const;
    bool setReserveMode(const int &new_reserveMode) const;
    bool setReserveMode(const QString &new_reserveMode) const;
    QString getReserveMode() const;

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

    bool enableRS232outputInterface() const;

    void initSampleRateList();
    QStringList getSampleRateList() const;
    int sampleRateNumberFromString(const QString &sampleRate_string) const;
    QString sampleRateStringFromNumber(const int &sampleRate_number) const;
    bool setSampleRate(const int &new_sampleRate) const;
    bool setSampleRate(const QString &new_sampleRate) const;
    QString getSampleRate() const;

    void initBufferModeList();
    QStringList getBufferModeList() const;
    int bufferModeNumberFromString(const QString &bufferMode_string) const;
    QString bufferModeStringFromNumber(const int &bufferMode_number) const;
    bool setBufferMode(const int &new_bufferMode) const;
    bool setBufferMode(const QString &new_bufferMode) const;
    QString getBufferMode() const;

    bool startBuffer() const;
    bool pauseBuffer() const;
    bool stopBuffer() const;

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
    bool getABCDE(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C, const int &Dpos, double &D, const int &Epos, double &E) const;
    bool getABCDEF(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C, const int &Dpos, double &D, const int &Epos, double &E, const int &Fpos, double &F) const;
    bool getXY(double &X, double &Y) const;
    bool getRTheta(double &R, double &Theta) const;
    bool getRThetaF(double &R, double &Theta, double &F) const;
    bool getXYRThetaF(double &X, double&Y, double &R, double &Theta, double &F) const;

    bool setDisplayData(const int &channel, const int &data) const;
    int getBufferSize() const;

    double getPointFromBuffer(const int &channel, const int &number) const;
    std::vector < double > getChannelFromBuffer(const int &channel) const;
    int getBuffer(std::vector < double > &ch1, std::vector < double > &ch2) const;


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

#endif // SR830_H
