#ifndef TEMPERATURECONTROLLER_H
#define TEMPERATURECONTROLLER_H

#include <QObject>
#include <QDebug>

#include <QTime>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QTest>

#include "../../../Functions/AllFunctions.h"
#include "../../CommonFunctionsForSRS/SRS.h"
#include "../AllTemperatureControllers.h"

class TemperatureController : public QObject
{
    Q_OBJECT

private:
    SRS* srs;

    PTC10* ptc10;

    QString TemperatureControllerModel;

    const int waitTime = 700;

public:
    explicit TemperatureController(QObject *parent = 0);
    explicit TemperatureController(const QString &portName, const int &baudrate, const int &stopBit = 1, const int &parity = 0, const QString &new_lockInAmplifierModel = "");
    explicit TemperatureController(QSerialPort *serial, const QString &new_temperatureControllerModel = "");
    explicit TemperatureController(PTC10* new_PTC10);
    ~TemperatureController();

    void toNULL();
    void init();

    void wait(const double &x = 1) const;

    void setPTC10(PTC10* new_ptc10);
    PTC10* getPTC10() const;

    bool openSerial(QSerialPort *serial) const;

    QStringList getSupportedModels() const;
    void setTemperatureControllerModel(const QString &new_temperatureControllerModel);
    QString getTemperatureControllerModel() const;
    bool autoSetTemperatureControllerModel(QSerialPort *serial);
    bool autoSetTemperatureControllerModel(const QString &portName, const int &baudrate);
    bool autoSetTemperatureControllerModel(const QString &portName);

    bool setConnection(const QString &portName, const int &baudrate, const int &stopBit = 1, const int &parity = 0, const QString &new_lockInAmplifierModel = "");
    bool setConnectionBrute(const QString &portName, const QString &new_lockInAmplifierModel = "");

    bool disconnect() const;

    QStringList getBaudrateList() const;
    bool isValidBaudrateAll(const int &baudrate) const;
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

    bool send(const QString &command, QString &response, const bool &waitForResponse) const;
    bool sendCommand(const QString &command) const;
    bool sendQuery(const QString &command, QString &response) const;
    QString ask(const QString &command) const;

    bool getIDN(QString &idn) const;

    bool test() const;

    double getAverageInputTime() const;
    double getAverageOutputTime() const;

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

#endif // TEMPERATURECONTROLLER_H
