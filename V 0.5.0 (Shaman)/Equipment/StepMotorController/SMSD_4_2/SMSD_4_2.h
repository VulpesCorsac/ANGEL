#ifndef SMSD_4_2_H
#define SMSD_4_2_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTime>

class SMSD_4_2 : public QObject {
    Q_OBJECT

private:
    QSerialPort *serial;

    int waitTimeout;

    bool active;
    int stepDenominator = 16;
    bool rightMovementDirection = true;
    int speed = 10000;
    int acceleration = 0;
    int startingSpeed = 1;

public:
    explicit SMSD_4_2();
    explicit SMSD_4_2(const QString &portName, int new_waitTimeout = 500);
    ~SMSD_4_2();

    bool activate();
    bool deactivate();
    bool isActive() const;

    QSerialPort* getSerial() const;

    bool setConnection(const QString &portName, int new_waitTimeout = 500);
    bool disconnect();

    void sendCommand(const QString &command, QString &response) const;

    bool setStepDenominator(const int &new_stepDenominator);
    int  getStepDenominator() const;

    bool setDefault();

    bool setSpeed(const int &new_speed);
    int  getSpeed() const;

    bool setMovementDirection(const bool &isRightMovementDirection);
    bool isRightMovementDirecton() const;

    bool setAcceleration(const int &new_acceleration);
    int  getAcceleration() const;

    bool setStartingSpeed(const int &new_startingSpeed);
    int  getStartingSpeed() const;

    bool setReverse();

    bool setPause(const int &pause) const;

    bool moveUntillZero() const;
    bool movement() const;

    bool moveStep(const int &new_speed, const bool &new_isRightMovementDirection, const int &stepAmmount);
    bool moveStep(const int &stepAmmount);

    bool relayEnable() const;
    bool relayDisable() const;

    bool moveUntillHigh() const;
    bool moveUntillLow() const;

    bool waitUntillHigh() const;
    bool waitUntillLow() const;

signals:
    void error(const QString &s) const;
    void response(const QString &s) const;
    void command(const QString &s) const;
    void timeout(const QString &s) const;
};

#endif // SMSD_4_2_H
