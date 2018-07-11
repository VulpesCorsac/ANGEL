#include "SerialPort.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
//    this->baudrateList.push_back(   "300");
//    this->baudrateList.push_back(   "600");
//    this->baudrateList.push_back(  "1200");
//    this->baudrateList.push_back(  "2400");
//    this->baudrateList.push_back(  "4800");
    this->baudrateList.push_back(  "9600");
    this->baudrateList.push_back( "19200");
//    this->baudrateList.push_back( "38400");
//    this->baudrateList.push_back( "57600");
//    this->baudrateList.push_back("115200");
}

SerialPort::~SerialPort()
{
    this->baudrateList.clear();
}

QStringList SerialPort::getBaudrateList() const
{
    return this->baudrateList;
}

void SerialPort::setWriteTimeout(const int &new_writeTimeout)
{
    this->writeTimeout = new_writeTimeout;

    return;
}

int SerialPort::getWriteTimeout() const
{
    return this->writeTimeout;
}

void SerialPort::setReadTimeout(const int &new_readTimeout)
{
    this->readTimeout = new_readTimeout;

    return;
}

int SerialPort::getReadTimeout() const
{
    return this->readTimeout;
}

void SerialPort::setReadWaitTimeout(const int &new_readWaitTimeout)
{
    this->readWaitTimeout = new_readWaitTimeout;

    return;
}

int SerialPort::getReadWaitTimeout() const
{
    return this->readWaitTimeout;
}

QSerialPort* SerialPort::getSerial (QSerialPort* const this_serial) const
{
    return this_serial;
}

void SerialPort::setSerial(QSerialPort* *this_serial, QSerialPort* const new_serial) const
{
    *this_serial = new_serial;

    return;
}

bool SerialPort::openSerial(QSerialPort* const this_serial) const
{
    if (!this_serial->open(QIODevice::ReadWrite)) {
        QSerialPort::SerialPortError error = QSerialPort::NoError;
        this_serial->error(error);

        emit this->errorSignal(tr("Can't open %1, error code %2").arg(this_serial->portName()).arg(this_serial->error()));

        return false;
    } else {
        return true;
    }
}

bool SerialPort::setConnection(QSerialPort* *this_serial, const QString &portName, const int &baudrate, const int &databits, const int &stopbit, const int &parity, const QString &flowControl) const
{
    if (*this_serial == nullptr)
        *this_serial = new QSerialPort();

    if (databits == 5)
        (*this_serial)->setDataBits(QSerialPort::Data5);
    if (databits == 6)
        (*this_serial)->setDataBits(QSerialPort::Data6);
    if (databits == 7)
        (*this_serial)->setDataBits(QSerialPort::Data7);
    if (databits == 8)
        (*this_serial)->setDataBits(QSerialPort::Data8);

    if (stopbit == 1)
        (*this_serial)->setStopBits(QSerialPort::OneStop);
    if (stopbit == 2)
        (*this_serial)->setStopBits(QSerialPort::TwoStop);
    if (stopbit == 3)
        (*this_serial)->setStopBits(QSerialPort::OneAndHalfStop);

    if (parity == 0)
        (*this_serial)->setParity(QSerialPort::NoParity);
    if (parity == 1)
        (*this_serial)->setParity(QSerialPort::OddParity);
    if (parity == 2)
        (*this_serial)->setParity(QSerialPort::EvenParity);

    if (flowControl.toUpper() == "" ||
        flowControl.toUpper() == "NONE")
        (*this_serial)->setFlowControl(QSerialPort::NoFlowControl);
    if (flowControl.toUpper() == "HARD" ||
        flowControl.toUpper() == "HARDWARE")
        (*this_serial)->setFlowControl(QSerialPort::HardwareControl);
    if (flowControl.toUpper() == "SOFT" ||
        flowControl.toUpper() == "SOFTWARE")
        (*this_serial)->setFlowControl(QSerialPort::SoftwareControl);

    (*this_serial)->setPortName(portName);
    (*this_serial)->setBaudRate((qint32) baudrate);

    (*this_serial)->setDataBits(QSerialPort::Data8);

    if (!(*this_serial)->open(QIODevice::ReadWrite)) {
        QSerialPort::SerialPortError error = QSerialPort::NoError;
        (*this_serial)->error(error);

        emit this->errorSignal(tr("Can't open %1, error code %2").arg(portName).arg((*this_serial)->error()));

        return false;
    }

    return false;
}

void SerialPort::disconnect(QSerialPort* const this_serial) const
{
    if (this_serial != nullptr)
        if (this_serial->isOpen())
            this_serial->close();
    return;
}

bool SerialPort::send(QSerialPort* const this_serial, const QString &command, QString &response, const bool &wait_for_response, const QString &terminate) const
{
    emit this->commandSignal(command);
    response.clear();

    if (this_serial == nullptr || !this_serial->isOpen())
        return false;

    QString modyfiedCommand = command;
    if (terminate == "LF" ||
        terminate == "\n")
        modyfiedCommand += "\n";
    if (terminate == "CR" ||
        terminate == "\r")
        modyfiedCommand += "\r";
    if (terminate == "CRLF" ||
        terminate == "\r\n")
        modyfiedCommand += "\r\n";
    if (terminate == "LFCR" ||
        terminate == "\n\r")
        modyfiedCommand += "\n\r";
    if (terminate == "")
        modyfiedCommand = modyfiedCommand.trimmed();


    this_serial->write(modyfiedCommand.toLocal8Bit());
    if (this_serial->waitForBytesWritten(writeTimeout)) {
        if (wait_for_response) {
            if (this_serial->waitForReadyRead(readTimeout)) {
                QByteArray responseData = this_serial->readAll();
                while (this_serial->waitForReadyRead(readWaitTimeout))
                    responseData += this_serial->readAll();
                response = QString(responseData).trimmed();

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

bool SerialPort::sendCommand(QSerialPort* const this_serial, const QString &command, const QString &terminate) const
{
    QString response;

    return send(this_serial, command, response, false, terminate);
}

bool SerialPort::sendQuery(QSerialPort* const this_serial, const QString &command, QString &response, const QString &terminate) const
{
    return send(this_serial, command, response, true, terminate);
}

QString SerialPort::ask(QSerialPort* const this_serial, const QString &command, const QString &terminate) const
{
    QString response;
    sendQuery(this_serial, command, response, terminate);

    return response;
}
