#include "PTC10.h"

PTC10::PTC10()
{
    this->serial = nullptr;
    this->srs = nullptr;

    init();
}

PTC10::PTC10(const QString &portName, const int &baudrate, const int &stopBit, const int &parity)
{
    this->serial = nullptr;
    this->srs = nullptr;

    init();

    setConnection(portName, baudrate, stopBit, parity);
}

PTC10::~PTC10()
{
    if (this->serial->isOpen())
        this->serial->close();
    this->serial->~QSerialPort();
    delete this->serial;
    this->serial = nullptr;

    this->srs->~SRS();
    delete this->srs;
    this->srs = nullptr;

    this->baudrateList.clear();
}

void PTC10::init()
{
    if (this->srs == nullptr)
        this->srs = new SRS();

    connect(this->srs, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString))   , Qt::DirectConnection);
    connect(this->srs, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
    connect(this->srs, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)) , Qt::DirectConnection);
    connect(this->srs, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)) , Qt::DirectConnection);

    return;
}

void PTC10::setSerial(QSerialPort *new_serial)
{
    return this->srs->setSerial(&this->serial, new_serial);
}

QSerialPort* PTC10::getSerial() const
{
    return this->srs->getSerial(this->serial);
}

bool PTC10::setConnection(const QString &portName, const int &baudrate, const int &stopBit, const int &parity)
{
    return this->srs->setConnection(&this->serial, portName, baudrate, stopBit, parity);
}

void PTC10::disconnect() const
{
    return this->srs->disconnect(this->serial);
}

void PTC10::initBaudrateList()
{
    this->baudrateList.clear();

    this->baudrateList.push_back("2400");
    this->baudrateList.push_back("4800");
    this->baudrateList.push_back("9600");
    this->baudrateList.push_back("19200");
    this->baudrateList.push_back("38400");
    this->baudrateList.push_back("115200");

    return;
}

QStringList PTC10::getBaudrateList()
{
    if (this->baudrateList.isEmpty())
        initBaudrateList();

    return this->baudrateList;
}

bool PTC10::isValidBaudrate(const int &baudrate) const
{
    return isValidString(this->baudrateList, QString::number(baudrate));
}

bool PTC10::changeBaudrate(const int &baudrate)
{
    if (!isValidBaudrate(baudrate))
        return false;

    if (!this->serial->isOpen()) {
        this->serial->setBaudRate((qint32) baudrate);
        return true;
    } else {
        this->serial->close();
        return setConnection(this->serial->portName(), baudrate);
    }
}

void PTC10::setWriteTimeout(const int &new_writeTimeout)
{
    return this->srs->setWriteTimeout(new_writeTimeout);
}

int PTC10::getWriteTimeout() const
{
    return this->srs->getWriteTimeout();
}

void PTC10::setReadTimeout(const int &new_readTimeout)
{
    return this->srs->setReadTimeout(new_readTimeout);
}

int PTC10::getReadTimeout() const
{
    return this->srs->getReadTimeout();
}

void PTC10::setReadWaitTimeout(const int &new_readWaitTimeout)
{
    return this->srs->setReadWaitTimeout(new_readWaitTimeout);
}

int PTC10::getReadWaitTimeout() const
{
    return this->srs->getReadWaitTimeout();
}

void PTC10::setAttemptsToConnect(const int &new_attemptsToConnect)
{
    return this->setAttemptsToConnect(new_attemptsToConnect);
}

int PTC10::getAttemptsToConnect() const
{
    return this->getAttemptsToConnect();
}

bool PTC10::isActive() const
{
    return this->srs->isActive(this->serial);
}

bool PTC10::send(const QString &command, QString &response, const bool &wait_for_response) const
{
    return this->srs->send(this->serial, command, response, wait_for_response);
}

bool PTC10::sendCommand(const QString &command) const
{
    return this->srs->sendCommand(this->serial, command);
}

bool PTC10::sendQuery(const QString &command, QString &response) const
{
    return this->srs->sendQuery(this->serial, command, response);
}

QString PTC10::ask(const QString &command) const
{
    return this->srs->ask(this->serial, command);
}

bool PTC10::getIDN(QString &idn) const
{
    return this->srs->getIDN(this->serial, idn);
}

int PTC10::numberFromString(const QStringList &list, const QString &string) const
{
    return this->srs->numberFromString(list, string);
}

QString PTC10::stringFromNumber(const QStringList &list, const int &number) const
{
    return this->srs->stringFromNumber(list, number);
}

bool PTC10::isValidString(const QStringList &list, const QString &string) const
{
    return this->srs->isValidString(list, string);
}

bool PTC10::isValidNumber(const QStringList &list, const int &number) const
{
    return this->srs->isValidNumber(list, number);
}

void PTC10::catchErrorSignal(const QString &s) const
{
    emit this->errorSignal(s);
}

void PTC10::catchResponseSignal(const QString &s) const
{
    emit this->responseSignal(s);
}

void PTC10::catchCommandSignal(const QString &s) const
{
    emit this->commandSignal(s);
}

void PTC10::catchTimeoutSignal(const QString &s) const
{
    emit this->timeoutSignal(s);
}

bool PTC10::isOldModel() const
{
    return this->oldModel;
}

void PTC10::setModel(const bool &new_oldModel)
{
    this->oldModel = new_oldModel;

    return;
}

void PTC10::fillChannels()
{
    this->channels = ask("getOutput.Names").split(",");

    return;
}

QStringList PTC10::getChannels()
{
    if (this->channels.isEmpty())
        fillChannels();

    return this->channels;
}

void PTC10::fillUnits()
{
    this->units = ask("getOutput.Units").split(",");

    return;
}

QStringList PTC10::getUnits()
{
    if (this->units.isEmpty())
        fillUnits();

    return this->units;
}

QString PTC10::getUnitAt(const int &i) const
{
    if (i < this->units.size())
        return this->units[i];

    return "";
}

void PTC10::setOutputEnabled(const bool &new_outputEnabled) const
{

}

bool PTC10::getOutputEnabled() const
{

}

void PTC10::setPower(const double &new_power) const
{

}

double PTC10::getPower() const
{

}

void PTC10::setPIDChannel(const QString &channel) const
{

}

QString PTC10::getPIDChannel() const
{

}

void PTC10::setPIDTemperature(const double &new_temperature) const
{

}

double PTC10::getPIDTemperature() const
{

}

void PTC10::setPIDCoefficientP(const double &new_P) const
{

}

double PTC10::getPIDCoefficientP() const
{

}

void PTC10::setPIDCoefficientI(const double &new_I) const
{

}

double PTC10::getPIDCoefficientI() const
{

}

void PTC10::setPIDCoefficientD(const double &new_D) const
{

}

double PTC10::getPIDCoefficientD() const
{

}

void PTC10::setPIDCoefficientsPID(const double &new_P, const double &new_I, const double &new_D) const
{

}

void PTC10::setPIDAll(const QString &channel, const double &new_temperature, const double &new_P, const double &new_I, const double &new_D) const
{

}
