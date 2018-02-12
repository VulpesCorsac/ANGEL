#include "SR844.h"

SR844::SR844(QObject *parent) : QObject(parent)
{

}

SR844::SR844(const QString &portName, const int &new_baudrate)
{

}

SR844::~SR844()
{

}

void SR844::setSerial(QSerialPort* new_serial)
{

}

QSerialPort* SR844::getSerial() const
{

}

bool SR844::setConnection(const QString &portName, const int &new_baudrate)
{

}

void SR844::disconnect() const
{

}

bool SR844::isValidBaudrate(const int &new_baudrate) const
{

}

bool SR844::changeBaudrate(const int &new_baudrate)
{

}

QStringList SR844::validBaudrates() const
{

}

bool SR844::isActive() const
{

}

bool SR844::send(const QString &command, QString &response, const bool &wait_for_response) const
{

}

bool SR844::sendCommand(const QString &command) const
{

}

bool SR844::sendQuery(const QString &command, QString &response) const
{

}

bool SR844::getIDN(QString &idn) const
{

}
