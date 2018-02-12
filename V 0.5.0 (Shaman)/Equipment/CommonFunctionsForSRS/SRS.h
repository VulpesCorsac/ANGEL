#ifndef SRS_H
#define SRS_H

#include <QSerialPort>
#include <QTime>

class SRS : public QObject
{
    Q_OBJECT

private:
    const int defaultWriteTimeout    = 50;
    const int defaultReadTimeout     = 200;
    const int defaultReadWaitTimeout = 50;

public:
    explicit SRS(QObject *parent = 0);
    ~SRS();

    QSerialPort* getSerial(QSerialPort* const this_serial) const;
    void setSerial(QSerialPort* *this_serial, QSerialPort* const new_serial) const;

    bool openSerial(QSerialPort* const this_serial) const;
    bool setConnection(QSerialPort* *this_serial, const QString &portName, const int &new_baudrate) const;
    void disconnect(QSerialPort* const this_serial) const;

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

signals:
    void errorSignal(const QString &s) const;
    void responseSignal(const QString &s) const;
    void commandSignal(const QString &s) const;
    void timeoutSignal(const QString &s) const;
};

#endif // SRS_H
