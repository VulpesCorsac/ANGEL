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

    connect(this->srs, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString))   , Qt::DirectConnection);
    connect(this->srs, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
    connect(this->srs, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)) , Qt::DirectConnection);
    connect(this->srs, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)) , Qt::DirectConnection);

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
    this->baudrateList.clear();

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
    return isValidString(this->baudrateList, QString::number(new_baudrate));
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

int SR865::numberFromString(const QStringList &list, const QString &string) const
{
    return this->srs->numberFromString(list, string);
}

QString SR865::stringFromNumber(const QStringList &list, const int &number) const
{
    return this->srs->stringFromNumber(list, number);
}

bool SR865::isValidString(const QStringList &list, const QString &string) const
{
    return this->srs->isValidString(list, string);
}

bool SR865::isValidNumber(const QStringList &list, const int &number) const
{
    return this->srs->isValidNumber(list, number);
}

void SR865::initFrequencyPostfixList()
{
    this->frequencyPostfixList.clear();

    this->frequencyPostfixList.push_back( "HZ");
    this->frequencyPostfixList.push_back("KHZ");
    this->frequencyPostfixList.push_back("MHZ");

    return;
}

QStringList SR865::getFrequencyPostfixList() const
{
    return this->frequencyPostfixList;
}

void SR865::initVoltagePostfixList()
{
    this->voltagePostfixList.clear();

    this->voltagePostfixList.push_back( "V");
    this->voltagePostfixList.push_back("mV");
    this->voltagePostfixList.push_back("uV");
    this->voltagePostfixList.push_back("nV");

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

    return (temp_offset >= this->minSineOffset &&
            temp_offset <= this->maxSineOffset);
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
    if (isValidSineOffset(new_offset, postfix))
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
    this->referenceSourceList.clear();

    this->referenceSourceList.push_back( "INT");
    this->referenceSourceList.push_back( "EXT");
    this->referenceSourceList.push_back("DUAL");
    this->referenceSourceList.push_back("CHOP");

    return;
}

QStringList SR865::getReferenceSourceList() const
{
    return this->referenceSourceList;
}

int SR865::refSourceNumberFromString(const QString &ref_string) const
{
    return numberFromString(this->referenceSourceList, ref_string);
}

QString SR865::refSourceStringFromNumber(const int &ref_number) const
{
    return this->srs->stringFromNumber(this->referenceSourceList, ref_number);
}

bool SR865::setReferenceSource(const int &new_source) const
{
    if (!isValidNumber(this->referenceSourceList, new_source))
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
    this->referenceTriggerModeList.clear();

    this->referenceTriggerModeList.push_back("SIN");
    this->referenceTriggerModeList.push_back("POSTTL");
    this->referenceTriggerModeList.push_back("NEGTTL");

    return;
}

QStringList SR865::getReferenceTriggerModeList() const
{
    return this->referenceTriggerModeList;
}

int SR865::refModeNumberFromString(const QString &mode_string) const
{
    return numberFromString(this->referenceTriggerModeList, mode_string);
}

QString SR865::refModeStringFromNumber(const int &mode_number) const
{
    return stringFromNumber(this->referenceTriggerModeList, mode_number);
}

bool SR865::setReferenceTriggerMode(const int &new_mode) const
{
    if (!isValidNumber(this->referenceTriggerModeList, new_mode))
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
    this->referenceTriggerOutputZList.clear();

    this->referenceTriggerOutputZList.push_back("50OHMS");
    this->referenceTriggerOutputZList.push_back( "1MEG" );

    return;
}

QStringList SR865::getReferenceTriggerOutputZList() const
{
    return this->referenceTriggerOutputZList;
}

int SR865::refOutputZNumberFromString(const QString &outpZ_string) const
{
    return numberFromString(this->referenceTriggerOutputZList, outpZ_string);
}

QString SR865::refOutputZStringFromNumber(const int &outpZ_number) const
{
    return stringFromNumber(this->referenceTriggerOutputZList, outpZ_number);
}

bool SR865::setReferenceTriggerOutputZ(const int &new_mode) const
{
    if (!isValidNumber(this->referenceTriggerOutputZList, new_mode))
        return false;

    QString command = "REFZ " + QString::number(new_mode);
    return sendCommand(command);
}

bool SR865::setReferenceTriggerOutputZ(const QString &new_mode) const
{
    return setReferenceTriggerOutputZ(refOutputZNumberFromString(new_mode));
}

QString SR865::getReferenceTriggerOutputZ() const
{
    return refOutputZStringFromNumber(ask("REFZ?").toInt());
}

void SR865::initSignalInputList()
{
    this->signalInputList.clear();

    this->signalInputList.push_back("VOLTAGE");
    this->signalInputList.push_back("CURRENT");

    return;
}

QStringList SR865::getSignalInputList() const
{
    return this->signalInputList;
}

int SR865::signalInputNumberFromString(const QString &signalInput_string) const
{
    return numberFromString(this->signalInputList, signalInput_string);
}

QString SR865::signalInputStringFromNumber(const int &signalInput_number) const
{
    return stringFromNumber(this->signalInputList, signalInput_number);
}

bool SR865::setSignalInput(const int &new_signalInput) const
{
    if (!isValidNumber(this->signalInputList, new_signalInput))
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
    this->voltageInputModeList.clear();

    this->voltageInputModeList.push_back("A");
    this->voltageInputModeList.push_back("A-B");

    return;
}

QStringList SR865::getVoltageInputModeList() const
{
    return this->voltageInputModeList;
}

int SR865::voltageInputModeNumberFromString(const QString &voltageInputMode_string) const
{
    return numberFromString(this->voltageInputModeList, voltageInputMode_string);
}

QString SR865::voltageInputModeStringFromNumber(const int &voltageInputMode_number) const
{
    return stringFromNumber(this->voltageInputModeList, voltageInputMode_number);
}

bool SR865::setVoltageInputMode(const int &new_voltageInputMode) const
{
    if (!isValidNumber(this->voltageInputModeList, new_voltageInputMode))
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
    this->voltageInputCouplingList.clear();

    this->voltageInputCouplingList.push_back("AC");
    this->voltageInputCouplingList.push_back("DC");

    return;
}

QStringList SR865::getVoltageInputCouplingList() const
{
    return this->voltageInputCouplingList;
}

int SR865::voltageInputCouplingNumberFromString(const QString &voltageInputCoupling_string) const
{
    return numberFromString(this->voltageInputCouplingList, voltageInputCoupling_string);
}

QString SR865::voltageInputCouplingStringFromNumber(const int &voltageInputCoupling_number) const
{
    return stringFromNumber(this->voltageInputCouplingList, voltageInputCoupling_number);
}

bool SR865::setVoltageInputCoupling(const int &new_voltageInputCoupling) const
{
    if (!isValidNumber(this->voltageInputCouplingList, new_voltageInputCoupling))
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
    this->voltageInputShieldsList.clear();

    this->voltageInputShieldsList.push_back("FLOAT");
    this->voltageInputShieldsList.push_back("GROUND");

    return;
}

QStringList SR865::getVoltageInputShieldsList() const
{
    return this->voltageInputShieldsList;
}

int SR865::voltageInputShieldsNumberFromString(const QString &voltageInputShields_string) const
{
    return numberFromString(this->voltageInputShieldsList, voltageInputShields_string);
}

QString SR865::voltageInputShieldsStringFromNumber(const int &voltageInputShields_number) const
{
    return stringFromNumber(this->voltageInputShieldsList, voltageInputShields_number);
}

bool SR865::setVoltageInputShields(const int &new_voltageInputShields) const
{
    if (!isValidNumber(this->voltageInputShieldsList, new_voltageInputShields))
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
    this->voltageInputRangeList.clear();

    this->voltageInputRangeList.push_back(  "1V");
    this->voltageInputRangeList.push_back("300M");
    this->voltageInputRangeList.push_back("100M");
    this->voltageInputRangeList.push_back( "30M");
    this->voltageInputRangeList.push_back( "10M");

    return;
}

QStringList SR865::getVoltageInputRangeList() const
{
    return this->voltageInputRangeList;
}

int SR865::voltageInputRangeNumberFromString(const QString &voltageInputRange_string) const
{
    return numberFromString(this->voltageInputRangeList, voltageInputRange_string);
}

QString SR865::voltageInputRangeStringFromNumber(const int &voltageInputRange_number) const
{
    return stringFromNumber(this->voltageInputRangeList, voltageInputRange_number);
}

bool SR865::setVoltageInputRange(const int &new_voltageInputRange) const
{
    if (!isValidNumber(this->voltageInputRangeList, new_voltageInputRange))
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
    this->currrentInputGainList.clear();

    this->currrentInputGainList.push_back( "1uA");
    this->currrentInputGainList.push_back("10nA");

    return;
}

QStringList SR865::getCurrentInputGainList() const
{
    return this->currrentInputGainList;
}

int SR865::currentInputGainNumberFromString(const QString &currentInputGain_string) const
{
    return numberFromString(this->currrentInputGainList, currentInputGain_string);
}

QString SR865::currentInputGainStringFromNumber(const int &currentInputGain_number) const
{
    return stringFromNumber(this->currrentInputGainList, currentInputGain_number);
}

bool SR865::setCurrentInputGain(const int &new_currentInputGain) const
{
    if (!isValidNumber(this->currrentInputGainList, new_currentInputGain))
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
    this->sensivityList.clear();

    this->sensivityList.push_back(  "1 V/uA");
    this->sensivityList.push_back("500 mV/nA");
    this->sensivityList.push_back("200 mV/nA");
    this->sensivityList.push_back("100 mV/nA");
    this->sensivityList.push_back( "50 mV/nA");
    this->sensivityList.push_back( "20 mV/nA");
    this->sensivityList.push_back( "10 mV/nA");
    this->sensivityList.push_back(  "5 mV/nA");
    this->sensivityList.push_back(  "2 mV/nA");
    this->sensivityList.push_back(  "1 mV/nA");
    this->sensivityList.push_back("500 uV/pA");
    this->sensivityList.push_back("200 uV/pA");
    this->sensivityList.push_back("100 uV/pA");
    this->sensivityList.push_back( "50 uV/pA");
    this->sensivityList.push_back( "20 uV/pA");
    this->sensivityList.push_back( "10 uV/pA");
    this->sensivityList.push_back(  "5 uV/pA");
    this->sensivityList.push_back(  "2 uV/pA");
    this->sensivityList.push_back(  "1 uV/pA");
    this->sensivityList.push_back("500 nV/fA");
    this->sensivityList.push_back("200 nV/fA");
    this->sensivityList.push_back("100 nV/fA");
    this->sensivityList.push_back( "50 nV/fA");
    this->sensivityList.push_back( "20 nV/fA");
    this->sensivityList.push_back( "10 nV/fA");
    this->sensivityList.push_back(  "5 nV/fA");
    this->sensivityList.push_back(  "2 nV/fA");
    this->sensivityList.push_back(  "1 nV/fA");

    return;
}

QStringList SR865::getSensivityList() const
{
    return this->sensivityList;
}

int SR865::sensivityNumberFromString(const QString &sensivity_string) const
{
    return numberFromString(this->sensivityList, sensivity_string);
}

QString SR865::sensivityStringFromNumber(const int &sensivity_number) const
{
    return stringFromNumber(this->sensivityList, sensivity_number);
}

bool SR865::setSensivity(const int &new_sensivity) const
{
    if (!isValidNumber(this->sensivityList, new_sensivity))
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
    this->timeConstantList.clear();

    this->timeConstantList.push_back(  "1 us");
    this->timeConstantList.push_back(  "3 us");
    this->timeConstantList.push_back( "10 us");
    this->timeConstantList.push_back( "30 us");
    this->timeConstantList.push_back("100 us");
    this->timeConstantList.push_back("300 us");
    this->timeConstantList.push_back(  "1 ms");
    this->timeConstantList.push_back(  "3 ms");
    this->timeConstantList.push_back( "10 ms");
    this->timeConstantList.push_back( "30 ms");
    this->timeConstantList.push_back("100 ms");
    this->timeConstantList.push_back("300 ms");
    this->timeConstantList.push_back(  "1 s");
    this->timeConstantList.push_back(  "3 s");
    this->timeConstantList.push_back( "10 s");
    this->timeConstantList.push_back( "30 s");
    this->timeConstantList.push_back("100 s");
    this->timeConstantList.push_back("300 s");
    this->timeConstantList.push_back(  "1 ks");
    this->timeConstantList.push_back(  "3 ks");
    this->timeConstantList.push_back( "10 ks");
    this->timeConstantList.push_back( "30 ks");

    return;
}

QStringList SR865::getTimeConstantList() const
{
    return this->timeConstantList;
}

int SR865::timeConstantNumberFromString(const QString &timeConstant_string) const
{
    return numberFromString(this->timeConstantList, timeConstant_string);
}

QString SR865::timeConstantStringFromNumber(const int &timeConstant_number) const
{
    return stringFromNumber(this->timeConstantList, timeConstant_number);
}

bool SR865::setTimeConstant(const int &new_timeConstant) const
{
    if (!isValidNumber(this->timeConstantList, new_timeConstant))
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
    this->filterList.clear();

    this->filterList.push_back( "6 dB/oct");
    this->filterList.push_back("12 dB/oct");
    this->filterList.push_back("18 dB/oct");
    this->filterList.push_back("24 dB/oct");

    return;
}

QStringList SR865::getFilterList() const
{
    return this->filterList;
}

int SR865::filterNumberFromString(const QString &filter_string) const
{
    return numberFromString(this->filterList, filter_string);
}

QString SR865::filterStringFromNumber(const int &filter_number) const
{
    return stringFromNumber(this->filterList, filter_number);
}

bool SR865::setFilter(const int &new_filter) const
{
    if (!isValidNumber(this->filterList, new_filter))
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

bool SR865::enableSynchronousFilter(const bool &enable) const
{
    QString command = "SYNC ";
    if (enable)
        command += "ON";
    else
        command += "OFF";

    return sendCommand(command);
}

bool SR865::getSynchronousFilterEnabled() const
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

bool SR865::getAdvancedFilterEnabled() const
{
    return ask("ADVFILT?").toInt();
}

bool SR865::autoPhase() const
{
   return sendCommand("APHS");
}

bool SR865::autoRange() const
{
    return sendCommand("ARNG");
}

bool SR865::autoScale() const
{
    return sendCommand("ASCL");
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
    if (isValidString(this->outputNumberListLongName, outp_string))
        return numberFromString(this->outputNumberListLongName, outp_string);
    if (isValidString(this->outputNumberListShortName, outp_string))
        return numberFromString(this->outputNumberListShortName, outp_string);
    return -1;
}

QString SR865::outpStringFromNumber(const int &outp_number) const
{
    return stringFromNumber(this->outputNumberListLongName, outp_number);
}

double SR865::getOUTP(const int &i) const
{
    if (!isValidNumber(this->outputNumberListLongName, i))
        return 0;

    QString command = "OUTP? " + QString::number(i);
    return ask(command).trimmed().toDouble();
}

double SR865::getOUTP(const QString &i) const
{
    if (isValidString(this->outputNumberListLongName, i))
        return getOUTP(numberFromString(this->outputNumberListLongName, i));
    if (isValidString(this->outputNumberListShortName, i))
        return getOUTP(numberFromString(this->outputNumberListShortName, i));
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
