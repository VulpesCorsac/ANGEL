#include "DS335.h"

DS335::DS335()
{
    this->serial = nullptr;
    this->srs = nullptr;

    init();
}

DS335::DS335(const QString &portName, const int &new_baudrate)
{
    this->serial = nullptr;
    this->srs = nullptr;

    init();

    setConnection(portName, new_baudrate);
}

DS335::~DS335()
{
    if (this->serial->isOpen())
        this->serial->close();
    this->serial->~QSerialPort();

    this->srs->~SRS();

    this->baudrateList.clear();
    this->functionList.clear();
    this->outputImpedanceList.clear();
}

void DS335::init()
{
    if (this->srs == nullptr)
        this->srs = new SRS();

    initBaudrateList();
    initAmplitudeTypeList();
    initFunctionList();
    initOutputImpedanceList();

    connect(this->srs, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString))   , Qt::DirectConnection);
    connect(this->srs, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
    connect(this->srs, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)) , Qt::DirectConnection);
    connect(this->srs, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)) , Qt::DirectConnection);

    return;
}

void DS335::setSerial(QSerialPort *new_serial)
{
    return this->srs->setSerial(&this->serial, new_serial);
}

QSerialPort* DS335::getSerial() const
{
    return this->srs->getSerial(this->serial);
}

bool DS335::setConnection(const QString &portName, const int &new_baudrate)
{
    return this->srs->setConnection(&this->serial, portName, new_baudrate);
}

void DS335::disconnect() const
{
    return this->srs->disconnect(this->serial);
}

void DS335::initBaudrateList()
{
    this->baudrateList.clear();

    this->baudrateList.push_back("1200");
    this->baudrateList.push_back("2400");
    this->baudrateList.push_back("4800");
    this->baudrateList.push_back("9600");

    return;
}

QStringList DS335::getBaudrateList() const
{
    return this->baudrateList;
}

bool DS335::isValidBaudrate(const int &new_baudrate) const
{
    return isValidString(this->baudrateList, QString::number(new_baudrate));
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
    return this->srs->isActive(this->serial);
}

bool DS335::send(const QString &command, QString &response, const bool &wait_for_response) const
{
    return this->srs->send(this->serial, command, response, wait_for_response);
}

bool DS335::sendCommand(const QString &command) const
{
    return this->srs->sendCommand(this->serial, command);
}

bool DS335::sendQuery(const QString &command, QString &response) const
{
    return this->srs->sendQuery(this->serial, command, response);
}

QString DS335::ask(const QString &command) const
{
    return this->srs->ask(this->serial, command);
}

bool DS335::getIDN(QString &idn) const
{
    return this->srs->getIDN(this->serial, idn);
}

int DS335::numberFromString(const QStringList &list, const QString &string) const
{
    return this->srs->numberFromString(list, string);
}

QString DS335::stringFromNumber(const QStringList &list, const int &number) const
{
    return this->srs->stringFromNumber(list, number);
}

bool DS335::isValidString(const QStringList &list, const QString &string) const
{
    return this->srs->isValidString(list, string);
}

bool DS335::isValidNumber(const QStringList &list, const int &number) const
{
    return this->srs->isValidNumber(list, number);
}

void DS335::initAmplitudeTypeList()
{
    this->amplitudeTypeList.clear();

    this->amplitudeTypeList.push_back("VP");
    this->amplitudeTypeList.push_back("VR");

    return;
}

QStringList DS335::getAmplitudeTypeList() const
{
   return this->amplitudeTypeList;
}

double DS335::getMinAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (outputZ.toUpper() == "50OHMS") {
        if (waveform == "SINE") {
            if (VRMS)
                return this->minAmplitude50OhmsSineVrms;
            else
                return this->minAmplitude50OhmsSineVpp;
        }
        if (waveform == "SQUARE") {
            if (VRMS)
                return this->minAmplitude50OhmsSquareVrms;
            else
                return this->minAmplitude50OhmsSquareVpp;
        }
        if (waveform == "TRIANGLE") {
            if (VRMS)
                return this->minAmplitude50OhmsTriangleVrms;
            else
                return this->minAmplitude50OhmsTriangleVpp;
        }
        if (waveform == "RAMP") {
            if (VRMS)
                return this->minAmplitude50OhmsRampVrms;
            else
                return this->minAmplitude50OhmsRampVpp;
        }
        if (waveform == "NOISE") {
            if (VRMS)
                return this->minAmplitude50OhmsNoiseVrms;
            else
                return this->minAmplitude50OhmsNoiseVpp;
        }
    } else {
        if (waveform == "SINE") {
            if (VRMS)
                return this->minAmplitudeHighZSineVrms;
            else
                return this->minAmplitudeHighZSineVpp;
        }
        if (waveform == "SQUARE") {
            if (VRMS)
                return this->minAmplitudeHighZSquareVrms;
            else
                return this->minAmplitudeHighZSquareVpp;
        }
        if (waveform == "TRIANGLE") {
            if (VRMS)
                return this->minAmplitudeHighZTriangleVrms;
            else
                return this->minAmplitudeHighZTriangleVpp;
        }
        if (waveform == "RAMP") {
            if (VRMS)
                return this->minAmplitudeHighZRampVrms;
            else
                return this->minAmplitudeHighZRampVpp;
        }
        if (waveform == "NOISE") {
            if (VRMS)
                return this->minAmplitudeHighZNoiseVrms;
            else
                return this->minAmplitudeHighZNoiseVpp;
        }
    }

    return -1;
}

double DS335::getMaxAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (outputZ == "50OHMS") {
        if (waveform == "SINE") {
            if (VRMS)
                return this->maxAmplitude50OhmsSineVrms;
            else
                return this->maxAmplitude50OhmsSineVpp;
        }
        if (waveform == "SQUARE") {
            if (VRMS)
                return this->maxAmplitude50OhmsSquareVrms;
            else
                return this->maxAmplitude50OhmsSquareVpp;
        }
        if (waveform == "TRIANGLE") {
            if (VRMS)
                return this->maxAmplitude50OhmsTriangleVrms;
            else
                return this->maxAmplitude50OhmsTriangleVpp;
        }
        if (waveform == "RAMP") {
            if (VRMS)
                return this->maxAmplitude50OhmsRampVrms;
            else
                return this->maxAmplitude50OhmsRampVpp;
        }
        if (waveform == "NOISE") {
            if (VRMS)
                return this->maxAmplitude50OhmsNoiseVrms;
            else
                return this->maxAmplitude50OhmsNoiseVpp;
        }
    } else {
        if (waveform == "SINE") {
            if (VRMS)
                return this->maxAmplitudeHighZSineVrms;
            else
                return this->maxAmplitudeHighZSineVpp;
        }
        if (waveform == "SQUARE") {
            if (VRMS)
                return this->maxAmplitudeHighZSquareVrms;
            else
                return this->maxAmplitudeHighZSquareVpp;
        }
        if (waveform == "TRIANGLE") {
            if (VRMS)
                return this->maxAmplitudeHighZTriangleVrms;
            else
                return this->maxAmplitudeHighZTriangleVpp;
        }
        if (waveform == "RAMP") {
            if (VRMS)
                return this->maxAmplitudeHighZRampVrms;
            else
                return this->maxAmplitudeHighZRampVpp;
        }
        if (waveform == "NOISE") {
            if (VRMS)
                return this->maxAmplitudeHighZNoiseVrms;
            else
                return this->maxAmplitudeHighZNoiseVpp;
        }
    }

    return -1;
}

double DS335::getStepAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (outputZ == "50OHMS") {
        if (waveform == "SINE") {
            if (VRMS)
                return this->stepAmplitude50OhmsSineVrms;
            else
                return this->stepAmplitude50OhmsSineVpp;
        }
        if (waveform == "SQUARE") {
            if (VRMS)
                return this->stepAmplitude50OhmsSquareVrms;
            else
                return this->stepAmplitude50OhmsSquareVpp;
        }
        if (waveform == "TRIANGLE") {
            if (VRMS)
                return this->stepAmplitude50OhmsTriangleVrms;
            else
                return this->stepAmplitude50OhmsTriangleVpp;
        }
        if (waveform == "RAMP") {
            if (VRMS)
                return this->stepAmplitude50OhmsRampVrms;
            else
                return this->stepAmplitude50OhmsRampVpp;
        }
        if (waveform == "NOISE") {
            if (VRMS)
                return this->stepAmplitude50OhmsNoiseVrms;
            else
                return this->stepAmplitude50OhmsNoiseVpp;
        }
    } else {
        if (waveform == "SINE") {
            if (VRMS)
                return this->stepAmplitudeHighZSineVrms;
            else
                return this->stepAmplitudeHighZSineVpp;
        }
        if (waveform == "SQUARE") {
            if (VRMS)
                return this->stepAmplitudeHighZSquareVrms;
            else
                return this->stepAmplitudeHighZSquareVpp;
        }
        if (waveform == "TRIANGLE") {
            if (VRMS)
                return this->stepAmplitudeHighZTriangleVrms;
            else
                return this->stepAmplitudeHighZTriangleVpp;
        }
        if (waveform == "RAMP") {
            if (VRMS)
                return this->stepAmplitudeHighZRampVrms;
            else
                return this->stepAmplitudeHighZRampVpp;
        }
        if (waveform == "NOISE") {
            if (VRMS)
                return this->stepAmplitudeHighZNoiseVrms;
            else
                return this->stepAmplitudeHighZNoiseVpp;
        }
    }

    return -1;
}

double DS335::getDecimalsAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (outputZ == "50OHMS") {
        if (waveform == "SINE") {
            if (VRMS)
                return this->decimalsAmplitude50OhmsSineVrms;
            else
                return this->decimalsAmplitude50OhmsSineVpp;
        }
        if (waveform == "SQUARE") {
            if (VRMS)
                return this->decimalsAmplitude50OhmsSquareVrms;
            else
                return this->decimalsAmplitude50OhmsSquareVpp;
        }
        if (waveform == "TRIANGLE") {
            if (VRMS)
                return this->decimalsAmplitude50OhmsTriangleVrms;
            else
                return this->decimalsAmplitude50OhmsTriangleVpp;
        }
        if (waveform == "RAMP") {
            if (VRMS)
                return this->decimalsAmplitude50OhmsRampVrms;
            else
                return this->decimalsAmplitude50OhmsRampVpp;
        }
        if (waveform == "NOISE") {
            if (VRMS)
                return this->decimalsAmplitude50OhmsNoiseVrms;
            else
                return this->decimalsAmplitude50OhmsNoiseVpp;
        }
    } else {
        if (waveform == "SINE") {
            if (VRMS)
                return this->decimalsAmplitudeHighZSineVrms;
            else
                return this->decimalsAmplitudeHighZSineVpp;
        }
        if (waveform == "SQUARE") {
            if (VRMS)
                return this->decimalsAmplitudeHighZSquareVrms;
            else
                return this->decimalsAmplitudeHighZSquareVpp;
        }
        if (waveform == "TRIANGLE") {
            if (VRMS)
                return this->decimalsAmplitudeHighZTriangleVrms;
            else
                return this->decimalsAmplitudeHighZTriangleVpp;
        }
        if (waveform == "RAMP") {
            if (VRMS)
                return this->decimalsAmplitudeHighZRampVrms;
            else
                return this->decimalsAmplitudeHighZRampVpp;
        }
        if (waveform == "NOISE") {
            if (VRMS)
                return this->decimalsAmplitudeHighZNoiseVrms;
            else
                return this->decimalsAmplitudeHighZNoiseVpp;
        }
    }

    return -1;
}

bool DS335::isValidAmplitude(const double &new_amplitude, const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    return (getMinAmplitude(waveform, outputZ, VRMS) <= new_amplitude &&
            new_amplitude <= getMaxAmplitude(waveform, outputZ, VRMS));
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
        return response.trimmed().left(response.trimmed().length() - 2).toDouble();
}

double DS335::getMinOffset() const
{
    return this->minOffsetV;
}

double DS335::getMaxOffset() const
{
    return this->maxOffsetV;
}

double DS335::getStepOffset() const
{
    return this->stepOffsetV;
}

double DS335::getDecimalsOffset() const
{
    return this->decimalsOffsetV;
}

bool DS335::isValidOffset(const double &offset) const
{
    return (this->minOffsetV <= offset && offset <= this->maxOffsetV);
}

bool DS335::setOffset(const double &new_offset) const
{
    if (!isValidOffset(new_offset))
        return false;

    QString command = "OFFS " + QString::number(new_offset);
    return sendCommand(command);
}

bool DS335::getOffset() const
{
    return ask("OFFS?").toDouble();
}

double DS335::getMinFrequency(const QString &waveform) const
{
    if (waveform == "SINE")
        return this->minFrequencySine;
    if (waveform == "SQUARE")
        return this->minFrequencySquare;
    if (waveform == "TRIANGLE")
        return this->minFrequencyTriangle;
    if (waveform == "RAMP")
        return this->minFrequencyRamp;
    if (waveform == "NOISE")
        return this->minFrequencyNoise;

    return -1;
}

double DS335::getMaxFrequency(const QString &waveform) const
{
    if (waveform == "SINE")
        return this->maxFrequencySine;
    if (waveform == "SQUARE")
        return this->maxFrequencySquare;
    if (waveform == "TRIANGLE")
        return this->maxFrequencyTriangle;
    if (waveform == "RAMP")
        return this->maxFrequencyRamp;
    if (waveform == "NOISE")
        return this->maxFrequencyNoise;

    return -1;
}

double DS335::getStepFrequency(const QString &waveform) const
{
    if (waveform == "SINE")
        return this->stepFrequencySine;
    if (waveform == "SQUARE")
        return this->stepFrequencySquare;
    if (waveform == "TRIANGLE")
        return this->stepFrequencyTriangle;
    if (waveform == "RAMP")
        return this->stepFrequencyRamp;
    if (waveform == "NOISE")
        return this->stepFrequencyNoise;

    return -1;
}

double DS335::getDecimalsFrequency(const QString &waveform) const
{
    if (waveform == "SINE")
        return this->decimalsFrequencySine;
    if (waveform == "SQUARE")
        return this->decimalsFrequencySquare;
    if (waveform == "TRIANGLE")
        return this->decimalsFrequencyTriangle;
    if (waveform == "RAMP")
        return this->decimalsFrequencyRamp;
    if (waveform == "NOISE")
        return this->decimalsFrequencyNoise;

    return -1;
}

bool DS335::isValidFrequency(const double &frequency, const QString &waveform) const
{
    return (getMinFrequency(waveform) <= frequency && frequency <= getMaxFrequency(waveform));
}

bool DS335::setFrequency(const double &new_frequency) const
{
    QString command = "FREQ " + QString::number(new_frequency,'f',6);
    return sendCommand(command);
}

double DS335::getFrequency() const
{
    return ask("FREQ?").toDouble();
}

void DS335::initFunctionList()
{
    this->functionList.clear();

    this->functionList.push_back("SINE");
    this->functionList.push_back("SQUARE");
    this->functionList.push_back("TRIANGLE");
    this->functionList.push_back("RAMP");
    this->functionList.push_back("NOISE");

    return;
}

QStringList DS335::getFunctionList() const
{
    return this->functionList;
}

int DS335::functionNumberFromString(const QString &function_string) const
{
    return numberFromString(this->functionList, function_string);
}

QString DS335::functionStringFromNumber(const int &function_number) const
{
    return stringFromNumber(this->functionList, function_number);
}

bool DS335::setFunction(const int &new_function) const
{
    if (!isValidNumber(this->functionList, new_function))
        return false;

    QString command = "FUNC " + QString::number(new_function);
    return sendCommand(command);
}

bool DS335::setFunction(const QString &new_function) const
{
    return setFunction(functionNumberFromString(new_function));
}

QString DS335::getFunction() const
{
    return functionStringFromNumber(ask("FUNC?").toInt());
}

bool DS335::setInverse(const bool &new_inverse) const
{
    QString command = "INVT ";
    if (new_inverse)
        command += "1";
    else
        command += "0";
    return sendCommand(command);
}

bool DS335::getInverse() const
{
    return (ask("INVT?").toInt() == 1);
}

bool DS335::setSynchronization(const bool &new_synchronization) const
{
    QString command = "SYNC ";
    if (new_synchronization)
        command += "1";
    else
        command += "0";
    return sendCommand(command);
}

bool DS335::getSynchronization() const
{
    return (ask("SYNC?").toInt() == 1);
}

void DS335::initOutputImpedanceList()
{
    this->outputImpedanceList.clear();

    this->outputImpedanceList.push_back("50OHMS");
    this->outputImpedanceList.push_back("Hi-Z");

    return;
}

QStringList DS335::getOutputImpedanceList() const
{
    return this->outputImpedanceList;
}

int DS335::outputImpedanceNumberFromString(const QString &outputImpedance_string) const
{
    return numberFromString(this->outputImpedanceList, outputImpedance_string);
}

QString DS335::outputImpedanceStringFromNumber(const int &outputImpedance_number) const
{
    return stringFromNumber(this->outputImpedanceList, outputImpedance_number);
}

bool DS335::setOutputImpedance(const int &new_outputImpedance) const
{
    if (!isValidNumber(this->outputImpedanceList, new_outputImpedance))
        return false;

    QString command = "TERM " + QString::number(new_outputImpedance);
    return sendCommand(command);
}

bool DS335::setOutputImpedance(const QString &new_outputImpedance) const
{
    return setOutputImpedance(outputImpedanceNumberFromString(new_outputImpedance));
}

QString DS335::getOutputImpedance() const
{
    return outputImpedanceStringFromNumber(ask("TERM?").toInt());
}

void DS335::catchErrorSignal(const QString &s) const
{
    emit this->errorSignal(s);
}

void DS335::catchResponseSignal(const QString &s) const
{
    emit this->responseSignal(s);
}

void DS335::catchCommandSignal(const QString &s) const
{
    emit this->commandSignal(s);
}

void DS335::catchTimeoutSignal(const QString &s) const
{
    emit this->timeoutSignal(s);
}
