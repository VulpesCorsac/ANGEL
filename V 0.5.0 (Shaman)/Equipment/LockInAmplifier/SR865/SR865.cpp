#include "SR865.h"

SR865::SR865(QObject *parent) : QObject(parent)
{
    this->serial = nullptr;
    this->srs = nullptr;
    init();
}

SR865::SR865(const QString &portName, const int &new_baudrate)
{
    this->serial = nullptr;
    this->srs = nullptr;
    init();

    setConnection(portName, new_baudrate);
}

SR865::~SR865()
{
    if (this->serial->isOpen())
        this->serial->close();
    this->serial->~QSerialPort();

    this->srs->~SRS();

    this->baudrateList.clear();
    this->referenceSourceList.clear();
    this->referenceTriggerModeList.clear();
    this->referenceTriggerOutputZList.clear();
    this->sensivityList.clear();
    this->timeConstantList.clear();
    this->filterList.clear();
    this->outputNumberListLongName.clear();
    this->outputNumberListShortName.clear();
}

void SR865::init()
{
    if (this->srs == nullptr)
        this->srs = new SRS();

    initBaudrateList();
    initReferenceSourceList();
    initReferenceTriggerModeList();
    initReferenceTriggerOutputZList();
    initSignalInputList();
    initVoltageInputModeList();
    initVoltageInputCouplingList();
    initVoltageInputShieldsList();
    initVoltageInputRangeList();
    initCurrentInputGainList();
    initSensivityList();
    initTimeConstantList();
    initFilterList();
    initOutputNumberList();
    initFrequencyPostfixList();
    initVoltagePostfixList();

    connect(this->srs, SIGNAL(errorSignal(QString)), this, SLOT(catchErrorSignal(QString)));
    connect(this->srs, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)));
    connect(this->srs, SIGNAL(commandSignal(QString)), this, SLOT(catchCommandSignal(QString)));
    connect(this->srs, SIGNAL(timeoutSignal(QString)), this, SLOT(catchTimeoutSignal(QString)));

    return;
}

void SR865::setSerial(QSerialPort *new_serial)
{
    return this->srs->setSerial(&this->serial, new_serial);
}

QSerialPort* SR865::getSerial() const
{
    return this->srs->getSerial(this->serial);
}

bool SR865::setConnection(const QString &portName, const int &new_baudrate)
{
    return this->srs->setConnection(&this->serial, portName, new_baudrate);
}

void SR865::disconnect() const
{
    return this->srs->disconnect(this->serial);
}

void SR865::initBaudrateList()
{
    this->baudrateList.push_back("300");
    this->baudrateList.push_back("1200");
    this->baudrateList.push_back("2400");
    this->baudrateList.push_back("4800");
    this->baudrateList.push_back("9600");
    this->baudrateList.push_back("19200");
    this->baudrateList.push_back("38400");
    this->baudrateList.push_back("115200");
    this->baudrateList.push_back("230400");
    this->baudrateList.push_back("460800");

    return;
}

QStringList SR865::getBaudrateList() const
{
    return this->baudrateList;
}

bool SR865::isValidBaudrate(const int &new_baudrate) const
{
    return (baudrateList.indexOf(QString::number(new_baudrate)) != -1);
}

bool SR865::changeBaudrate(const int &new_baudrate)
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

bool SR865::isActive() const
{
    return this->srs->isActive(this->serial);
}

bool SR865::send(const QString &command, QString &response, const bool &wait_for_response) const
{
    return this->srs->send(this->serial, command, response, wait_for_response);
}

bool SR865::sendCommand(const QString &command) const
{
    return this->srs->sendCommand(this->serial, command);
}

bool SR865::sendQuery(const QString &command, QString &response) const
{
    return this->srs->sendQuery(this->serial, command, response);
}

QString SR865::ask(const QString &command) const
{
    return this->srs->ask(this->serial, command);
}

bool SR865::getIDN(QString &idn) const
{
    return this->srs->getIDN(this->serial, idn);
}

void SR865::initFrequencyPostfixList()
{
    frequencyPostfixList.push_back( "HZ");
    frequencyPostfixList.push_back("KHZ");
    frequencyPostfixList.push_back("MHZ");

    return;
}

QStringList SR865::getFrequencyPostfixList() const
{
    return this->frequencyPostfixList;
}

void SR865::initVoltagePostfixList()
{
    voltagePostfixList.push_back( "V");
    voltagePostfixList.push_back("mV");
    voltagePostfixList.push_back("uV");
    voltagePostfixList.push_back("nV");

    return;
}

QStringList SR865::getVoltagePostfixList() const
{
    return this->voltagePostfixList;
}

bool SR865::isValidInternalFrequency(const double &new_frequency, const QString &postfix) const
{
    double temp_freq = new_frequency;
    if (postfix.trimmed().toUpper() == "KHZ")
        temp_freq *= 1e3;
    if (postfix.trimmed().toUpper() == "MHZ")
        temp_freq *= 1e6;

    return (temp_freq >= this->minInternalFrequency &&
            temp_freq <= this->maxInternalFrequency);
}

double SR865::getMinInternalFrequency() const
{
    return this->minInternalFrequency;
}

double SR865::getMaxInternalFrequency() const
{
    return this->maxInternalFrequency;
}

bool SR865::setInternalFrequency(const double &new_frequency, const QString &postfix) const
{
    QString command = "FREQ " + QString::number(new_frequency);
    if (postfix.trimmed().toUpper() == "MHZ" ||
        postfix.trimmed().toUpper() == "KHZ" ||
        postfix.trimmed().toUpper() == " HZ")
        command += " " + postfix.trimmed().toUpper();
    command = command.trimmed();
    if (isValidInternalFrequency(new_frequency, postfix))
        return sendCommand(command);
    else
        return false;
}

double SR865::getInternalFrequency() const
{
    return ask("FREQINT?").toDouble();
}

double SR865::getExternalFrequency() const
{
    return ask("FREQEXT?").toDouble();
}

double SR865::getFrequency() const
{
    return ask("FREQ?").toDouble();
}

double SR865::getFrequencyDetect() const
{
    return ask("FREQDET?").toDouble();
}

int SR865::getMinHarmonic() const
{
    return this->minHarmonic;
}

int SR865::getMaxHarmonic() const
{
    return this->maxHarmonic;
}

bool SR865::isValidHarmonic(const int &i) const
{
    return (i >= this->minHarmonic &&
            i <= this->maxHarmonic);
}

bool SR865::setHarmonic(const int &i) const
{
    QString command = "HARM " + QString::number(i);
    if (isValidHarmonic(i))
        return sendCommand(command);
    else
        return false;
}

int SR865::getHarmonic() const
{
    return ask("HARM?").toInt();
}

int SR865::getMinHarmonicDual() const
{
    return this->minHarmonicDual;
}

int SR865::getMaxHarmonicDual() const
{
    return this->maxHarmonicDual;
}

bool SR865::isValidHarmonicDual(const int &i) const
{
    return (i >= this->minHarmonicDual &&
            i <= this->maxHarmonicDual);
}

bool SR865::setHarmonicDual(const int &i) const
{
    QString command = "HARMDUAL " + QString::number(i);
    if (isValidHarmonicDual(i))
        return sendCommand(command);
    else
        return false;
}

int SR865::getHarmonicDual() const
{
    return ask("HARMDUAL?").toInt();
}

bool SR865::isValidSineOutAmplitude(const double &new_sineAmplitude, const QString &postfix) const
{
    double temp_ampl = new_sineAmplitude;
    if (postfix.trimmed().toUpper() == "NV")
        temp_ampl *= 1e-9;
    if (postfix.trimmed().toUpper() == "UV")
        temp_ampl *= 1e-6;
    if (postfix.trimmed().toUpper() == "MV")
        temp_ampl *= 1e-3;

    return (temp_ampl >= this->minSineOutputAmplitude &&
            temp_ampl <= this->maxSineOutputAmplitude);
}

double SR865::getMinSineOutAmplitude() const
{
    return this->minSineOutputAmplitude;
}

double SR865::getMaxSineOutAmplitude() const
{
    return this->maxSineOutputAmplitude;
}

bool SR865::setSineOutAmplitude(const double &new_sineAmplitude, const QString &postfix) const
{
    QString command = "SLVL " + QString::number(new_sineAmplitude);
    if (postfix.trimmed().toUpper() == "NV" ||
        postfix.trimmed().toUpper() == "UV" ||
        postfix.trimmed().toUpper() == "MV" ||
        postfix.trimmed().toUpper() ==  "V")
        command += " " + postfix.trimmed().toUpper();
    command = command.trimmed();
    if (isValidSineOutAmplitude(new_sineAmplitude, postfix))
        return sendCommand(command);
    else
        return false;
}

double SR865::getSineOutAmplitude() const
{
    return ask("SLVL?").toDouble();
}

bool SR865::isValidSineOffset(const double &new_offset, const QString &postfix) const
{
    double temp_offset = new_offset;
    if (postfix.trimmed().toUpper() == "NV")
        temp_offset *= 1e-9;
    if (postfix.trimmed().toUpper() == "UV")
        temp_offset *= 1e-6;
    if (postfix.trimmed().toUpper() == "MV")
        temp_offset *= 1e-3;

    return (temp_offset >= this->minSineOutputAmplitude &&
            temp_offset <= this->maxSineOutputAmplitude);
}

double SR865::getMinSineOffset() const
{
    return this->minSineOffset;
}

double SR865::getMaxSineOffset() const
{
    return this->maxSineOffset;
}

bool SR865::setSineOffset(const double &new_offset, const QString &postfix) const
{
    QString command = "SOFF " + QString::number(new_offset);
    if (postfix.trimmed().toUpper() == "NV" ||
        postfix.trimmed().toUpper() == "UV" ||
        postfix.trimmed().toUpper() == "MV" ||
        postfix.trimmed().toUpper() ==  "V")
        command += " " + postfix.trimmed().toUpper();
    command = command.trimmed();
    if (isValidSineOutAmplitude(new_offset, postfix))
        return sendCommand(command);
    else
        return false;
}

double SR865::getSineOffset() const
{
    return ask("SOFF?").toDouble();
}

void SR865::initReferenceSourceList()
{
    referenceSourceList.push_back( "INT");
    referenceSourceList.push_back( "EXT");
    referenceSourceList.push_back("DUAL");
    referenceSourceList.push_back("CHOP");

    return;
}

QStringList SR865::getReferenceSourceList() const
{
    return this->referenceSourceList;
}

int SR865::refSourceNumberFromString(const QString &ref_string) const
{
    if (referenceSourceList.indexOf(ref_string.trimmed()) != -1)
        return referenceSourceList.indexOf(ref_string.trimmed());
    return -1;
}

QString SR865::refSourceStringFromNumber(const int &ref_number) const
{
    if (ref_number >= 0 &&
        ref_number < referenceSourceList.length())
        return referenceSourceList.at(ref_number);
    return "";
}

bool SR865::setReferenceSource(const int &new_source) const
{
    if (new_source < 0 || new_source >= referenceSourceList.size())
        return false;

    QString command = "RSRC " + QString::number(new_source);
    return sendCommand(command);
}

bool SR865::setReferenceSource(const QString &new_source) const
{
    return setReferenceSource(refSourceNumberFromString(new_source));
}

QString SR865::getReferenceSource() const
{
    return refSourceStringFromNumber(ask("RSRC?").toInt());
}

void SR865::initReferenceTriggerModeList()
{
    referenceTriggerModeList.push_back("SIN");
    referenceTriggerModeList.push_back("POSTTL");
    referenceTriggerModeList.push_back("NEGTTL");

    return;
}

QStringList SR865::getReferenceTriggerModeList() const
{
    return this->referenceTriggerModeList;
}

int SR865::refModeNumberFromString(const QString &mode_string) const
{
    if (referenceTriggerModeList.indexOf(mode_string.trimmed()) != -1)
        return referenceTriggerModeList.indexOf(mode_string.trimmed());
    return -1;
}

QString SR865::refModeStringFromNumber(const int &mode_number) const
{
    if (mode_number >= 0 &&
        mode_number < referenceTriggerModeList.length())
        return referenceTriggerModeList.at(mode_number);
    return "";
}

bool SR865::setReferenceTriggerMode(const int &new_mode) const
{
    if (new_mode < 0 || new_mode >= referenceTriggerModeList.size())
        return false;

    QString command = "RTRG " + QString::number(new_mode);
    return sendCommand(command);
}

bool SR865::setReferenceTriggerMode(const QString &new_mode) const
{
    return setReferenceTriggerMode(refModeNumberFromString(new_mode));
}

QString SR865::getReferenceTriggerMode() const
{
    return refModeStringFromNumber(ask("RTRG?").toInt());
}

void SR865::initReferenceTriggerOutputZList()
{
    referenceTriggerOutputZList.push_back("50OHMS");
    referenceTriggerOutputZList.push_back( "1MEG" );

    return;
}

QStringList SR865::getReferenceTriggerOutputZList() const
{
    return this->referenceTriggerOutputZList;
}

int SR865::refOutputNumberFromString(const QString &outpZ_string) const
{
    if (referenceTriggerOutputZList.indexOf(outpZ_string.trimmed()) != -1)
        return referenceTriggerOutputZList.indexOf(outpZ_string.trimmed());
    return -1;
}

QString SR865::refOutputStringFromNumber(const int &outpZ_number) const
{
    if (outpZ_number >= 0 &&
        outpZ_number < referenceTriggerOutputZList.length())
        return referenceTriggerOutputZList.at(outpZ_number);
    return "";
}

bool SR865::setReferenceTriggerOutputZ(const int &new_mode) const
{
    if (new_mode < 0 || new_mode >= referenceTriggerOutputZList.size())
        return false;

    QString command = "REFZ " + QString::number(new_mode);
    return sendCommand(command);
}

bool SR865::setReferenceTriggerOutputZ(const QString &new_mode) const
{
    return setReferenceTriggerOutputZ(refOutputNumberFromString(new_mode));
}

QString SR865::getReferenceTriggerOutputZ() const
{
    return refOutputStringFromNumber(ask("REFZ?").toInt());
}

void SR865::initSignalInputList()
{
    signalInputList.push_back("VOLTAGE");
    signalInputList.push_back("CURRENT");

    return;
}

QStringList SR865::getSignalInputList() const
{
    return this->signalInputList;
}

int SR865::signalInputNumberFromString(const QString &signalInput_string) const
{
    if (signalInputList.indexOf(signalInput_string.trimmed()) != -1)
        return signalInputList.indexOf(signalInput_string.trimmed());
    return -1;
}

QString SR865::signalInputStringFromNumber(const int &signalInput_number) const
{
    if (signalInput_number >= 0 &&
        signalInput_number < signalInputList.length())
        return signalInputList.at(signalInput_number);
    return "";
}

bool SR865::setSignalInput(const int &new_signalInput) const
{
    if (new_signalInput < 0 || new_signalInput >= signalInputList.size())
        return false;

    QString command = "IVMD " + QString::number(new_signalInput);
    return sendCommand(command);
}

bool SR865::setSignalInput(const QString &new_signalInput) const
{
    return setSignalInput(signalInputNumberFromString(new_signalInput));
}

QString SR865::getSignalInput() const
{
    return signalInputStringFromNumber(ask("IVMD?").toInt());
}

void SR865::initVoltageInputModeList()
{
    voltageInputModeList.push_back("A");
    voltageInputModeList.push_back("A-B");

    return;
}

QStringList SR865::getVoltageInputModeList() const
{
    return this->voltageInputModeList;
}

int SR865::voltageInputModeNumberFromString(const QString &voltageInputMode_string) const
{
    if (voltageInputModeList.indexOf(voltageInputMode_string.trimmed()) != -1)
        return voltageInputModeList.indexOf(voltageInputMode_string.trimmed());
    return -1;
}

QString SR865::voltageInputModeStringFromNumber(const int &voltageInputMode_number) const
{
    if (voltageInputMode_number >= 0 &&
        voltageInputMode_number < voltageInputModeList.length())
        return voltageInputModeList.at(voltageInputMode_number);
    return "";
}

bool SR865::setVoltageInputMode(const int &new_voltageInputMode) const
{
    if (new_voltageInputMode < 0 || new_voltageInputMode >= voltageInputModeList.size())
        return false;

    QString command = "ISRC " + QString::number(new_voltageInputMode);
    return sendCommand(command);
}

bool SR865::setVoltageInputMode(const QString &new_voltageInputMode) const
{
    return setVoltageInputMode(voltageInputModeNumberFromString(new_voltageInputMode));
}

QString SR865::getVoltageInputMode() const
{
    return voltageInputModeStringFromNumber(ask("ISRC?").toInt());
}

void SR865::initVoltageInputCouplingList()
{
    voltageInputCouplingList.push_back("AC");
    voltageInputCouplingList.push_back("DC");

    return;
}

QStringList SR865::getVoltageInputCouplingList() const
{
    return this->voltageInputCouplingList;
}

int SR865::voltageInputCouplingNumberFromString(const QString &voltageInputCoupling_string) const
{
    if (voltageInputCouplingList.indexOf(voltageInputCoupling_string.trimmed()) != -1)
        return voltageInputCouplingList.indexOf(voltageInputCoupling_string.trimmed());
    return -1;
}

QString SR865::voltageInputCouplingStringFromNumber(const int &voltageInputCoupling_number) const
{
    if (voltageInputCoupling_number >= 0 &&
        voltageInputCoupling_number < voltageInputCouplingList.length())
        return voltageInputCouplingList.at(voltageInputCoupling_number);
    return "";
}

bool SR865::setVoltageInputCoupling(const int &new_voltageInputCoupling) const
{
    if (new_voltageInputCoupling < 0 || new_voltageInputCoupling >= voltageInputCouplingList.size())
        return false;

    QString command = "ICPL " + QString::number(new_voltageInputCoupling);
    return sendCommand(command);
}

bool SR865::setVoltageInputCoupling(const QString &new_voltageInputCoupling) const
{
    return setVoltageInputCoupling(voltageInputCouplingNumberFromString(new_voltageInputCoupling));
}

QString SR865::getVoltageInputCoupling() const
{
    return voltageInputCouplingStringFromNumber(ask("ICPL?").toInt());
}

void SR865::initVoltageInputShieldsList()
{
    voltageInputShieldsList.push_back("FLOAT");
    voltageInputShieldsList.push_back("GROUND");

    return;
}

QStringList SR865::getVoltageInputShieldsList() const
{
    return this->voltageInputShieldsList;
}

int SR865::voltageInputShieldsNumberFromString(const QString &voltageInputShields_string) const
{
    if (voltageInputShieldsList.indexOf(voltageInputShields_string.trimmed()) != -1)
        return voltageInputShieldsList.indexOf(voltageInputShields_string.trimmed());
    return -1;
}

QString SR865::voltageInputShieldsStringFromNumber(const int &voltageInputShields_number) const
{
    if (voltageInputShields_number >= 0 &&
        voltageInputShields_number < voltageInputShieldsList.length())
        return voltageInputShieldsList.at(voltageInputShields_number);
    return "";
}

bool SR865::setVoltageInputShields(const int &new_voltageInputShields) const
{
    if (new_voltageInputShields < 0 || new_voltageInputShields >= voltageInputShieldsList.size())
        return false;

    QString command = "IGND " + QString::number(new_voltageInputShields);
    return sendCommand(command);
}

bool SR865::setVoltageInputShields(const QString &new_voltageInputShields) const
{
    return setVoltageInputShields(voltageInputShieldsNumberFromString(new_voltageInputShields));
}

QString SR865::getVoltageInputShields() const
{
    return voltageInputShieldsStringFromNumber(ask("IGND?").toInt());
}

void SR865::initVoltageInputRangeList()
{
    voltageInputRangeList.push_back(  "1V");
    voltageInputRangeList.push_back("300M");
    voltageInputRangeList.push_back("100M");
    voltageInputRangeList.push_back( "30M");
    voltageInputRangeList.push_back( "10M");

    return;
}

QStringList SR865::getVoltageInputRangeList() const
{
    return this->voltageInputRangeList;
}

int SR865::voltageInputRangeNumberFromString(const QString &voltageInputRange_string) const
{
    if (voltageInputRangeList.indexOf(voltageInputRange_string.trimmed()) != -1)
        return voltageInputRangeList.indexOf(voltageInputRange_string.trimmed());
    return -1;
}

QString SR865::voltageInputRangeStringFromNumber(const int &voltageInputRange_number) const
{
    if (voltageInputRange_number >= 0 &&
        voltageInputRange_number < voltageInputRangeList.length())
        return voltageInputRangeList.at(voltageInputRange_number);
    return "";
}

bool SR865::setVoltageInputRange(const int &new_voltageInputRange) const
{
    if (new_voltageInputRange < 0 || new_voltageInputRange >= voltageInputRangeList.size())
        return false;

    QString command = "IRNG " + QString::number(new_voltageInputRange);
    return sendCommand(command);
}

bool SR865::setVoltageInputRange(const QString &new_voltageInputRange) const
{
    return setVoltageInputRange(voltageInputRangeNumberFromString(new_voltageInputRange));
}

QString SR865::getVoltageInputRange() const
{
    return voltageInputRangeStringFromNumber(ask("IRNG?").toInt());
}

void SR865::initCurrentInputGainList()
{
    currrentInputGainList.push_back( "1uA");
    currrentInputGainList.push_back("10nA");

    return;
}

QStringList SR865::getCurrentInputGainList() const
{
    return this->currrentInputGainList;
}

int SR865::currentInputGainNumberFromString(const QString &currentInputGain_string) const
{
    if (currrentInputGainList.indexOf(currentInputGain_string.trimmed()) != -1)
        return currrentInputGainList.indexOf(currentInputGain_string.trimmed());
    return -1;
}

QString SR865::currentInputGainStringFromNumber(const int &currentInputGain_number) const
{
    if (currentInputGain_number >= 0 &&
        currentInputGain_number < currrentInputGainList.length())
        return currrentInputGainList.at(currentInputGain_number);
    return "";
}

bool SR865::setCurrentInputGain(const int &new_currentInputGain) const
{
    if (new_currentInputGain < 0 || new_currentInputGain >= currrentInputGainList.size())
        return false;

    QString command = "ICUR " + QString::number(new_currentInputGain);
    return sendCommand(command);
}

bool SR865::setCurrentInputGain(const QString &new_currentInputGain) const
{
    return setCurrentInputGain(currentInputGainNumberFromString(new_currentInputGain));
}

QString SR865::getCurrentInputGain() const
{
    return currentInputGainStringFromNumber(ask("ICUR?").toInt());
}

int SR865::getSignalStrength() const
{
    return ask("ILVL").toInt();
}

void SR865::initSensivityList()
{
    sensivityList.push_back(  "1 V/uA");
    sensivityList.push_back("500 mV/nA");
    sensivityList.push_back("200 mV/nA");
    sensivityList.push_back("100 mV/nA");
    sensivityList.push_back( "50 mV/nA");
    sensivityList.push_back( "20 mV/nA");
    sensivityList.push_back( "10 mV/nA");
    sensivityList.push_back(  "5 mV/nA");
    sensivityList.push_back(  "2 mV/nA");
    sensivityList.push_back(  "1 mV/nA");
    sensivityList.push_back("500 uV/pA");
    sensivityList.push_back("200 uV/pA");
    sensivityList.push_back("100 uV/pA");
    sensivityList.push_back( "50 uV/pA");
    sensivityList.push_back( "20 uV/pA");
    sensivityList.push_back( "10 uV/pA");
    sensivityList.push_back(  "5 uV/pA");
    sensivityList.push_back(  "2 uV/pA");
    sensivityList.push_back(  "1 uV/pA");
    sensivityList.push_back("500 nV/fA");
    sensivityList.push_back("200 nV/fA");
    sensivityList.push_back("100 nV/fA");
    sensivityList.push_back( "50 nV/fA");
    sensivityList.push_back( "20 nV/fA");
    sensivityList.push_back( "10 nV/fA");
    sensivityList.push_back(  "5 nV/fA");
    sensivityList.push_back(  "2 nV/fA");
    sensivityList.push_back(  "1 nV/fA");

    return;
}

QStringList SR865::getSensivityList() const
{
    return this->sensivityList;
}

int SR865::sensivityNumberFromString(const QString &sensivity_string) const
{
    if (sensivityList.indexOf(sensivity_string.trimmed()) != -1)
        return sensivityList.indexOf(sensivity_string.trimmed());
    return -1;
}

QString SR865::sensivityStringFromNumber(const int &sensivity_number) const
{
    if (sensivity_number >= 0 &&
        sensivity_number < sensivityList.length())
        return sensivityList.at(sensivity_number);
    return "";
}

bool SR865::setSensivity(const int &new_sensivity) const
{
    if (new_sensivity < 0 || new_sensivity >= sensivityList.size())
        return false;

    QString command = "SCAL " + QString::number(new_sensivity);
    return sendCommand(command);
}

bool SR865::setSensivity(const QString &new_sensivity) const
{
    return setSensivity(sensivityNumberFromString(new_sensivity));
}

QString SR865::getSensivity() const
{
    return sensivityStringFromNumber(ask("SCAL?").toInt());
}

void SR865::initTimeConstantList()
{
    timeConstantList.push_back(  "1 us");
    timeConstantList.push_back(  "3 us");
    timeConstantList.push_back( "10 us");
    timeConstantList.push_back( "30 us");
    timeConstantList.push_back("100 us");
    timeConstantList.push_back("300 us");
    timeConstantList.push_back(  "1 ms");
    timeConstantList.push_back(  "3 ms");
    timeConstantList.push_back( "10 ms");
    timeConstantList.push_back( "30 ms");
    timeConstantList.push_back("100 ms");
    timeConstantList.push_back("300 ms");
    timeConstantList.push_back(  "1 s");
    timeConstantList.push_back(  "3 s");
    timeConstantList.push_back( "10 s");
    timeConstantList.push_back( "30 s");
    timeConstantList.push_back("100 s");
    timeConstantList.push_back("300 s");
    timeConstantList.push_back(  "1 ks");
    timeConstantList.push_back(  "3 ks");
    timeConstantList.push_back( "10 ks");
    timeConstantList.push_back( "30 ks");

    return;
}

QStringList SR865::getTimeConstantList() const
{
    return this->timeConstantList;
}

int SR865::timeConstantNumberFromString(const QString &timeConstant_string) const
{
    if (timeConstantList.indexOf(timeConstant_string.trimmed()) != -1)
        return timeConstantList.indexOf(timeConstant_string.trimmed());
    return -1;
}

QString SR865::timeConstantStringFromNumber(const int &timeConstant_number) const
{
    if (timeConstant_number >= 0 &&
        timeConstant_number < timeConstantList.length())
        return timeConstantList.at(timeConstant_number);
    return "";
}

bool SR865::setTimeConstant(const int &new_timeConstant) const
{
    if (new_timeConstant < 0 || new_timeConstant >= timeConstantList.size())
        return false;

    QString command = "OFLT " + QString::number(new_timeConstant);
    return sendCommand(command);
}

bool SR865::setTimeConstant(const QString &new_timeConstant) const
{
    return setTimeConstant(timeConstantNumberFromString(new_timeConstant));
}

QString SR865::getTimeConstant() const
{
    return timeConstantStringFromNumber(ask("OFLT?").toInt());
}

void SR865::initFilterList()
{
    filterList.push_back( "6 dB/oct");
    filterList.push_back("12 dB/oct");
    filterList.push_back("18 dB/oct");
    filterList.push_back("24 dB/oct");

    return;
}

QStringList SR865::getFilterList() const
{
    return this->filterList;
}

int SR865::filterNumberFromString(const QString &filter_string) const
{
    if (filterList.indexOf(filter_string.trimmed()) != -1)
        return filterList.indexOf(filter_string.trimmed());
    return -1;
}

QString SR865::filterStringFromNumber(const int &filter_number) const
{
    if (filter_number >= 0 &&
        filter_number < filterList.length())
        return filterList.at(filter_number);
    return "";
}

bool SR865::setFilter(const int &new_filter) const
{
    if (new_filter < 0 || new_filter >= filterList.size())
        return false;

    QString command = "OFSL " + QString::number(new_filter);
    return sendCommand(command);
}

bool SR865::setFilter(const QString &new_filter) const
{
    return setFilter(filterNumberFromString(new_filter));
}

QString SR865::getFilter() const
{
    return filterStringFromNumber(ask("OFSL?").toInt());
}

bool SR865::enableSychronousFilter(const bool &enable) const
{
    QString command = "SYNC ";
    if (enable)
        command += "ON";
    else
        command += "OFF";

    return sendCommand(command);
}

bool SR865::isSychronousFilterEnabled() const
{
    return ask("SYNC?").toInt();
}

bool SR865::enableAdvancedFilterFilter(const bool &enable) const
{
    QString command = "ADVFILT ";
    if (enable)
        command += "ON";
    else
        command += "OFF";
    return sendCommand(command);
}

bool SR865::isAdvancedFilterEnabled() const
{
    return ask("ADVFILT?").toInt();
}

void SR865::initOutputNumberList()
{
    this->outputNumberListLongName.clear();
    this->outputNumberListShortName.clear();

    this->outputNumberListLongName.push_back("X");
    this->outputNumberListShortName.push_back("X");

    this->outputNumberListLongName.push_back("Y");
    this->outputNumberListShortName.push_back("Y");

    this->outputNumberListLongName.push_back("R");
    this->outputNumberListShortName.push_back("R");

    this->outputNumberListLongName.push_back("THETA");
    this->outputNumberListShortName.push_back("THET");

    this->outputNumberListLongName.push_back("IN1");
    this->outputNumberListShortName.push_back("IN1");

    this->outputNumberListLongName.push_back("IN2");
    this->outputNumberListShortName.push_back("IN2");

    this->outputNumberListLongName.push_back("IN3");
    this->outputNumberListShortName.push_back("IN3");

    this->outputNumberListLongName.push_back("IN4");
    this->outputNumberListShortName.push_back("IN4");

    this->outputNumberListLongName.push_back("OUT1");
    this->outputNumberListShortName.push_back("OUT1");

    this->outputNumberListLongName.push_back("OUT2");
    this->outputNumberListShortName.push_back("OUT2");

    this->outputNumberListLongName.push_back("XNOISE");
    this->outputNumberListShortName.push_back("XNO");

    this->outputNumberListLongName.push_back("YNOISE");
    this->outputNumberListShortName.push_back("YNO");

    this->outputNumberListLongName.push_back("PHASE");
    this->outputNumberListShortName.push_back("PHA");

    this->outputNumberListLongName.push_back("SAMP");
    this->outputNumberListShortName.push_back("SAM");

    this->outputNumberListLongName.push_back("LEVEL");
    this->outputNumberListShortName.push_back("LEV");

    this->outputNumberListLongName.push_back("FINT");
    this->outputNumberListShortName.push_back("FI");

    this->outputNumberListLongName.push_back("FEXT");
    this->outputNumberListShortName.push_back("FE");

    return;
}

QStringList SR865::getOutputNumberList() const
{
    return this->outputNumberListLongName;
}

int SR865::outpNumberFromString(const QString &outp_string) const
{
    if (outputNumberListLongName.indexOf(outp_string.trimmed().toUpper()) != -1)
        return outputNumberListLongName.indexOf(outp_string);
    if (outputNumberListShortName.indexOf(outp_string) != -1)
        return outputNumberListShortName.indexOf(outp_string);
    return -1;
}

QString SR865::outpStringFromNumber(const int &outp_number) const
{
    if (outp_number >= 0 &&
        outp_number < outputNumberListLongName.length())
        return outputNumberListLongName.at(outp_number);
    return "";
}

double SR865::getOUTP(const int &i) const
{
    if (i < 0 || i >= outputNumberListLongName.size())
        return 0;

    QString command = "OUTP? " + QString::number(i);
    QString response;
    sendQuery(command, response);
    return response.trimmed().toDouble();
}

double SR865::getOUTP(const QString &i) const
{
    if (outputNumberListLongName.indexOf(i) != -1)
        return getOUTP(outputNumberListLongName.indexOf(i));
    if (outputNumberListShortName.indexOf(i) != -1)
        return getOUTP(outputNumberListShortName.indexOf(i));
    return 0;
}

double SR865::getX() const
{
    return getOUTP(0);
}

double SR865::getY() const
{
    return getOUTP(1);
}

double SR865::getR() const
{
    return getOUTP(2);
}

double SR865::getTheta() const
{
    return getOUTP(3);
}

bool SR865::getAB(const int & Apos, double &A, const int &Bpos, double &B) const
{
    QString command = "SNAP? " + QString::number(Apos) + "," + QString::number(Bpos);
    QString response;
    if (!sendQuery(command, response))
        return false;

    QStringList responseList = response.trimmed().split(',');
    QString _a = responseList.at(0);
    QString _b = responseList.at(1);
    A = _a.trimmed().toDouble();
    B = _b.trimmed().toDouble();

    return true;
}

bool SR865::getABC(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C) const
{
    QString command = "SNAP? " + QString::number(Apos) + "," + QString::number(Bpos) + "," + QString::number(Cpos);
    QString response;
    if (!sendQuery(command, response))
        return false;

    QStringList responseList = response.trimmed().split(',');
    QString _a = responseList.at(0);
    QString _b = responseList.at(1);
    QString _c = responseList.at(2);
    A = _a.trimmed().toDouble();
    B = _b.trimmed().toDouble();
    C = _c.trimmed().toDouble();

    return true;
}

bool SR865::getXY(double &X, double &Y) const
{
    return getAB(0, X, 1, Y);
}

bool SR865::getRTheta(double &R, double &Theta) const
{
    return getAB(2, R, 3, Theta);
}

bool SR865::getRThetaFint(double &R, double &Theta, double &F) const
{
    return getABC(2, R, 3, Theta, 15, F);
}

bool SR865::getRThetaFext(double &R, double &Theta, double &F) const
{
    return getABC(2, R, 3, Theta, 16, F);
}

bool SR865::getRThetaF(double &R, double &Theta, double &F) const
{
    if (getReferenceSource() == "INT")
        return getRThetaFint(R, Theta, F);
    if (getReferenceSource() == "EXT")
        return getRThetaFext(R, Theta, F);
    return false;
}

void SR865::catchErrorSignal(const QString &s) const
{
    emit this->errorSignal(s);
}

void SR865::catchResponseSignal(const QString &s) const
{
    emit this->responseSignal(s);
}

void SR865::catchCommandSignal(const QString &s) const
{
    emit this->commandSignal(s);
}

void SR865::catchTimeoutSignal(const QString &s) const
{
    emit this->timeoutSignal(s);
}
