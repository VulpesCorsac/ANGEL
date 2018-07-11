#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtSerialPort/QSerialPort>
#include <QTime>

#include <QtTest/QTest>

#include <qDebug>

class SerialPort : public QObject
{
    Q_OBJECT

private:
    int writeTimeout    = 50;
    int readTimeout     = 200;
    int readWaitTimeout = 50;

    QStringList baudrateList;

public:
    explicit SerialPort(QObject *parent = 0);
    ~SerialPort();

    QStringList getBaudrateList() const;

    void setWriteTimeout(const int &new_writeTimeout);
    int getWriteTimeout() const;

    void setReadTimeout(const int &new_readTimeout);
    int getReadTimeout() const;

    void setReadWaitTimeout(const int &new_readWaitTimeout);
    int getReadWaitTimeout() const;

    QSerialPort* getSerial(QSerialPort* const this_serial) const;
    void setSerial(QSerialPort* *this_serial, QSerialPort* const new_serial) const;

    bool openSerial(QSerialPort* const this_serial) const;
    bool setConnection(QSerialPort* *this_serial, const QString &portName, const int &baudrate, const int &databits = 8, const int &stopbit = 1, const int &parity = 0, const QString &flowControl = "") const;
    void disconnect(QSerialPort* const this_serial) const;

    bool send(QSerialPort* const this_serial, const QString &command, QString &response, const bool &wait_for_response, const QString &terminate = "\n") const;
    bool sendCommand(QSerialPort* const this_serial, const QString &command, const QString &terminate = "\n") const;
    bool sendQuery(QSerialPort* const this_serial, const QString &command, QString &response, const QString &terminate = "\n") const;
    QString ask(QSerialPort* const this_serial, const QString &command, const QString &terminate = "\n") const;

signals:
    void errorSignal(const QString &s) const;
    void responseSignal(const QString &s) const;
    void commandSignal(const QString &s) const;
    void timeoutSignal(const QString &s) const;
};

#endif // SERIALPORT_H
