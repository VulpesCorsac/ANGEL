#include "SMSD_4_2.h"

SMSD_4_2::SMSD_4_2()
{
    this->active = false;
    this->serial = nullptr;
}

SMSD_4_2::SMSD_4_2(const QString &portName, int new_waitTimeout)
{
    this->active = false;
    this->serial = nullptr;
    this->setConnection(portName, new_waitTimeout);
    this->setDefault();
}

SMSD_4_2::~SMSD_4_2()
{
    if (this->isActive())
        this->deactivate();

    if (this->serial != nullptr) {
        this->disconnect();
        this->serial->~QSerialPort();
    }
}

bool SMSD_4_2::activate()
{
    if (this->isActive())
        return true;

    QString command = "EN*";
    QString response;
    this->sendCommand(command, response);
    if (response == "EN*E10*") {
        this->active = true;
        this->setDefault();
        return true;
    } else
        return false;
}

bool SMSD_4_2::deactivate()
{
    QString command = "DS*";
    QString response;
    this->sendCommand(command, response);
    if (response == "DS*E10*") {
        this->active = false;
        return true;
    } else
        return false;
}

bool SMSD_4_2::isActive() const
{
    return this->active;
}

QSerialPort *SMSD_4_2::getSerial() const
{
    return this->serial;
}

bool SMSD_4_2::setConnection(const QString &portName, int new_waitTimeout)
{
    if (this->isActive())
        return false;

    this->waitTimeout = new_waitTimeout;

    if (this->serial == nullptr)
        this->serial = new QSerialPort();
    this->serial->setPortName(portName);
    this->serial->setBaudRate(QSerialPort::Baud9600);
    this->serial->setParity(QSerialPort::EvenParity);
    this->serial->setDataBits(QSerialPort::Data8);
    this->serial->setStopBits(QSerialPort::OneStop);

    if(!this->serial->open(QIODevice::ReadWrite)) {
        QSerialPort::SerialPortError getError;
        this->serial->error(getError);

        emit this->error(tr("Can't open %1, error code %2").arg(portName).arg(serial->error()));

        return false;
    } else {
        if(this->activate())
            return true;
        else
            return false;
    }
}

bool SMSD_4_2::disconnect()
{
    if (!this->isActive())
        return false;

    if (this->deactivate()) {
        this->serial->close();

        return true;
    } else
        return false;

}

void SMSD_4_2::sendCommand(const QString &command, QString &response) const
{
    emit this->command(command);

    if (!this->isActive() && !command.startsWith("EN*"))
        return;

    serial->write(command.toLocal8Bit());

    if (serial->waitForBytesWritten(waitTimeout)) {
        if (serial->waitForReadyRead(waitTimeout)) {
            QByteArray responseData = serial->readAll();
            while (serial->waitForReadyRead(50))
                responseData += serial->readAll();
            response = QString(responseData);

            emit this->response(response);
        }
    } else
        emit this->timeout(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
}

bool SMSD_4_2::setStepDenominator(const int &new_stepDenominator)
{
    if (new_stepDenominator == 1 || new_stepDenominator == 2 ||
        new_stepDenominator == 4 || new_stepDenominator == 16) {
        this->stepDenominator = new_stepDenominator;

        return true;
    } else
        return false;
}

int SMSD_4_2::getStepDenominator() const
{
    return this->stepDenominator;
}

bool SMSD_4_2::setDefault()
{
    bool result = true;
    result &= this->setSpeed(200);
    result &= this->setAcceleration(0);
    result &= this->setMovementDirection(false);

    return result;
}

bool SMSD_4_2::setSpeed(const int &new_speed)
{
    if (!this->isActive())
        return false;

    if (new_speed > 0 && new_speed <= 10000) {
        QString command = "SD" + QString::number(new_speed) + "*";
        QString response;
        this->sendCommand(command, response);
        if (response == command + "E10*") {
            this->speed = new_speed;

            return true;
        } else
            return false;
    } else
        return false;
}

int SMSD_4_2::getSpeed() const
{
    return this->speed;
}

bool SMSD_4_2::setMovementDirection(const bool &isRightMovementDirection)
{
    if (!this->isActive())
        return false;

    if (isRightMovementDirection) {
        QString command = "DR*";
        QString response;
        this->sendCommand(command, response);
        if (response == "DR*E10*") {
            this->rightMovementDirection = isRightMovementDirection;

            return true;
        } else
            return false;
    } else {
        QString command = "DL*";
        QString response;
        this->sendCommand(command, response);
        if (response == "DL*E10*") {
            this->rightMovementDirection = isRightMovementDirection;

            return true;
        } else
            return false;
    }
}

bool SMSD_4_2::isRightMovementDirecton() const
{
    return this->rightMovementDirection;
}

bool SMSD_4_2::setAcceleration(const int &new_acceleration)
{
    if (!this->isActive()) {
        return false;
    }

    if (new_acceleration >= -1000 && new_acceleration <= 1000) {
        QString command = "AL" + QString::number(new_acceleration) + "*";
        QString response;
        this->sendCommand(command, response);
        if (response == command + "E10*") {
            this->acceleration = new_acceleration;

            return true;
        } else
            return false;
    } else
        return false;
}

int SMSD_4_2::getAcceleration() const
{
    return this->acceleration;
}

bool SMSD_4_2::setStartingSpeed(const int &new_startingSpeed)
{
    if (!this->isActive())
        return false;

    if (new_startingSpeed > 0 && new_startingSpeed <= 2000) {
        QString command = "SS" + QString::number(new_startingSpeed) + "*";
        QString response;
        this->sendCommand(command, response);
        if (response == command + "E10*") {
            this->startingSpeed = new_startingSpeed;

            return true;
        } else
            return false;
    } else
        return false;
}

int SMSD_4_2::getStartingSpeed() const
{
    return this->startingSpeed;
}

bool SMSD_4_2::setReverse()
{
    if (!this->isActive())
        return false;

    QString command = "RS*";
    QString response;
    this->sendCommand(command, response);
    if (response == "RS*E10*")
        return true;
    else
        return false;
}

bool SMSD_4_2::setPause(const int &pause) const
{
    if (!this->isActive())
        return false;

    if (pause > 0 && pause <= 10000000) {
        QString command = "SP" + QString::number(pause) + "*";
        QString response;
        this->sendCommand(command, response);
        if (response == command + "E10*")
            return true;
        else
            return false;
    } else
        return false;
}

bool SMSD_4_2::moveUntillZero() const
{
    if (!this->isActive())
        return false;

    QString command = "HM*";
    QString response;
    this->sendCommand(command, response);
    if (response == "HM*E10*")
        return true;
    return false;
}

bool SMSD_4_2::movement() const
{
    if (!this->isActive())
        return false;

    QString command = "MV*";
    QString response;
    this->sendCommand(command, response);
    if (response == "MV*E10*")
        return true;
    return false;
}

bool SMSD_4_2::moveStep(const int &new_speed, const bool &new_isRightMovementDirection, const int &stepAmmount)
{
    if (!this->isActive()) {
        qDebug() << "Step notor is not active";

        return false;
    }

    this->setSpeed(new_speed);
    this->setMovementDirection(new_isRightMovementDirection);
    QString command = "MV" + QString::number(stepAmmount) + "*";
    QString response;
    this->sendCommand(command, response);

    qDebug() << "Command: " << command;
    qDebug() << "Response: " << response;

    if (response == command + "E10*") {
        qDebug() << "All is OK!";

        return true;
    }
    return false;
}

bool SMSD_4_2::moveStep(const int &stepAmmount)
{
    if (moveStep(this->speed, this->rightMovementDirection, stepAmmount))
        return true;
    return false;
}

bool SMSD_4_2::relayEnable() const
{
    if (!this->isActive())
        return false;

    QString command = "SF*";
    QString response;
    this->sendCommand(command, response);
    if (response == "SF*E10*")
        return true;
    return false;
}

bool SMSD_4_2::relayDisable() const
{
    if (!this->isActive())
        return false;

    QString command = "CF*";
    QString response;
    this->sendCommand(command, response);
    if (response == "CF*E10*")
        return true;
    return false;
}

bool SMSD_4_2::moveUntillHigh() const
{
    if (!this->isActive())
        return false;

    QString command = "MH*";
    QString response;
    this->sendCommand(command, response);
    if (response == "MH*E10*")
        return true;
    return false;
}

bool SMSD_4_2::moveUntillLow() const
{
    if (!this->isActive())
        return false;

    QString command = "ML*";
    QString response;
    this->sendCommand(command, response);
    if (response == "ML*E10*")
        return true;
    return false;
}

bool SMSD_4_2::waitUntillHigh() const
{
    if (!this->isActive())
        return false;

    QString command = "WH*";
    QString response;
    this->sendCommand(command, response);
    if (response == "WH*E10*")
        return true;
    return false;
}

bool SMSD_4_2::waitUntillLow() const
{
    if (!this->isActive())
        return false;

    QString command = "WL*";
    QString response;
    this->sendCommand(command, response);
    if (response == "WL*E10*")
        return true;
    return false;
}
