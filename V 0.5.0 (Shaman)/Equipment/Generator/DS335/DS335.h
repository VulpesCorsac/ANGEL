#ifndef DS335_H
#define DS335_H

#include <QObject>
#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class DS335 : public QObject
{
    Q_OBJECT

private:
    const int defaultWriteTimeout    = 50;
    const int defaultReadTimeout     = 200;
    const int defaultReadWaitTimeout = 50;

    QSerialPort* serial;

    const double maxAmplitude50OhmSineVpp   = 10;
    const double minAmplitude50OhmSineVpp   = 0.05;
    const double stepAmplitude50OhmSineVpp  = 0.01;
    const int decimalsAmplitude50OhmSineVpp = 3;

    const double maxAmplitude50OhmSineVrms   = 3.54;
    const double minAmplitude50OhmSineVrms   = 0.02;
    const double stepAmplitude50OhmSineVrms  = 0.01;
    const int decimalsAmplitude50OhmSineVrms = 3;

    const double maxAmplitudeHighZSineVpp   = 20;
    const double minAmplitudeHighZSineVpp   = 0.1;
    const double stepAmplitudeHighZSineVpp  = 0.01;
    const int decimalsAmplitudeHighZSineVpp = 3;

    const double maxAmplitudeHighZSineVrms   = 7.07;
    const double minAmplitudeHighZSineVrms   = 0.04;
    const double stepAmplitudeHighZSineVrms  = 0.01;
    const int decimalsAmplitudeHighZSineVrms = 3;

    const double maxAmplitude50OhmSquareVpp   = 10;
    const double minAmplitude50OhmSquareVpp   = 0.05;
    const double stepAmplitude50OhmSquareVpp  = 0.01;
    const int decimalsAmplitude50OhmSquareVpp = 3;

    const double maxAmplitude50OhmSquareVrms   = 5;
    const double minAmplitude50OhmSquareVrms   = 0.03;
    const double stepAmplitude50OhmSquareVrms  = 0.01;
    const int decimalsAmplitude50OhmSquareVrms = 3;

    const double maxAmplitudeHighZSquareVpp   = 20;
    const double minAmplitudeHighZSquareVpp   = 0.1;
    const double stepAmplitudeHighZSquareVpp  = 0.01;
    const int decimalsAmplitudeHighZSquareVpp = 3;

    const double maxAmplitudeHighZSquareVrms   = 10;
    const double minAmplitudeHighZSquareVrms   = 0.05;
    const double stepAmplitudeHighZSquareVrms  = 0.01;
    const int decimalsAmplitudeHighZSquareVrms = 3;

    const double maxAmplitude50OhmTriangleVpp   = 10;
    const double minAmplitude50OhmTriangleVpp   = 0.05;
    const double stepAmplitude50OhmTriangleVpp  = 0.01;
    const int decimalsAmplitude50OhmTriangleVpp = 3;

    const double maxAmplitude50OhmTriangleVrms   = 2.89;
    const double minAmplitude50OhmTriangleVrms   = 0.01;
    const double stepAmplitude50OhmTriangleVrms  = 0.01;
    const int decimalsAmplitude50OhmTriangleVrms = 3;

    const double maxAmplitudeHighZTriangleVpp   = 20;
    const double minAmplitudeHighZTriangleVpp   = 0.1;
    const double stepAmplitudeHighZTriangleVpp  = 0.01;
    const int decimalsAmplitudeHighZTriangleVpp = 3;

    const double maxAmplitudeHighZTriangleVrms   = 5.77;
    const double minAmplitudeHighZTriangleVrms   = 0.03;
    const double stepAmplitudeHighZTriangleVrms  = 0.01;
    const int decimalsAmplitudeHighZTriangleVrms = 3;

    const double maxAmplitude50OhmRampVpp   = 10;
    const double minAmplitude50OhmRampVpp   = 0.05;
    const double stepAmplitude50OhmRampVpp  = 0.01;
    const int decimalsAmplitude50OhmRampVpp = 3;

    const double maxAmplitude50OhmRampVrms   = 2.89;
    const double minAmplitude50OhmRampVrms   = 0.01;
    const double stepAmplitude50OhmRampVrms  = 0.01;
    const int decimalsAmplitude50OhmRampVrms = 3;

    const double maxAmplitudeHighZRampVpp   = 20;
    const double minAmplitudeHighZRampVpp   = 0.1;
    const double stepAmplitudeHighZRampVpp  = 0.01;
    const int decimalsAmplitudeHighZRampVpp = 3;

    const double maxAmplitudeHighZRampVrms   = 5.77;
    const double minAmplitudeHighZRampVrms   = 0.03;
    const double stepAmplitudeHighZRampVrms  = 0.01;
    const int decimalsAmplitudeHighZRampVrms = 3;

    const double maxAmplitude50OhmNoiseVpp   = 10;
    const double minAmplitude50OhmNoiseVpp   = 0.05;
    const double stepAmplitude50OhmNoiseVpp  = 0.1;
    const int decimalsAmplitude50OhmNoiseVpp = 3;

    const double maxAmplitude50OhmNoiseVrms   = 1.62;
    const double minAmplitude50OhmNoiseVrms   = 0.01;
    const double stepAmplitude50OhmNoiseVrms  = 0.01;
    const int decimalsAmplitude50OhmNoiseVrms = 3;

    const double maxAmplitudeHighZNoiseVpp   = 20;
    const double minAmplitudeHighZNoiseVpp   = 0.1;
    const double stepAmplitudeHighZNoiseVpp  = 0.01;
    const int decimalsAmplitudeHighZNoiseVpp = 3;

    const double maxAmplitudeHighZNoiseVrms   = 3.24;
    const double minAmplitudeHighZNoiseVrms   = 0.02;
    const double stepAmplitudeHighZNoiseVrms  = 0.01;
    const int decimalsAmplitudeHighZNoiseVrms = 3;

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

public:
    explicit DS335();
    explicit DS335(const QString &portName, const int &new_baudrate);
    ~DS335();

    void setSerial(QSerialPort* new_serial);
    QSerialPort* getSerial() const;

    bool setConnection(const QString &portName, const int &new_baudrate);
    void disconnect() const;

    bool isValidBaudrate(const int &new_baudrate) const;
    bool changeBaudrate(const int &new_baudrate);

    bool isActive() const;

    bool send(const QString &command, QString &response, const bool &wait_for_response) const;
    bool sendCommand(const QString &command) const;
    bool sendQuery(const QString &command, QString &response) const;

    bool getIDN(QString &idn) const;

    bool setAmplitude(const double &new_amplitude, const bool &VRMS) const;
    double getAmplitude(const bool &VRMS) const;
    double getMaxAmplitude() const; // TODO

    bool setFrequency(const double &new_frequency) const;
    double getFrequency() const;
    double getMaxFrequency() const; // TODO

    int functionToNumber(const QString &function_string) const;
    QString numberToFunction(const int &function_number) const;
    QStringList getFunctionList() const; // TODO

    bool setFunction(const int &function_number) const;
    bool setFunction(const QString &function) const;
    QString getFunction() const;

    bool setInverse(const bool &new_inverse) const;
    bool getInverse() const;

    bool setSynchronization(const bool &new_synchronization) const;
    bool getSynchronization() const;

    int impedanceToNumber(const QString &impedance_string) const;
    QString numberToImpedance(const int &impedance_number) const;
    QStringList getImpedanceList() const; // TODO

    bool setOutputImpedance(const QString &impedance_string) const;
    bool setOutputImpedance(const int &impedance_number) const;
    QString getOutputImpedance() const;

signals:
    void errorSignal(const QString &s) const;
    void responseSignal(const QString &s) const;
    void commandSignal(const QString &s) const;
    void timeoutSignal(const QString &s) const;
};

#endif // DS335_H
