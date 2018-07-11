#ifndef DS345_H
#define DS345_H

#include <QObject>
#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

#include "../../CommonFunctionsForSRS/SRS.h"

class DS345 : public QObject
{
    Q_OBJECT

private:
    const double maxAmplitudeSineVpp   = 10;
    const double minAmplitudeSineVpp   = 0.01;
    const double stepAmplitudeSineVpp  = 0.01;
    const int decimalsAmplitudeSineVpp = 3;

    const double maxAmplitudeSineVrms   = 3.54;
    const double minAmplitudeSineVrms   = 0.00354;
    const double stepAmplitudeSineVrms  = 0.01;
    const int decimalsAmplitudeSineVrms = 3;

    const double maxAmplitudeSinedBm   = +23.98;
    const double minAmplitudeSinedBm   = -36.02;
    const double stepAmplitudeSinedBm  = 0.01;
    const int decimalsAmplitudeSinedBm = 3;

    const double maxAmplitudeSquareVpp   = 10;
    const double minAmplitudeSquareVpp   = 0.01;
    const double stepAmplitudeSquareVpp  = 0.01;
    const int decimalsAmplitudeSquareVpp = 3;

    const double maxAmplitudeSquareVrms   = 5;
    const double minAmplitudeSquareVrms   = 0.005;
    const double stepAmplitudeSquareVrms  = 0.01;
    const int decimalsAmplitudeSquareVrms = 3;

    const double maxAmplitudeSquaredBm   = +26.99;
    const double minAmplitudeSquaredBm   = -33.0;
    const double stepAmplitudeSquaredBm  = 0.01;
    const int decimalsAmplitudeSquaredBm = 3;

    const double maxAmplitudeTriangleVpp   = 10;
    const double minAmplitudeTriangleVpp   = 0.01;
    const double stepAmplitudeTriangleVpp  = 0.01;
    const int decimalsAmplitudeTriangleVpp = 3;

    const double maxAmplitudeTriangleVrms   = 2.89;
    const double minAmplitudeTriangleVrms   = 0.00289;
    const double stepAmplitudeTriangleVrms  = 0.01;
    const int decimalsAmplitudeTriangleVrms = 3;

    const double maxAmplitudeTriangledBm   = +22.22;
    const double minAmplitudeTriangledBm   = -37.78;
    const double stepAmplitudeTriangledBm  = 0.01;
    const int decimalsAmplitudeTriangledBm = 3;

    const double maxAmplitudeRampVpp   = 10;
    const double minAmplitudeRampVpp   = 0.01;
    const double stepAmplitudeRampVpp  = 0.01;
    const int decimalsAmplitudeRampVpp = 3;

    const double maxAmplitudeRampVrms   = 2.89;
    const double minAmplitudeRampVrms   = 0.00289;
    const double stepAmplitudeRampVrms  = 0.01;
    const int decimalsAmplitudeRampVrms = 3;

    const double maxAmplitudeRampdBm   = +22.22;
    const double minAmplitudeRampdBm   = -37.78;
    const double stepAmplitudeRampdBm  = 0.01;
    const int decimalsAmplitudeRampdBm = 3;

    const double maxAmplitudeNoiseVpp   = 10;
    const double minAmplitudeNoiseVpp   = 0.01;
    const double stepAmplitudeNoiseVpp  = 0.01;
    const int decimalsAmplitudeNoiseVpp = 3;

    const double maxAmplitudeNoiseVrms   = 2.09;
    const double minAmplitudeNoiseVrms   = 0.00209;
    const double stepAmplitudeNoiseVrms  = 0.01;
    const int decimalsAmplitudeNoiseVrms = 3;

    const double maxAmplitudeNoisedBm   = +19.41;
    const double minAmplitudeNoisedBm   = -40; // -40.59
    const double stepAmplitudeNoisedBm  = 0.01;
    const int decimalsAmplitudeNoisedBm = 3;

    const double maxAmplitudeArbitraryVpp   = 10;
    const double minAmplitudeArbitraryVpp   = 0.01;
    const double stepAmplitudeArbitraryVpp  = 0.01;
    const int decimalsAmplitudeArbitraryVpp = 3;

    const double maxOffsetV   = 5.00;
    const double minOffsetV   = 0.00;
    const double stepOffsetV  = 0.1;
    const int decimalsOffsetV = 2;

    const double   maxFrequencySine = 30.2E6;
    const double   minFrequencySine = 1E-6;
    const double  stepFrequencySine = 1E-6;
    const int decimalsFrequencySine = 6;

    const double   maxFrequencySquare = 30.2E6;
    const double   minFrequencySquare = 1E-6;
    const double  stepFrequencySquare = 1E-6;
    const int decimalsFrequencySquare = 6;

    const double   maxFrequencyRamp = 10E3;
    const double   minFrequencyRamp = 1E-6;
    const double  stepFrequencyRamp = 1E-6;
    const int decimalsFrequencyRamp = 6;

    const double   maxFrequencyTriangle = 10E3;
    const double   minFrequencyTriangle = 1E-6;
    const double  stepFrequencyTriangle = 1E-6;
    const int decimalsFrequencyTriangle = 6;

    const double   maxFrequencyNoise = 10E6;
    const double   minFrequencyNoise = 10E6;
    const double  stepFrequencyNoise = 0;
    const int decimalsFrequencyNoise = 0;

    const double   maxFrequencyArbitrary = 10E6;
    const double   minFrequencyArbitrary = 10E6;
    const double  stepFrequencyArbitrary = 0;
    const int decimalsFrequencyArbitrary = 0;

    const int minAMDepth = -100;
    const int maxAMDepth = +100;

    const double   maxModulationRate = 10E3;
    const double   minModulationRate = 1E-3;
    const double  stepModulationRate = 1;
    const int decimalsModulationRate = 3;

    SRS* srs;
    QSerialPort* serial;

    QStringList baudrateList;
    QStringList amplitudeTypeList;
    QStringList functionList;
    QStringList modulationFunctionList;
    QStringList modulationTypeList;

public:
    explicit DS345();
    explicit DS345(const QString &portName, const int &baudrate, const int &stopBit = 1, const int &parity = 0);
    ~DS345();

    void init();

    void setSerial(QSerialPort* new_serial);
    QSerialPort* getSerial() const;

    bool setConnection(const QString &portName, const int &baudrate, const int &stopBit = 1, const int &parity = 0);
    void disconnect() const;

    void initBaudrateList();
    QStringList getBaudrateList() const;
    bool isValidBaudrate(const int &baudrate) const;
    bool changeBaudrate(const int &baudrate);

    void setWriteTimeout(const int &new_writeTimeout);
    int getWriteTimeout() const;

    void setReadTimeout(const int &new_readTimeout);
    int getReadTimeout() const;

    void setReadWaitTimeout(const int &new_readWaitTimeout);
    int getReadWaitTimeout() const;

    void setAttemptsToConnect(const int &new_attemptsToConnect);
    int getAttemptsToConnect() const;

    bool isActive() const;

    bool send(const QString &command, QString &response, const bool &wait_for_response) const;
    bool sendCommand(const QString &command) const;
    bool sendQuery(const QString &command, QString &response) const;
    QString ask(const QString &command) const;

    bool getIDN(QString &idn) const;

    int numberFromString(const QStringList &list, const QString &string) const;
    QString stringFromNumber(const QStringList &list, const int &number) const;
    bool isValidString(const QStringList &list, const QString &string) const;
    bool isValidNumber(const QStringList &list, const int &number) const;

    bool test() const;

    void initAmplitudeTypeList();
    QStringList getAmplitudeTypeList() const;
    double getMinAmplitude(const QString &waveform, const QString &unit) const;
    double getMaxAmplitude(const QString &waveform, const QString &unit) const;
    double getStepAmplitude(const QString &waveform, const QString &unit) const;
    double getDecimalsAmplitude(const QString &waveform, const QString &unit) const;
    bool isValidAmplitude(const double &amplitude, const QString &waveform, const QString &unit) const;
    bool isValidAmplitudeType(const QString &unit) const;
    bool setAmplitude(const double &amplitude, const QString &unit) const;
    double getAmplitude(const QString &unit) const;

    double getMinOffset() const;
    double getMaxOffset() const;
    double getStepOffset() const;
    double getDecimalsOffset() const;
    bool isValidOffset(const double &offset) const;
    bool setOffset(const double &offset) const;
    double getOffset() const;

    double getMinFrequency(const QString &waveform) const;
    double getMaxFrequency(const QString &waveform) const;
    double getStepFrequency(const QString &waveform) const;
    double getDecimalsFrequency(const QString &waveform) const;
    bool isValidFrequency(const double &frequency, const QString &waveform) const;
    bool setFrequency(const double &frequency) const;
    double getFrequency() const;

    void initFunctionList();
    QStringList getFunctionList() const;
    int functionNumberFromString(const QString &function_string) const;
    QString functionStringFromNumber(const int &function_number) const;
    bool setFunction(const int &function) const;
    bool setFunction(const QString &function) const;
    QString getFunction() const;

    bool setInverse(const bool &inverse) const;
    bool getInverse() const;

    int getMinAMDepth() const;
    int getMaxAMDepth() const;
    bool isValidAMDepth(const int &persentage) const;
    bool setAMdepth(const int &persentage) const;
    int getAMdepth() const;

    double getMinFMSpan(const QString &waveform) const;
    double getMaxFMSpan(const QString &waveform) const;
    bool isValidFMSpan(const int &span, const QString &waveform) const;
    bool setFMSpan(const double &span) const;
    double getFMSpan() const;

    void initModulationFunctionList();
    QStringList getModulationFunctionList() const;
    int modulationFunctionNumberFromString(const QString &function_string) const;
    QString modulationFunctionStringFromNumber(const int &function_number) const;
    bool setModulationFunction(const int &function) const;
    bool setModulationFunction(const QString &function) const;
    QString getModulationFunction() const;

    bool setModulationEnabled(const bool &enable) const;
    bool getModulationEnabled() const;

    void initModulationTypeList();
    QStringList getModulationTypeList() const;
    int modulationTypeNumberFromString(const QString &type_string) const;
    QString modulationTypeStringFromNumber(const int &type_number) const;
    bool setModulationType(const int &type) const;
    bool setModulationType(const QString &type) const;
    QString getModulationType() const;

    double getMinModulationRate() const;
    double getMaxModulationRate() const;
    double getStepModulationRate() const;
    double getDecimalsModulationRate() const;
    bool isValidModulationRate(const double &rate) const;
    bool setModulationRate(const double &rate) const;
    double getModulationRate() const;

    double getMinModulationSpan(const QString &waveform) const;
    double getMaxModulationSpan(const QString &waveform) const;
    double getStepModulationSpan(const QString &waveform) const;
    double getDecimalsModulationSpan(const QString &waveform) const;
    bool isValidModulationSpan(const double &span, const QString &waveform) const;
    bool setModulationSpan(const double &span) const;
    double getModulationSpan() const;

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

#endif // DS345_H
