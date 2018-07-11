#ifndef PTC10_H
#define PTC10_H

#include <QObject>
#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

#include "../../CommonFunctionsForSRS/SRS.h"

class PTC10 : public QObject
{
    Q_OBJECT

private:
    SRS* srs;
    QSerialPort* serial;

    QStringList baudrateList;
    QStringList channels;
    QStringList units;

    bool oldModel = false;

public:
    explicit PTC10();
    explicit PTC10(const QString &portName, const int &baudrate, const int &stopBit = 1, const int &parity = 0);
    ~PTC10();

    void init();

    void setSerial(QSerialPort* new_serial);
    QSerialPort* getSerial() const;

    bool setConnection(const QString &portName, const int &baudrate, const int &stopBit = 1, const int &parity = 0);
    void disconnect() const;

    void initBaudrateList();
    QStringList getBaudrateList();
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

    bool isOldModel() const;
    void setModel(const bool &new_oldModel);

    void fillChannels();
    QStringList getChannels();
    void fillUnits();
    QStringList getUnits();
    QString getUnitAt(const int &i) const;

    void setOutputEnabled(const bool &new_outputEnabled) const;
    bool getOutputEnabled() const;
    void setPower(const double &new_power) const;
    double getPower() const;

    void setPIDChannel(const QString &channel) const;
    QString getPIDChannel() const;
    void setPIDTemperature(const double &new_temperature) const;
    double getPIDTemperature() const;
    void setPIDCoefficientP(const double &new_P) const;
    double getPIDCoefficientP() const;
    void setPIDCoefficientI(const double &new_I) const;
    double getPIDCoefficientI() const;
    void setPIDCoefficientD(const double &new_D) const;
    double getPIDCoefficientD() const;
    void setPIDCoefficientsPID(const double &new_P, const double &new_I, const double &new_D) const;
    void setPIDAll(const QString &channel, const double &new_temperature, const double &new_P, const double &new_I, const double &new_D) const;

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

#endif // PTC10_H
