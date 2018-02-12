#include "DS335.h"

DS335::DS335()
{
    this->serial = nullptr;
}

DS335::DS335(const QString &portName, const int &new_baudrate)
{
    this->serial = nullptr;
    setConnection(portName, new_baudrate);
}

DS335::~DS335()
{
    if (this->serial->isOpen())
        this->serial->close();
    this->serial->~QSerialPort();
}

QSerialPort* DS335::getSerial() const
{
    return this->serial;
}

void DS335::setSerial(QSerialPort *new_serial)
{
    this->serial = new_serial;
}

bool DS335::setConnection(const QString &portName, const int &new_baudrate)
{
    if (this->isActive())
        return true;

    if (this->serial == nullptr)
        this->serial = new QSerialPort(this);

    this->serial->setPortName(portName);
    if (isValidBaudrate(new_baudrate))
        this->serial->setBaudRate((qint32) new_baudrate);
    else
        return false;

    this->serial->setParity(QSerialPort::NoParity);
    this->serial->setDataBits(QSerialPort::Data8);
    this->serial->setStopBits(QSerialPort::TwoStop);

    if (!this->serial->open(QIODevice::ReadWrite)) {
        QSerialPort::SerialPortError error;
        this->serial->error(error);

        emit this->errorSignal(tr("Can't open %1, error code %2").arg(portName).arg(serial->error()));

        return false;
    } else {
        QString idn;

        return getIDN(idn);
    }
}

void DS335::disconnect() const
{
    if (this->serial->isOpen())
        this->serial->close();
}

bool DS335::isValidBaudrate(const int &new_baudrate) const
{
    if (new_baudrate == 1200 || new_baudrate == 2400 ||
        new_baudrate == 4800 || new_baudrate == 9600)
        return true;
    return false;
}

bool DS335::changeBaudrate(const int &new_baudrate)
{
    if (!isValidBaudrate(new_baudrate))
        return false;

    if (!this->serial->isOpen()) {
        this->serial->setBaudRate((qint32) new_baudrate);
        return true;
    } else {
        this->serial->close();
        return setConnection(this->serial->portName(), new_baudrate);
    }
}

bool DS335::isActive() const
{
    QString idn;
    if (getIDN(idn))
        return true;
    return false;
}

bool DS335::send(const QString &command, QString &response, const bool &wait_for_response) const
{
    emit this->commandSignal(command);
    response = "";

    if (this->serial == nullptr || !this->serial->isOpen())
    if (this->serial == nullptr)
        return false;

    QString modyfiedCommand = command.trimmed() + "\n";

    serial->write(modyfiedCommand.toLocal8Bit());
    if (serial->waitForBytesWritten(defaultWriteTimeout)) {
        if (wait_for_response) {
            if (this->serial->waitForReadyRead(defaultReadTimeout)) {
                QByteArray responseData = serial->readAll();
                while (this->serial->waitForReadyRead(defaultReadWaitTimeout))
                    responseData += serial->readAll();
                response = QString(responseData);

                emit this->responseSignal(response);
                return true;
            } else {
                emit this->timeoutSignal(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
                return false;
            }
        } else {
            return true;
        }
    } else {
        emit this->timeoutSignal(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
        return false;
    }
}

bool DS335::sendCommand(const QString &command) const
{
    QString response;
    return send(command, response, false);
}

bool DS335::sendQuery(const QString &command, QString &response) const
{
    return send(command, response, true);
}

bool DS335::getIDN(QString &idn) const
{
    return sendQuery("*IDN?", idn);
}

bool DS335::setAmplitude(const double &new_amplitude, const bool &VRMS) const
{
    QString command = "AMPL " + QString::number(new_amplitude);
    if (VRMS)
        command += "VR";
    else
        command += "VP";
    return sendCommand(command);
}

double DS335::getAmplitude(const bool &VRMS) const
{
    QString response;
    QString command = "AMPL? ";
    if (VRMS)
        command += "VR";
    else
        command += "VP";
    if (!sendQuery(command, response))
        return -1;
    else
        return response.left(response.length() - 2).toDouble();
}

bool DS335::setFrequency(const double &new_frequency) const
{
    QString command = "FREQ " + QString::number(new_frequency);
    return sendCommand(command);
}

double DS335::getFrequency() const
{
    QString response;
    QString command = "FREQ?";
    if (!sendQuery(command, response))
        return -1;
    else
        return response.toDouble();
}

int DS335::functionToNumber(const QString &function_string) const
{
    QString function = function_string.trimmed();
    if (function == "SINE")
        return 0;
    if (function == "SQUARE")
        return 1;
    if (function == "TRIANGLE")
        return 2;
    if (function == "RAMP")
        return 3;
    if (function == "NOISE")
        return 4;
    return -1;
}

QString DS335::numberToFunction(const int &function_number) const
{
    if (function_number == 0)
        return "SINE";
    if (function_number == 1)
        return "SQUARE";
    if (function_number == 2)
        return "TRIANGLE";
    if (function_number == 3)
        return "RAMP";
    if (function_number == 4)
        return "NOISE";
    return "";
}

bool DS335::setFunction(const int &function_number) const
{
    QString command = "FUNC " + QString::number(function_number);
    return sendCommand(command);
}

bool DS335::setFunction(const QString &function) const
{
    return setFunction(functionToNumber(function));
}

QString DS335::getFunction() const
{
    QString command = "FUNC?";
    QString response;
    if (sendQuery(command, response))
        return numberToFunction(response.trimmed().toInt());
    else
        return "";
}

bool DS335::setInverse(const bool &new_inverse) const
{
    QString command = "INVT ";
    if (new_inverse)
        command = "1";
    else
        command = "0";
    return sendCommand(command);
}

bool DS335::getInverse() const
{
    QString command = "INVT?";
    QString response;
    sendQuery(command, response);
    return (response.trimmed().toInt() == 1);
}

bool DS335::setSynchronization(const bool &new_synchronization) const
{
    QString command = "SYNC ";
    if (new_synchronization)
        command = "1";
    else
        command = "0";
    return sendCommand(command);
}

bool DS335::getSynchronization() const
{
    QString command = "SYNC?";
    QString response;
    sendQuery(command, response);
    return (response.trimmed().toInt() == 1);
}

int DS335::impedanceToNumber(const QString &impedance_string) const
{
    if (impedance_string == "50 Ohm")
        return 0;
    return 1;
}

QString DS335::numberToImpedance(const int &impedance_number) const
{
    if (impedance_number == 0)
        return "50 Ohm";
    if (impedance_number == 1)
        return "hi-Z";
    return "";
}

bool DS335::setOutputImpedance(const QString &impedance_string) const
{
    QString command = "TERM " + QString::number(impedanceToNumber(impedance_string));
    return sendCommand(command);
}

bool DS335::setOutputImpedance(const int &impedance_number) const
{
    return setOutputImpedance(numberToImpedance(impedance_number));
}

QString DS335::getOutputImpedance() const
{
    QString command = "TERM?";
    QString response;
    sendQuery(command, response);
    return numberToImpedance(response.trimmed().toInt());
}
