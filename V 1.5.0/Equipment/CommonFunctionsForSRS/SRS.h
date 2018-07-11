#ifndef SRS_H
#define SRS_H

#include <QtSerialPort/QSerialPort>
#include <QTime>

#include <QtTest/QTest>

#include "../../HardwareCommunication/SerialPort/SerialPort.h"

#include <qDebug>

class SRS : public QObject
{
    Q_OBJECT

private:
    int attemptsToConnect = 5;

    QStringList supportedList;

    SerialPort *serialCommunication;

public:
    explicit SRS(QObject *parent = 0);
    ~SRS();

    void toNull();
    void init();

    QStringList getSupportedList() const;
    QStringList getBaudrateList() const;

    void setWriteTimeout(const int &new_writeTimeout);
    int getWriteTimeout() const;

    void setReadTimeout(const int &new_readTimeout);
    int getReadTimeout() const;

    void setReadWaitTimeout(const int &new_readWaitTimeout);
    int getReadWaitTimeout() const;

    void setAttemptsToConnect(const int &new_attemptsToConnect);
    int getAttemptsToConnect() const;

    QSerialPort* getSerial(QSerialPort* const this_serial) const;
    void setSerial(QSerialPort* *this_serial, QSerialPort* const new_serial) const;

    bool openSerial(QSerialPort* const this_serial) const;
    bool setConnection(QSerialPort* *this_serial, const QString &portName, const int &baudrate, const int &stopbit = 1, const int &parity = 0, const QString flowControl = "") const;
    void disconnect(QSerialPort* const this_serial) const;

    QString autoDetect(QSerialPort* const this_serial) const;
    QString autoDetect(const QString &portName, const int &baudrate, const int &stopbit = 1, const int &parity = 0, const QString flowControl = "") const;
    QString autoDetect(const QString &portName) const;

    bool send(QSerialPort* const this_serial, const QString &command, QString &response, const bool &wait_for_response) const;
    bool sendCommand(QSerialPort* const this_serial, const QString &command) const;
    bool sendQuery(QSerialPort* const this_serial, const QString &command, QString &response) const;
    QString ask(QSerialPort* const this_serial, const QString &command) const;

    bool getIDN(QSerialPort* const this_serial, QString &idn) const;
    bool isActive(QSerialPort* const this_serial) const;

    void initQStringList(QStringList &target, const QStringList &source) const;
    int numberFromString(const QStringList &list, const QString &string) const;
    QString stringFromNumber(const QStringList &list, const int &number) const;
    bool isValidString(const QStringList &list, const QString &string) const;
    bool isValidNumber(const QStringList &list, const int &number) const;

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

#endif // SRS_H
