#include "DS345.h"

DS345::DS345()
{
    this->serial = nullptr;
}

DS345::DS345(const QString &portName, const int &new_baudrate)
{
    this->serial = nullptr;
    setConnection(portName, new_baudrate);
}

DS345::~DS345()
{
    if (this->serial->isOpen())
        this->serial->close();
    this->serial->~QSerialPort();
}

QSerialPort* DS345::getSerial() const
{
    return this->serial;
}

void DS345::setSerial(QSerialPort *new_serial)
{
    this->serial = new_serial;
}

bool DS345::setConnection(const QString &portName, const int &new_baudrate)
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

void DS345::disconnect() const
{
    if (this->serial->isOpen())
        this->serial->close();
}

bool DS345::isValidBaudrate(const int &new_baudrate) const
{
    if (new_baudrate == 1200 || new_baudrate == 2400 ||
        new_baudrate == 4800 || new_baudrate == 9600)
        return true;
    return false;
}

bool DS345::changeBaudrate(const int &new_baudrate)
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

bool DS345::isActive() const
{
    QString idn;
    if (getIDN(idn))
        return true;
    return false;
}

bool DS345::send(const QString &command, QString &response, const bool &wait_for_response) const
{
    emit this->commandSignal(command);
    response = "";

    if (this->serial == nullptr || !this->serial->isOpen())
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
        } else
            return true;
    } else {
        emit this->timeoutSignal(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
        return false;
    }
}

bool DS345::sendCommand(const QString &command) const
{
    QString response;
    return send(command, response, false);
}

bool DS345::sendQuery(const QString &command, QString &response) const
{
    return send(command, response, true);
}

bool DS345::getIDN(QString &idn) const
{
    return sendQuery("*IDN?", idn);
}

bool DS345::correctAmplitudeType(const QString &type) const
{
    if (type.trimmed() == "VR" || type.trimmed() == "VP" || type.trimmed() == "DB")
        return true;
    return false;
}

bool DS345::setAmplitude(const double &new_amplitude, const QString &type) const
{
    QString command = "AMLP " + QString::number(new_amplitude) + " ";
    if (correctAmplitudeType(type))
        command += type.trimmed();
    else
        return false;
    return sendCommand(command);
}

double DS345::getAmplitude(QString &type) const
{
    QString response;
    QString command = "AMPL? ";
    if (correctAmplitudeType(type))
        command += type.trimmed();
    if (!sendQuery(command, response))
        return -1;
    if (correctAmplitudeType(type))
        type = response.right(2);
    return response.trimmed().left(response.length() - 2).toDouble();
}

bool DS345::setFrequency(const double &new_frequency) const
{
    QString command = "FREQ " + QString::number(new_frequency);
    return sendCommand(command);
}

double DS345::getFrequency() const
{
    QString response;
    QString command = "FREQ?";
    if (!sendQuery(command, response))
        return -1;
    else
        return response.trimmed().toDouble();
}

int DS345::functionToNumber(const QString &function_string) const
{
    QString function = function_string.trimmed();
    if (function.trimmed() == "SINE")
        return 0;
    if (function.trimmed() == "SQUARE")
        return 1;
    if (function.trimmed() == "TRIANGLE")
        return 2;
    if (function.trimmed() == "RAMP")
        return 3;
    if (function.trimmed() == "NOISE")
        return 4;
    if (function.trimmed() == "ARBITRARY")
        return 5;
    return -1;
}

QString DS345::numberToFunction(const int &function_number) const
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
    if (function_number == 5)
        return "ARBITRARY";
    return "";
}

bool DS345::setFunction(const int &function_number) const
{
    QString command = "FUNC " + QString::number(function_number);
    return sendCommand(command);
}

bool DS345::setFunction(const QString &function) const
{
    return setFunction(functionToNumber(function));
}

QString DS345::getFunction() const
{
    QString command = "FUNC?";
    QString response;
    if (sendQuery(command, response))
        return numberToFunction(response.trimmed().toInt());
    else
        return "";
}

bool DS345::setInverse(const bool &new_inverse) const
{
    QString command = "INVT ";
    if (new_inverse)
        command = "1";
    else
        command = "0";
    return sendCommand(command);
}

bool DS345::getInverse() const
{
    QString command = "INVT?";
    QString response;
    sendQuery(command, response);
    return (response.trimmed().toInt() == 1);
}

bool DS345::setSynchronization(const bool &new_synchronization) const
{
    QString command = "SYNC ";
    if (new_synchronization)
        command = "1";
    else
        command = "0";
    return sendCommand(command);
}

bool DS345::getSynchronization() const
{
    QString command = "SYNC?";
    QString response;
    sendQuery(command, response);
    return (response.trimmed().toInt() == 1);
}

bool DS345::setAMdepth(const int &new_persentage) const
{
    QString command = "DPTH " + QString::number(new_persentage);
    return sendCommand(command);
}

int DS345::getAMdepth() const
{
    QString command = "DPTH?";
    QString response;
    sendQuery(command, response);
    return response.trimmed().toInt();
}

bool DS345::setFMspan(const double &new_span) const
{
    QString command = "FDEV " + QString::number(new_span);
    return sendCommand(command);
}

double DS345::getFMspan() const
{
    QString command = "FDEV?";
    QString response;
    sendQuery(command, response);
    return response.trimmed().toDouble();

}

int DS345::modulateFunctionToNumber(const QString &function_string) const
{
    if (function_string.trimmed() == "SINGLE SWEEP")
        return 0;
    if (function_string.trimmed() == "RAMP")
        return 1;
    if (function_string.trimmed() == "TRIANGLE")
        return 2;
    if (function_string.trimmed() == "SINE")
        return 3;
    if (function_string.trimmed() == "SQUARE")
        return 4;
    if (function_string.trimmed() == "ARB")
        return 5;
    if (function_string.trimmed() == "NONE")
        return 6;
    return -1;
}

QString DS345::numberToModulateFunction(const int &function_number) const
{
    if (function_number == 0)
        return "SINGLE SWEEP";
    if (function_number == 1)
        return "RAMP";
    if (function_number == 2)
        return "TRIANGLE";
    if (function_number == 3)
        return "SINE";
    if (function_number == 4)
        return "SQUARE";
    if (function_number == 5)
        return "ARB";
    if (function_number == 6)
        return "NONE";
    return "";
}

bool DS345::setModulationFunction(const int &function_number) const
{
    QString command = "MDWF " + QString::number(function_number);
    return sendCommand(command);
}

bool DS345::setModulationFunction(const QString &function_string) const
{
    return setModulationFunction(modulateFunctionToNumber(function_string));
}

QString DS345::getModulationFunction() const
{
    QString command = "MDWF?";
    QString response;
    sendQuery(command, response);
    return numberToModulateFunction(response.trimmed().toInt());
}

bool DS345::setModulationEnabled(const bool &enable) const
{
    QString command = "MENA ";
    if (enable)
        command += "1";
    else
        command += "0";
    return sendCommand(command);
}

bool DS345::getModulationEnabled() const
{
    QString command = "MENA?";
    QString response;
    sendQuery(command, response);
    return (response.trimmed() == "1");
}

QString DS345::numberToModulateType(const int &type_number) const
{
    if (type_number == 0)
        return "LIN SWEEP";
    if (type_number == 1)
        return "LOG SWEEP";
    if (type_number == 2)
        return "INTERNAL AM";
    if (type_number == 3)
        return "FM";
    if (type_number == 4)
        return "fm";
    if (type_number == 5)
        return "BURST";
    return "";
}

int DS345::modulateTypeToNumber(const QString &function_string) const
{
    if (function_string.trimmed() == "LIN SWEEP")
        return 0;
    if (function_string.trimmed() == "LOG SWEEP")
        return 1;
    if (function_string.trimmed() == "INTERNAL AM")
        return 2;
    if (function_string.trimmed() == "FM")
        return 3;
    if (function_string.trimmed() == "fm")
        return 4;
    if (function_string.trimmed() == "BURST")
        return 5;

    return -1;
}

bool DS345::setModulationType(const int &function_number) const
{
    QString command = "MTYP " + QString::number(function_number);
    return sendCommand(command);
}

bool DS345::setModulationType(const QString &function_string) const
{
    return setModulationType(modulateTypeToNumber(function_string));
}

QString DS345::getModulationType() const
{
    QString command = "MTYP?";
    QString response;
    sendQuery(command, response);
    return numberToModulateType(response.trimmed().toInt());
}

bool DS345::setModulationRate(const double &new_rate) const
{
    QString command = "RATE " + QString::number(new_rate);
    return sendCommand(command);
}

double DS345::getModulationRate() const
{
    QString command = "RATE?";
    QString response;
    sendQuery(command, response);
    return response.trimmed().toDouble();
}

bool DS345::setModulationSpan(const double &new_span) const
{
    QString command = "SPAN " + QString::number(new_span);
    return sendCommand(command);
}

double DS345::getModulationSpan() const
{
    QString command = "SPAN?";
    QString response;
    sendQuery(command, response);
    return response.trimmed().toDouble();
}
