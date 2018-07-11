#include "SRS.h"

SRS::SRS(QObject *parent) : QObject(parent)
{
    toNull();

    this->supportedList.push_back("DS335");
    this->supportedList.push_back("DS345");

    this->supportedList.push_back("SR830");
    this->supportedList.push_back("SR844");
    this->supportedList.push_back("SR865");

    this->supportedList.push_back("PTC10");
}

SRS::~SRS()
{
    this->supportedList.clear();

    if (this->serialCommunication != nullptr) {
        this->serialCommunication->~SerialPort();
    }
    this->serialCommunication = nullptr;
}

void SRS::toNull()
{
    if (this->serialCommunication != nullptr)
        this->serialCommunication = nullptr;

    init();

    return;
}

void SRS::init()
{
    if (this->serialCommunication == nullptr)
        this->serialCommunication = new SerialPort();

    if (this->serialCommunication != nullptr) {
        connect(this->serialCommunication, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString)),    Qt::DirectConnection);
        connect(this->serialCommunication, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->serialCommunication, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)),  Qt::DirectConnection);
        connect(this->serialCommunication, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)),  Qt::DirectConnection);
    }
}

QStringList SRS::getSupportedList() const
{
    return this->supportedList;
}

QStringList SRS::getBaudrateList() const
{
    return this->serialCommunication->getBaudrateList();
}

void SRS::setWriteTimeout(const int &new_writeTimeout)
{
    this->serialCommunication->setWriteTimeout(new_writeTimeout);

    return;
}

int SRS::getWriteTimeout() const
{
    return this->serialCommunication->getWriteTimeout();
}

void SRS::setReadTimeout(const int &new_readTimeout)
{
    this->serialCommunication->setReadTimeout(new_readTimeout);

    return;
}

int SRS::getReadTimeout() const
{
    return this->serialCommunication->getReadTimeout();
}

void SRS::setReadWaitTimeout(const int &new_readWaitTimeout)
{
    this->serialCommunication->setReadWaitTimeout(new_readWaitTimeout);

    return;
}

int SRS::getReadWaitTimeout() const
{
    return this->serialCommunication->getReadWaitTimeout();
}

void SRS::setAttemptsToConnect(const int &new_attemptsToConnect)
{
    this->attemptsToConnect = new_attemptsToConnect;

    return;
}

int SRS::getAttemptsToConnect() const
{
    return this->attemptsToConnect;
}

QSerialPort* SRS::getSerial (QSerialPort* const this_serial) const
{
    return this->serialCommunication->getSerial(this_serial);
}

void SRS::setSerial(QSerialPort **this_serial, QSerialPort* const new_serial) const
{
    *this_serial = new_serial;

    return;
}

bool SRS::openSerial(QSerialPort* const this_serial) const
{
    return this->serialCommunication->openSerial(this_serial);
}

bool SRS::setConnection(QSerialPort* *this_serial, const QString &portName, const int &baudrate, const int &stopbit, const int &parity, const QString flowControl) const
{
    if (isActive(*this_serial))
        return true;

     bool ans = this->serialCommunication->setConnection(this_serial, portName, baudrate, 8, stopbit, parity, flowControl);

     return ans && isActive(*this_serial);
}

void SRS::disconnect(QSerialPort* const this_serial) const
{
    return this->serialCommunication->disconnect(this_serial);
}

QString SRS::autoDetect(QSerialPort* const this_serial) const
{
    QString idn;

    if (!this_serial->isOpen()) {
        if (!this->openSerial(this_serial))
            return "";
    }

    if (!this->getIDN(this_serial, idn)) {
        return "";
    }

//    this_serial->close(); // ЗАЧЕМ? И ПОЧЕМУ ТАК РАБОТАЕТ?!?!?!
    foreach (QString equipment, this->supportedList) {
        if (idn.contains(equipment))
            return equipment;
    }

    return "";
}

QString SRS::autoDetect(const QString &portName, const int &baudrate, const int &stopbit, const int &parity, const QString flowControl) const
{
    QSerialPort *serial = new QSerialPort();

    serial->setPortName(portName);
    serial->setBaudRate(baudrate);

    if (stopbit == 1)
        serial->setStopBits(QSerialPort::OneStop);
    if (stopbit == 2)
        serial->setStopBits(QSerialPort::TwoStop);
    if (stopbit == 3)
        serial->setStopBits(QSerialPort::OneAndHalfStop);

    if (parity == 0)
        serial->setParity(QSerialPort::NoParity);
    if (parity == 1)
        serial->setParity(QSerialPort::OddParity);
    if (parity == 2)
        serial->setParity(QSerialPort::EvenParity);


    if (flowControl.toUpper() == "" ||
        flowControl.toUpper() == "NONE")
        serial->setFlowControl(QSerialPort::NoFlowControl);
    if (flowControl.toUpper() == "HARD" ||
        flowControl.toUpper() == "HARDWARE")
        serial->setFlowControl(QSerialPort::HardwareControl);
    if (flowControl.toUpper() == "SOFT" ||
        flowControl.toUpper() == "SOFTWARE")
        serial->setFlowControl(QSerialPort::SoftwareControl);

    return autoDetect(serial);
}

QString SRS::autoDetect(const QString &portName) const
{
    QStringList baudrateList = this->serialCommunication->getBaudrateList();
    QStringList flowControlList;
    flowControlList.append("NONE");
    flowControlList.append("HARD");
    flowControlList.append("SOFT");
    int readTimeout = this->serialCommunication->getReadTimeout();

    for (int stopbit = 1; stopbit <= 3; stopbit++)
        for (int parity = 0; parity <= 2; parity++)
            for (int baud = 0; baud < baudrateList.length(); baud++) {
                for (int flow = 0; flow < flowControlList.length(); flow++) {
                    for (int attempt = 0; attempt < this->attemptsToConnect; attempt++) {
                        QString response = autoDetect(portName, baudrateList[baud].toInt(), stopbit, parity, flowControlList[flow]);
                        if (!response.trimmed().isEmpty())
                            return response;
                        QTest::qWait(20*readTimeout);
                    }
                }
            }
    return "";
}

bool SRS::send(QSerialPort* const this_serial, const QString &command, QString &response, const bool &wait_for_response) const
{
    return this->serialCommunication->send(this_serial, command, response, wait_for_response, "\n");
}

bool SRS::sendCommand(QSerialPort* const this_serial, const QString &command) const
{
    QString response;

    return this->serialCommunication->send(this_serial, command, response, false, "\n");
}

bool SRS::sendQuery(QSerialPort* const this_serial, const QString &command, QString &response) const
{
    return this->serialCommunication->send(this_serial, command, response, true, "\n");
}

QString SRS::ask(QSerialPort* const this_serial, const QString &command) const
{
    QString response;
    this->serialCommunication->sendQuery(this_serial, command, response, "\n");

    return response;
}

bool SRS::isActive(QSerialPort* const this_serial) const
{
    QString idn;

    return getIDN(this_serial, idn);
}

bool SRS::getIDN(QSerialPort* const this_serial, QString &idn) const
{
    for (int attempt = 0; attempt < this->attemptsToConnect; attempt++) {
        sendQuery(this_serial, "*IDN?", idn);
        if (!idn.trimmed().isEmpty())
            return true;
    }
    return false;
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
    if (isValidNumber(list, number))
        return list.at(number);
    return "";
}

bool SRS::isValidString(const QStringList &list, const QString &string) const
{
    return(numberFromString(list, string) != -1);
}

bool SRS::isValidNumber(const QStringList &list, const int &number) const
{
    return (number >= 0 && number < list.size());
}

void SRS::catchErrorSignal(const QString &s) const
{
    emit this->errorSignal(s);
}

void SRS::catchResponseSignal(const QString &s) const
{
    emit this->responseSignal(s);
}

void SRS::catchCommandSignal(const QString &s) const
{
    emit this->commandSignal(s);
}

void SRS::catchTimeoutSignal(const QString &s) const
{
    emit this->timeoutSignal(s);
}
