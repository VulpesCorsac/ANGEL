#ifndef DS335_H
#define DS335_H

#include <QObject>
#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

#include "../../CommonFunctionsForSRS/SRS.h"

class DS335 : public QObject
{
    Q_OBJECT

private:
    const double maxAmplitude50OhmsSineVpp   = 10;
    const double minAmplitude50OhmsSineVpp   = 0.05;
    const double stepAmplitude50OhmsSineVpp  = 0.01;
    const int decimalsAmplitude50OhmsSineVpp = 3;

    const double maxAmplitude50OhmsSineVrms   = 3.54;
    const double minAmplitude50OhmsSineVrms   = 0.02;
    const double stepAmplitude50OhmsSineVrms  = 0.01;
    const int decimalsAmplitude50OhmsSineVrms = 3;

    const double maxAmplitudeHighZSineVpp   = 20;
    const double minAmplitudeHighZSineVpp   = 0.1;
    const double stepAmplitudeHighZSineVpp  = 0.01;
    const int decimalsAmplitudeHighZSineVpp = 3;

    const double maxAmplitudeHighZSineVrms   = 7.07;
    const double minAmplitudeHighZSineVrms   = 0.04;
    const double stepAmplitudeHighZSineVrms  = 0.01;
    const int decimalsAmplitudeHighZSineVrms = 3;

    const double maxAmplitude50OhmsSquareVpp   = 10;
    const double minAmplitude50OhmsSquareVpp   = 0.05;
    const double stepAmplitude50OhmsSquareVpp  = 0.01;
    const int decimalsAmplitude50OhmsSquareVpp = 3;

    const double maxAmplitude50OhmsSquareVrms   = 5;
    const double minAmplitude50OhmsSquareVrms   = 0.03;
    const double stepAmplitude50OhmsSquareVrms  = 0.01;
    const int decimalsAmplitude50OhmsSquareVrms = 3;

    const double maxAmplitudeHighZSquareVpp   = 20;
    const double minAmplitudeHighZSquareVpp   = 0.1;
    const double stepAmplitudeHighZSquareVpp  = 0.01;
    const int decimalsAmplitudeHighZSquareVpp = 3;

    const double maxAmplitudeHighZSquareVrms   = 10;
    const double minAmplitudeHighZSquareVrms   = 0.05;
    const double stepAmplitudeHighZSquareVrms  = 0.01;
    const int decimalsAmplitudeHighZSquareVrms = 3;

    const double maxAmplitude50OhmsTriangleVpp   = 10;
    const double minAmplitude50OhmsTriangleVpp   = 0.05;
    const double stepAmplitude50OhmsTriangleVpp  = 0.01;
    const int decimalsAmplitude50OhmsTriangleVpp = 3;

    const double maxAmplitude50OhmsTriangleVrms   = 2.89;
    const double minAmplitude50OhmsTriangleVrms   = 0.01;
    const double stepAmplitude50OhmsTriangleVrms  = 0.01;
    const int decimalsAmplitude50OhmsTriangleVrms = 3;

    const double maxAmplitudeHighZTriangleVpp   = 20;
    const double minAmplitudeHighZTriangleVpp   = 0.1;
    const double stepAmplitudeHighZTriangleVpp  = 0.01;
    const int decimalsAmplitudeHighZTriangleVpp = 3;

    const double maxAmplitudeHighZTriangleVrms   = 5.77;
    const double minAmplitudeHighZTriangleVrms   = 0.03;
    const double stepAmplitudeHighZTriangleVrms  = 0.01;
    const int decimalsAmplitudeHighZTriangleVrms = 3;

    const double maxAmplitude50OhmsRampVpp   = 10;
    const double minAmplitude50OhmsRampVpp   = 0.05;
    const double stepAmplitude50OhmsRampVpp  = 0.01;
    const int decimalsAmplitude50OhmsRampVpp = 3;

    const double maxAmplitude50OhmsRampVrms   = 2.89;
    const double minAmplitude50OhmsRampVrms   = 0.01;
    const double stepAmplitude50OhmsRampVrms  = 0.01;
    const int decimalsAmplitude50OhmsRampVrms = 3;

    const double maxAmplitudeHighZRampVpp   = 20;
    const double minAmplitudeHighZRampVpp   = 0.1;
    const double stepAmplitudeHighZRampVpp  = 0.01;
    const int decimalsAmplitudeHighZRampVpp = 3;

    const double maxAmplitudeHighZRampVrms   = 5.77;
    const double minAmplitudeHighZRampVrms   = 0.03;
    const double stepAmplitudeHighZRampVrms  = 0.01;
    const int decimalsAmplitudeHighZRampVrms = 3;

    const double maxAmplitude50OhmsNoiseVpp   = 10;
    const double minAmplitude50OhmsNoiseVpp   = 0.05;
    const double stepAmplitude50OhmsNoiseVpp  = 0.1;
    const int decimalsAmplitude50OhmsNoiseVpp = 3;

    const double maxAmplitude50OhmsNoiseVrms   = 1.62;
    const double minAmplitude50OhmsNoiseVrms   = 0.01;
    const double stepAmplitude50OhmsNoiseVrms  = 0.01;
    const int decimalsAmplitude50OhmsNoiseVrms = 3;

    const double maxAmplitudeHighZNoiseVpp   = 20;
    const double minAmplitudeHighZNoiseVpp   = 0.1;
    const double stepAmplitudeHighZNoiseVpp  = 0.01;
    const int decimalsAmplitudeHighZNoiseVpp = 3;

    const double maxAmplitudeHighZNoiseVrms   = 3.24;
    const double minAmplitudeHighZNoiseVrms   = 0.02;
    const double stepAmplitudeHighZNoiseVrms  = 0.01;
    const int decimalsAmplitudeHighZNoiseVrms = 3;

    const double maxOffsetV   = 5.00;
    const double minOffsetV   = 0.00;
    const double stepOffsetV  = 0.1;
    const int decimalsOffsetV = 2;

    const double   maxFrequencySine = 3.1E6;
    const double   minFrequencySine = 1E-6;
    const double  stepFrequencySine = 1E-6;
    const int decimalsFrequencySine = 6;

    const double   maxFrequencySquare = 3.1E6;
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

    const double   maxFrequencyNoise = 3.5E6;
    const double   minFrequencyNoise = 3.5E6;
    const double  stepFrequencyNoise = 0;
    const int decimalsFrequencyNoise = 0;

    SRS* srs;
    QSerialPort* serial;

    QStringList baudrateList;
    QStringList amplitudeTypeList;
    QStringList functionList;
    QStringList outputImpedanceList;

public:
    explicit DS335();
    explicit DS335(const QString &portName, const int &baudrate, const int &stopBit = 1, const int &parity = 0);
    ~DS335();

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

    void initAmplitudeTypeList();
    QStringList getAmplitudeTypeList() const;
    double getMinAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const;
    double getMaxAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const;
    double getStepAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const;
    double getDecimalsAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const;
    bool isValidAmplitude(const double &amplitude, const QString &waveform, const QString &outputZ, const bool &VRMS) const;
    bool setAmplitude(const double &amplitude, const bool &VRMS) const;
    double getAmplitude(const bool &VRMS) const;

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

    bool setSynchronization(const bool &synchronization) const;
    bool getSynchronization() const;

    void initOutputImpedanceList();
    QStringList getOutputImpedanceList() const;
    int outputImpedanceNumberFromString(const QString &outputImpedance_string) const;
    QString outputImpedanceStringFromNumber(const int &outputImpedance_number) const;
    bool setOutputImpedance(const int &outputImpedance) const;
    bool setOutputImpedance(const QString &outputImpedance) const;
    QString getOutputImpedance() const;

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

#endif // DS335_H
