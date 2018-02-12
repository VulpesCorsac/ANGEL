#include "SRS.h"

SRS::SRS(QObject *parent) : QObject(parent)
{

}

SRS::~SRS()
{

}

QSerialPort* SRS::getSerial (QSerialPort*  const this_serial) const
{
    return this_serial;
}

void SRS::setSerial(QSerialPort* *this_serial, QSerialPort* const new_serial) const
{
    *this_serial = new_serial;
    return;
}

bool SRS::openSerial(QSerialPort* const this_serial) const
{
    if (!this_serial->open(QIODevice::ReadWrite)) {
        QSerialPort::SerialPortError error;
        this_serial->error(error);

        emit this->errorSignal(tr("Can't open %1, error code %2").arg(this_serial->portName()).arg(this_serial->error()));

        return false;
    } else {
        return true;
    }
}

bool SRS::setConnection(QSerialPort* *this_serial, const QString &portName, const int &new_baudrate) const
{
    if (isActive(*this_serial))
        return true;

    if (*this_serial == nullptr)
        *this_serial = new QSerialPort();

    (*this_serial)->setPortName(portName);
    (*this_serial)->setBaudRate((qint32) new_baudrate);

    (*this_serial)->setParity(QSerialPort::NoParity);
    (*this_serial)->setDataBits(QSerialPort::Data8);
    (*this_serial)->setStopBits(QSerialPort::TwoStop);

    if (!(*this_serial)->open(QIODevice::ReadWrite)) {
        QSerialPort::SerialPortError error;
        (*this_serial)->error(error);

        emit this->errorSignal(tr("Can't open %1, error code %2").arg(portName).arg((*this_serial)->error()));

        return false;
    } else {
        return isActive(*this_serial);
    }
}

void SRS::disconnect(QSerialPort* const this_serial) const
{
    if (this_serial != nullptr)
        if (this_serial->isOpen())
            this_serial->close();
    return;
}

bool SRS::send(QSerialPort* const this_serial, const QString &command, QString &response, const bool &wait_for_response) const
{
    emit this->commandSignal(command);
    response = "";

    if (this_serial == nullptr || !this_serial->isOpen())
        return false;

    QString modyfiedCommand = command.trimmed() + "\n";

    this_serial->write(modyfiedCommand.toLocal8Bit());
    if (this_serial->waitForBytesWritten(defaultWriteTimeout)) {
        if (wait_for_response) {
            if (this_serial->waitForReadyRead(defaultReadTimeout)) {
                QByteArray responseData = this_serial->readAll();
                while (this_serial->waitForReadyRead(defaultReadWaitTimeout))
                    responseData += this_serial->readAll();
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

bool SRS::sendCommand(QSerialPort* const this_serial, const QString &command) const
{
    QString response;

    return send(this_serial, command, response, false);
}

bool SRS::sendQuery(QSerialPort* const this_serial, const QString &command, QString &response) const
{
    return send(this_serial, command, response, true);
}

QString SRS::ask(QSerialPort* const this_serial, const QString &command) const
{
    QString response;
    sendQuery(this_serial, command, response);

    return response;
}

bool SRS::isActive(QSerialPort* const this_serial) const
{
    QString idn;

    return getIDN(this_serial, idn);
}

bool SRS::getIDN(QSerialPort* const this_serial, QString &idn) const
{
    return sendQuery(this_serial, "*IDN?", idn);
}

void SRS::initQStringList(QStringList &target, const QStringList &source) const
{
    target.clear();
    target = source;

    return;
}

int SRS::numberFromString(const QStringList &list, const QString &string) const
{
    return list.indexOf(string);
}

QString SRS::stringFromNumber(const QStringList &list, const int &number) const
{
    if (number >= 0 &&
        number < list.length())
        return list.at(number);
    return "";
}

bool SRS::isValidString(const QStringList &list, const QString &string) const
{
    return(numberFromString(list, string) != -1);
}

bool SRS::isValidNumber(const QStringList &list, const int &number) const
{
    return (number < 0 || number >= list.size());
}
