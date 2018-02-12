#ifndef SR844_H
#define SR844_H

#include <QObject>
#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class SR844 : public QObject
{
    Q_OBJECT

public:
    explicit SR844(QObject *parent = 0);
    explicit SR844(const QString &portName, const int &new_baudrate); // TAKEN SR856
    ~SR844(); // TAKEN SR856

    void setSerial(QSerialPort* new_serial); // TAKEN SR856
    QSerialPort* getSerial() const; // TAKEN SR856

    bool setConnection(const QString &portName, const int &new_baudrate); // TAKEN SR856
    void disconnect() const; // TAKEN SR856

    bool isValidBaudrate(const int &new_baudrate) const; // MODIFIED SR856
    bool changeBaudrate(const int &new_baudrate); // TAKEN SR856
    QStringList validBaudrates() const;

    bool isActive() const; // TAKEN SR856

    bool send(const QString &command, QString &response, const bool &wait_for_response) const; // TAKEN SR856
    bool sendCommand(const QString &command) const; // TAKEN SR856
    bool sendQuery(const QString &command, QString &response) const; // TAKEN SR856

    bool getIDN(QString &idn) const; // TAKEN SR856

signals:
    void errorSignal(const QString &s) const;
    void responseSignal(const QString &s) const;
    void commandSignal(const QString &s) const;
    void timeoutSignal(const QString &s) const;

public slots:
};

#endif // SR844_H
