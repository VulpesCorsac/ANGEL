#include "TemperatureController.h"

TemperatureController::TemperatureController(QObject *parent ) : QObject(parent)
{
    toNULL();
}

TemperatureController::TemperatureController(const QString &portName, const int &baudrate, const int &stopBit, const int &parity, const QString &new_TemperatureControllerModel)
{
    toNULL();
    setConnection(portName, baudrate, stopBit, parity, new_TemperatureControllerModel);
}

TemperatureController::TemperatureController(QSerialPort *serial, const QString &new_temperatureControllerModel)
{
    toNULL();
    QString _temperatureControllerModel = new_temperatureControllerModel.trimmed().toUpper();
    if (_temperatureControllerModel == "PTC10") {
        if (this->ptc10 == nullptr)
            this->ptc10 = new PTC10(serial->portName(), serial->baudRate());
        else
            this->ptc10->setSerial(serial);
    }
}

TemperatureController::TemperatureController(PTC10* new_PTC10)
{
    toNULL();
    setPTC10(new_PTC10);
}

TemperatureController::~TemperatureController()
{
    if (this->ptc10 != nullptr)
        this->ptc10->~PTC10();
    delete this->ptc10;
    if (this->srs != nullptr)
        this->srs->~SRS();
    delete this->srs;

    this->ptc10 = nullptr;
    this->srs = nullptr;
}

void TemperatureController::toNULL()
{
    if (this->ptc10 != nullptr) {
        this->ptc10 = nullptr;
    }
    if (this->srs != nullptr) {
        this->srs = nullptr;
    }

    init();

    return;
}

void TemperatureController::init()
{
    if (this->srs == nullptr)
        this->srs = new SRS();

    if (this->srs != nullptr) {
        connect(this->srs,   SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString)),    Qt::DirectConnection);
        connect(this->srs,   SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->srs,   SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)),  Qt::DirectConnection);
        connect(this->srs,   SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)),  Qt::DirectConnection);
    }
    if (this->ptc10 != nullptr) {
        connect(this->ptc10, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString)),    Qt::DirectConnection);
        connect(this->ptc10, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->ptc10, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)),  Qt::DirectConnection);
        connect(this->ptc10, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)),  Qt::DirectConnection);
    }

    return;
}

void TemperatureController::wait(const double &x) const
{
    QTest::qWait(_round(this->waitTime*x));

    return;
}

void TemperatureController::setPTC10(PTC10* new_ptc10)
{

}

PTC10* TemperatureController::getPTC10() const
{

}

bool TemperatureController::openSerial(QSerialPort *serial) const
{

}

QStringList TemperatureController::getSupportedModels() const
{

}

void TemperatureController::setTemperatureControllerModel(const QString &new_temperatureControllerModel)
{

}

QString TemperatureController::getTemperatureControllerModel() const
{

}

bool TemperatureController::autoSetTemperatureControllerModel(QSerialPort *serial)
{

}

bool TemperatureController::autoSetTemperatureControllerModel(const QString &portName, const int &baudrate)
{

}

bool TemperatureController::autoSetTemperatureControllerModel(const QString &portName)
{

}

bool TemperatureController::setConnection(const QString &portName, const int &baudrate, const int &stopBit, const int &parity, const QString &new_TemperatureControllerModel)
{

}

bool TemperatureController::setConnectionBrute(const QString &portName, const QString &new_temperatureControllerModel)
{

}

bool TemperatureController::disconnect() const
{

}

QStringList TemperatureController::getBaudrateList() const
{

}

bool TemperatureController::isValidBaudrateAll(const int &baudrate) const
{

}

bool TemperatureController::isValidBaudrate(const int &baudrate) const
{

}

bool TemperatureController::changeBaudrate(const int &baudrate)
{

}

void TemperatureController::setWriteTimeout(const int &new_writeTimeout)
{

}

int TemperatureController::getWriteTimeout() const
{

}

void TemperatureController::setReadTimeout(const int &new_readTimeout)
{

}

int TemperatureController::getReadTimeout() const
{

}

void TemperatureController::TemperatureController::setReadWaitTimeout(const int &new_readWaitTimeout)
{

}

int TemperatureController::TemperatureController::getReadWaitTimeout() const
{

}

void TemperatureController::setAttemptsToConnect(const int &new_attemptsToConnect)
{

}

int TemperatureController::getAttemptsToConnect() const
{

}

bool TemperatureController::isActive() const
{

}

bool TemperatureController::send(const QString &command, QString &response, const bool &waitForResponse) const
{

}

bool TemperatureController::sendCommand(const QString &command) const
{

}

bool TemperatureController::sendQuery(const QString &command, QString &response) const
{

}

QString TemperatureController::ask(const QString &command) const
{

}

bool TemperatureController::getIDN(QString &idn) const
{

}

bool TemperatureController::test() const
{

}

double TemperatureController::getAverageInputTime() const
{

}

double TemperatureController::getAverageOutputTime() const
{

}

void TemperatureController::catchErrorSignal(const QString &s) const
{
    emit this->errorSignal(s);
}

void TemperatureController::catchResponseSignal(const QString &s) const
{
    emit this->responseSignal(s);
}

void TemperatureController::catchCommandSignal(const QString &s) const
{
    emit this->commandSignal(s);
}

void TemperatureController::catchTimeoutSignal(const QString &s) const
{
    emit this->timeoutSignal(s);
}
