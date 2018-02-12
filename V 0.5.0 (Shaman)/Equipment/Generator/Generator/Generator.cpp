#include "Generator.h"

Generator::Generator(QObject *parent) : QObject(parent)
{
    this->ds335 = nullptr;
    this->ds345 = nullptr;
    this->generatorType = "";
}

Generator::Generator(const QString &portName, const int &new_baudrate, const QString &new_generatorType)
{
    setConnection(portName, new_baudrate, new_generatorType);
}

Generator::Generator(QSerialPort *serial, const QString &new_generatorType)
{
    if (new_generatorType.trimmed().toUpper() == "DS335") {
        if (this->ds335 == nullptr)
            this->ds335 = new DS335(serial->portName(), serial->baudRate());
        else
            this->ds335->setSerial(serial);
    }
    if (new_generatorType.trimmed().toUpper() == "DS345") {
        if (this->ds345 == nullptr)
            this->ds345 = new DS345(serial->portName(), serial->baudRate());
        else
            this->ds345->setSerial(serial);
    }
    if (new_generatorType.trimmed().isEmpty() ||
        new_generatorType.trimmed().toUpper() == "AUTO")
        autoSetGeneratorType(serial);
}

Generator::Generator(DS335 *new_ds335)
{
    setDS335(new_ds335);
}

Generator::Generator(DS345 *new_ds345)
{
    setDS345(new_ds345);
}

Generator::~Generator()
{
    this->generatorType = "";
    if (this->ds335 != nullptr)
        this->ds335->~DS335();
    if (this->ds345 != nullptr)
        this->ds345->~DS345();
    this->ds335 = nullptr;
    this->ds345 = nullptr;
}

void Generator::setDS335(DS335 *new_ds335)
{
    this->ds335 = new_ds335;
}

DS335* Generator::getDS335() const
{
    return this->ds335;
}

void Generator::setDS345(DS345 *new_ds345)
{
    this->ds345 = new_ds345;
}

DS345* Generator::getDS345() const
{
    return this->ds345;
}

bool Generator::openSerial(QSerialPort *serial) const
{
    if (!serial->open(QIODevice::ReadWrite)) {
        QSerialPort::SerialPortError error;
        serial->error(error);

        emit this->errorSignal(tr("Can't open %1, error code %2").arg(serial->portName()).arg(serial->error()));

        return false;
    } else {
        return true;
    }
}

void Generator::setGeneratorType(const QString &new_generatorType)
{
    this->generatorType = new_generatorType;
}

QString Generator::getGeneratorType() const
{
    return this->generatorType;
}

bool Generator::autoSetGeneratorType(QSerialPort *serial)
{
//    QString portName = serial->portName();
//    qint32 baudRate = serial->baudRate();

    DS335 temp(serial->portName(), serial->baudRate());
    QString idn;
    temp.getIDN(idn);
    temp.disconnect();
    if (idn.trimmed().startsWith("StanfordResearchSystems,DS335")) {
        if (this->ds335 == nullptr)
            this->ds335 = new DS335();
        this->generatorType = "DS335";
        this->openSerial(serial);
        this->ds335->setSerial(serial);
        return true;
    }
    if (idn.trimmed().startsWith("StanfordResearchSystems,DS345")) {
        if (this->ds345 == nullptr)
            this->ds345 = new DS345();
        this->generatorType = "DS345";
        this->openSerial(serial);
        this->ds345->setSerial(serial);
        return true;
    }
    return false;
}

bool Generator::setConnection(const QString &portName, const int &new_baudrate, const QString &new_generatorType)
{
    if (new_generatorType.trimmed().toUpper() == "DS335") {
        if (this->ds335 == nullptr)
            this->ds335 = new DS335();
        this->generatorType = "DS335";
        return this->ds335->setConnection(portName, new_baudrate);
    }
    if (new_generatorType.trimmed().toUpper() == "DS345") {
        if (this->ds345 == nullptr)
            this->ds345 = new DS345();
        this->generatorType = "DS345";
        return this->ds345->setConnection(portName, new_baudrate);
    }
    if (new_generatorType.trimmed().isEmpty() ||
        new_generatorType.trimmed().toUpper() == "AUTO") {
        QSerialPort *serial = new QSerialPort();
        serial->setPortName(portName);
        serial->setBaudRate(new_baudrate);
        return autoSetGeneratorType(serial);
    }
    return false;
}

bool Generator::disconnect() const
{
    if (this->generatorType == "DS335") {
        this->ds335->disconnect();
        return true;
    }
    if (this->generatorType == "DS345") {
        this->ds345->disconnect();
        return true;
    }
    return false;
}

bool Generator::isValidBaudrate(const int &new_baudrate) const
{
    bool answer = false;
    DS335 temp335;
    DS345 temp345;
    answer &= temp335.isValidBaudrate(new_baudrate);
    answer &= temp345.isValidBaudrate(new_baudrate);
    return answer;
}

bool Generator::changeBaudrate(const int &new_baudrate)
{
    if (this->generatorType == "DS335")
        return this->ds335->changeBaudrate(new_baudrate);
    if (this->generatorType == "DS345")
        return this->ds345->changeBaudrate(new_baudrate);
    return false;
}

bool Generator::isActive() const
{
    if (this->generatorType == "DS335")
        return this->ds335->isActive();
    if (this->generatorType == "DS345")
        return this->ds345->isActive();
    return false;
}

bool Generator::send(const QString &command, QString &response, const bool &wait_for_response) const
{
    if (this->generatorType == "DS335")
        return this->ds335->send(command, response, wait_for_response);
    if (this->generatorType == "DS345")
        return this->ds345->send(command, response, wait_for_response);
    return false;
}

bool Generator::sendCommand(const QString &command) const
{
    if (this->generatorType == "DS335")
        return this->ds335->sendCommand(command);
    if (this->generatorType == "DS345")
        return this->ds345->sendCommand(command);
    return false;
}

bool Generator::sendQuery(const QString &command, QString &response) const
{
    if (this->generatorType == "DS335")
        return this->ds335->sendQuery(command, response);
    if (this->generatorType == "DS345")
        return this->ds345->sendQuery(command, response);
    return false;
}

bool Generator::getIDN(QString &idn) const
{
    if (this->generatorType == "DS335")
        return this->ds335->getIDN(idn);
    if (this->generatorType == "DS345")
        return this->ds345->getIDN(idn);
    return false;
}

bool Generator::setAmplitude(const double &new_amplitude, const QString &type) const
{
    if (this->generatorType == "DS335") {
        if (type == "VR")
            return this->ds335->setAmplitude(new_amplitude, true);
        return this->ds335->setAmplitude(new_amplitude, false);
    }
    if (this->generatorType == "DS345")
        return this->ds345->setAmplitude(new_amplitude, type);
    return false;
}

double Generator::getAmplitude(QString &type) const
{
    if (this->generatorType == "DS335") {
        if (type == "VR")
            return this->ds335->getAmplitude(true);
        return this->ds335->getAmplitude(false);
    }
    if (this->generatorType == "DS345")
        return this->ds345->getAmplitude(type);
    return -1;
}

bool Generator::setFrequency(const double &new_frequency) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setFrequency(new_frequency);
    if (this->generatorType == "DS345")
        return this->ds345->setFrequency(new_frequency);
    return false;
}

double Generator::getFrequency() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getFrequency();
    if (this->generatorType == "DS345")
        return this->ds345->getFrequency();
    return -1;
}

int Generator::functionToNumber(const QString &function_string) const
{
    if (this->generatorType == "DS335")
        return this->ds335->functionToNumber(function_string);
    if (this->generatorType == "DS345")
        return this->ds345->functionToNumber(function_string);
    return -1;
}

QString Generator::numberToFunction(const int &function_number) const
{
    if (this->generatorType == "DS335")
        return this->ds335->numberToFunction(function_number);
    if (this->generatorType == "DS345")
        return this->ds345->numberToFunction(function_number);
    return "";
}

bool Generator::setFunction(const int &function_number) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setFunction(function_number);
    if (this->generatorType == "DS345")
        return this->ds345->setFunction(function_number);
    return false;
}

bool Generator::setFunction(const QString &function) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setFunction(function);
    if (this->generatorType == "DS345")
        return this->ds345->setFunction(function);
    return false;
}

QString Generator::getFunction() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getFunction();
    if (this->generatorType == "DS345")
        return this->ds345->getFunction();
    return "";
}

bool Generator::setInverse(const bool &new_inverse) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setInverse(new_inverse);
    if (this->generatorType == "DS345")
        return this->ds345->setInverse(new_inverse);
    return false;
}

bool Generator::getInverse() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getInverse();
    if (this->generatorType == "DS345")
        return this->ds345->getInverse();
    return false;
}

bool Generator::setSynchronization(const bool &new_synchronization) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setSynchronization(new_synchronization);
    if (this->generatorType == "DS345")
        return this->ds345->setSynchronization(new_synchronization);
    return false;
}

bool Generator::getSynchronization() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getSynchronization();
    if (this->generatorType == "DS345")
        return this->ds345->getSynchronization();
    return false;
}

int Generator::impedanceToNumber(const QString &impedance_string) const
{
    if (this->generatorType == "DS335")
        return this->ds335->impedanceToNumber(impedance_string);
    return -1;
}

QString Generator::numberToImpedance(const int &impedance_number) const
{
    if (this->generatorType == "DS335")
        return this->ds335->numberToImpedance(impedance_number);
    return "";
}

bool Generator::setOutputImpedance(const int &impedance_number) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setOutputImpedance(impedance_number);
    return false;
}

bool Generator::setOutputImpedance(const QString &impedance_string) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setOutputImpedance(impedance_string);
    return false;
}

QString Generator::getOutputImpedance() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getOutputImpedance();
    return "";
}

bool Generator::setAMdepth(const int &new_persentage) const
{
    if (this->generatorType == "DS345")
        return this->ds345->setAMdepth(new_persentage);
    return false;
}

int Generator::getAMdepth() const
{
    if (this->generatorType == "DS345")
        return this->ds345->getAMdepth();
    return -1;
}

bool Generator::setFMspan(const double &new_span) const
{
    if (this->generatorType == "DS345")
        return this->ds345->setFMspan(new_span);
    return false;
}

double Generator::getFMspan() const
{
    if (this->generatorType == "DS345")
        return this->ds345->getFMspan();
    return -1;
}

int Generator::modulateFunctionToNumber(const QString &function_string) const
{
    if (this->generatorType == "DS345")
        return this->ds345->modulateFunctionToNumber(function_string);
    return -1;
}

QString Generator::numberToModulateFunction(const int &function_number) const
{
    if (this->generatorType == "DS345")
        return this->ds345->numberToModulateFunction(function_number);
    return "";
}

bool Generator::setModulationFunction(const int &function_number) const
{
    if (this->generatorType == "DS345")
        return this->ds345->setModulationFunction(function_number);
    return false;
}

bool Generator::setModulationFunction(const QString &function_string) const
{
    if (this->generatorType == "DS345")
        return this->ds345->setModulationFunction(function_string);
    return false;
}

QString Generator::getModulationFunction() const
{
    if (this->generatorType == "DS345")
        return this->ds345->getModulationFunction();
    return "";
}

bool Generator::setModulationEnabled(const bool &enable) const
{
    if (this->generatorType == "DS345")
        return this->ds345->setModulationEnabled(enable);
    return false;
}

bool Generator::getModulationEnabled() const
{
    if (this->generatorType == "DS345")
        return this->ds345->getModulationEnabled();
    return false;
}

int Generator::modulateTypeToNumber(const QString &type_string) const
{
    if (this->generatorType == "DS345")
        return this->ds345->modulateTypeToNumber(type_string);
    return -1;
}

QString Generator::numberToModulateType(const int &type_number) const
{
    if (this->generatorType == "DS345")
        return this->ds345->numberToModulateType(type_number);
    return "";
}

bool Generator::setModulationType(const int &function_number) const
{
    if (this->generatorType == "DS345")
        return this->ds345->setModulationType(function_number);
    return false;
}

bool Generator::setModulationType(const QString &function_string) const
{
    if (this->generatorType == "DS345")
        return this->ds345->setModulationType(function_string);
    return false;
}

QString Generator::getModulationType() const
{
    if (this->generatorType == "DS345")
        return this->ds345->getModulationType();
    return "";
}

bool Generator::setModulationRate(const double &new_rate) const
{
    if (this->generatorType == "DS345")
        return this->ds345->setModulationRate(new_rate);
    return false;
}

double Generator::getModulationRate() const
{
    if (this->generatorType == "DS345")
        return this->ds345->getModulationRate();
    return -1;
}

bool Generator::setModulationSpan(const double &new_span) const
{
    if (this->generatorType == "DS345")
        return this->ds345->setModulationSpan(new_span);
    return false;
}

double Generator::getModulationSpan() const
{
    if (this->generatorType == "DS345")
        return this->ds345->getModulationSpan();
    return -1;
}
