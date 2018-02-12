#include "SR830.h"

SR830::SR830(QObject *parent) : QObject(parent)
{
    this->serial = nullptr;
    this->srs = nullptr;
    init();
}

SR830::SR830(const QString &portName, const int &new_baudrate)
{
    this->serial = nullptr;
    this->srs = nullptr;
    init();

    setConnection(portName, new_baudrate);
}

SR830::~SR830()
{
    if (this->serial->isOpen())
        this->serial->close();
    this->serial->~QSerialPort();

    this->srs->~SRS();

    baudrateList.clear();
    referenceSourceList.clear();
    referenceTriggerModeList.clear();
    signalInputList.clear();
    voltageInputModeList.clear();
    voltageInputCouplingList.clear();
    voltageInputShieldsList.clear();
    notchFilterList.clear();
    sensivityList.clear();
    reserveModeList.clear();
    timeConstantList.clear();
    filterList.clear();
    sampleRateList.clear();
    bufferModeList.clear();
    outputNumberList.clear();
}

void SR830::init()
{
    if (this->srs == nullptr)
        this->srs = new SRS();

    initBaudrateList();
    initReferenceSourceList();
    initReferenceTriggerModeList();
    initSignalInputList();
    initVoltageInputCouplingList();
    initVoltageInputShieldsList();
    initInputLineNotchFilterList();
    initSensivityList();
    initReserveModeList();
    initTimeConstantList();
    initFilterList();
    initSampleRateList();
    initBufferModeList();
    initOutputNumberList();

    connect(this->srs, SIGNAL(errorSignal(QString)), this, SLOT(catchErrorSignal(QString)));
    connect(this->srs, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)));
    connect(this->srs, SIGNAL(commandSignal(QString)), this, SLOT(catchCommandSignal(QString)));
    connect(this->srs, SIGNAL(timeoutSignal(QString)), this, SLOT(catchTimeoutSignal(QString)));
}

void SR830::setSerial(QSerialPort* new_serial)
{
    return this->srs->setSerial(&this->serial, new_serial);
}

QSerialPort* SR830::getSerial() const
{
    return this->srs->getSerial(this->serial);
}

bool SR830::setConnection(const QString &portName, const int &new_baudrate)
{
    return this->srs->setConnection(&this->serial, portName, new_baudrate);
}

void SR830::disconnect() const
{
    return this->srs->disconnect(this->serial);
}

void SR830::initBaudrateList()
{
    this->baudrateList.push_back( "1200");
    this->baudrateList.push_back( "2400");
    this->baudrateList.push_back( "4800");
    this->baudrateList.push_back( "9600");
    this->baudrateList.push_back("19200");

    return;
}

QStringList SR830::getBaudrateList() const
{
    return this->baudrateList;
}

bool SR830::isValidBaudrate(const int &new_baudrate) const
{
    return (baudrateList.indexOf(QString::number(new_baudrate)) != -1);
}

bool SR830::changeBaudrate(const int &new_baudrate)
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

bool SR830::isActive() const
{
    return this->srs->isActive(this->serial);
}

bool SR830::send(const QString &command, QString &response, const bool &wait_for_response) const
{
    return this->srs->send(this->serial, command, response, wait_for_response);
}

bool SR830::sendCommand(const QString &command) const
{
    return this->srs->sendCommand(this->serial, command);
}

bool SR830::sendQuery(const QString &command, QString &response) const
{
    return this->srs->sendQuery(this->serial, command, response);
}

QString SR830::ask(const QString &command) const
{
    return this->srs->ask(this->serial, command);
}

bool SR830::getIDN(QString &idn) const
{
    return this->srs->getIDN(this->serial, idn);
}

bool SR830::isValidInternalFrequency(const double &new_frequency) const
{
    return (new_frequency >= this->minInternalFrequency &&
            new_frequency <= this->maxInternalFrequency);
}

double SR830::getMinInternalFrequency() const
{
    return this->minInternalFrequency;
}

double SR830::getMaxInternalFrequency() const
{
    return this->maxInternalFrequency;
}

bool SR830::setInternalFrequency(const double &new_frequency) const
{
    QString command = "FREQ " + QString::number(new_frequency);
    if (isValidInternalFrequency(new_frequency))
        return sendCommand(command);
    else
        return false;
}

double SR830::getFrequency() const
{
    return ask("FREQ?").toDouble();
}

int SR830::getMinHarmonic() const
{
    return this->minHarmonic;
}

int SR830::getMaxHarmonic() const
{
    return this->maxHarmonic;
}

bool SR830::isValidHarmonic(const int &i) const
{
    return (i >= this->minHarmonic &&
            i <= this->maxHarmonic);
}

bool SR830::setHarmonic(const int &i) const
{
    QString command = "HARM " + QString::number(i);
    if (isValidHarmonic(i))
        return sendCommand(command);
    else
        return false;
}

int SR830::getHarmonic() const
{
    return ask("HARM?").toInt();
}

bool SR830::isValidSineOutAmplitude(const double &new_sineAmplitude) const
{
    return (new_sineAmplitude >= this->minSineOutputAmplitude &&
            new_sineAmplitude <= this->maxSineOutputAmplitude);
}

double SR830::getMinSineOutAmplitude() const
{
    return this->minSineOutputAmplitude;
}

double SR830::getMaxSineOutAmplitude() const
{
    return this->maxSineOutputAmplitude;
}

bool SR830::setSineOutAmplitude(const double &new_sineAmplitude) const
{
    QString command = "SLVL " + QString::number(new_sineAmplitude);
    if (isValidSineOutAmplitude(new_sineAmplitude))
        return sendCommand(command);
    else
        return false;
}

double SR830::getSineOutAmplitude() const
{
    return ask("SLVL?").toDouble();
}

void SR830::initReferenceSourceList()
{
    referenceSourceList.push_back("INT");
    referenceSourceList.push_back("EXT");

    return;
}

QStringList SR830::getReferenceSourceList() const
{
    return this->referenceSourceList;
}

int SR830::refSourceNumberFromString(const QString &ref_string) const
{
    if (referenceSourceList.indexOf(ref_string.trimmed()) != -1)
        return referenceSourceList.indexOf(ref_string.trimmed());
    return -1;
}

QString SR830::refSourceStringFromNumber(const int &ref_number) const
{
    if (ref_number >= 0 &&
        ref_number < referenceSourceList.length())
        return referenceSourceList.at(ref_number);
    return "";
}

bool SR830::setReferenceSource(const int &new_source) const
{
    if (new_source < 0 || new_source >= referenceSourceList.size())
        return false;

    QString command = "FMOD " + QString::number(new_source);
    return sendCommand(command);
}

bool SR830::setReferenceSource(const QString &new_source) const
{
    return setReferenceSource(refSourceNumberFromString(new_source));
}

QString SR830::getReferenceSource() const
{
    return refSourceStringFromNumber(ask("FMOD?").toInt());
}

void SR830::initReferenceTriggerModeList()
{
    referenceTriggerModeList.push_back("SIN");
    referenceTriggerModeList.push_back("TTL Rising");
    referenceTriggerModeList.push_back("TTL Falling");

    return;
}

QStringList SR830::getReferenceTriggerModeList() const
{
    return this->referenceTriggerModeList;
}

int SR830::refModeNumberFromString(const QString &mode_string) const
{
    if (referenceTriggerModeList.indexOf(mode_string.trimmed()) != -1)
        return referenceTriggerModeList.indexOf(mode_string.trimmed());
    return -1;
}

QString SR830::refModeStringFromNumber(const int &mode_number) const
{
    if (mode_number >= 0 &&
        mode_number < referenceTriggerModeList.length())
        return referenceTriggerModeList.at(mode_number);
    return "";
}

bool SR830::setReferenceTriggerMode(const int &new_mode) const
{
    if (new_mode < 0 || new_mode >= referenceTriggerModeList.size())
        return false;

    QString command = "RSLP " + QString::number(new_mode);
    return sendCommand(command);
}

bool SR830::setReferenceTriggerMode(const QString &new_mode) const
{
    return setReferenceTriggerMode(refModeNumberFromString(new_mode));
}

QString SR830::getReferenceTriggerMode() const
{
    return refModeStringFromNumber(ask("RSLP?").toInt());
}

void SR830::initSignalInputList()
{
    signalInputList.push_back("Vol A");
    signalInputList.push_back("Vol A-B");
    signalInputList.push_back("Cur 1MOhm");
    signalInputList.push_back("Cur 100MOhm");

    return;
}

QStringList SR830::getSignalInputList() const
{
    return this->signalInputList;
}

int SR830::signalInputNumberFromString(const QString &signalInput_string) const
{
    if (signalInputList.indexOf(signalInput_string.trimmed()) != -1)
        return signalInputList.indexOf(signalInput_string.trimmed());
    return -1;
}

QString SR830::signalInputStringFromNumber(const int &signalInput_number) const
{
    if (signalInput_number >= 0 &&
        signalInput_number < signalInputList.length())
        return signalInputList.at(signalInput_number);
    return "";
}

bool SR830::setSignalInput(const int &new_signalInput) const
{
    if (new_signalInput < 0 || new_signalInput >= signalInputList.size())
        return false;

    QString command = "ISRC " + QString::number(new_signalInput);
    return sendCommand(command);
}

bool SR830::setSignalInput(const QString &new_signalInput) const
{
    return setSignalInput(signalInputNumberFromString(new_signalInput));
}

QString SR830::getSignalInput() const
{
    return signalInputStringFromNumber(ask("ISRC?").toInt());
}

void SR830::initVoltageInputCouplingList()
{
    voltageInputCouplingList.push_back("AC");
    voltageInputCouplingList.push_back("DC");

    return;
}

QStringList SR830::getVoltageInputCouplingList() const
{
    return this->voltageInputCouplingList;
}

int SR830::voltageInputCouplingNumberFromString(const QString &voltageInputCoupling_string) const
{
    if (voltageInputCouplingList.indexOf(voltageInputCoupling_string.trimmed()) != -1)
        return voltageInputCouplingList.indexOf(voltageInputCoupling_string.trimmed());
    return -1;
}

QString SR830::voltageInputCouplingStringFromNumber(const int &voltageInputCoupling_number) const
{
    if (voltageInputCoupling_number >= 0 &&
        voltageInputCoupling_number < voltageInputCouplingList.length())
        return voltageInputCouplingList.at(voltageInputCoupling_number);
    return "";
}

bool SR830::setVoltageInputCoupling(const int &new_voltageInputCoupling) const
{
    if (new_voltageInputCoupling < 0 || new_voltageInputCoupling >= voltageInputCouplingList.size())
        return false;

    QString command = "ICPL " + QString::number(new_voltageInputCoupling);
    return sendCommand(command);
}

bool SR830::setVoltageInputCoupling(const QString &new_voltageInputCoupling) const
{
    return setVoltageInputCoupling(voltageInputCouplingNumberFromString(new_voltageInputCoupling));
}

QString SR830::getVoltageInputCoupling() const
{
    return voltageInputCouplingStringFromNumber(ask("ICPL?").toInt());
}

void SR830::initVoltageInputShieldsList()
{
    voltageInputShieldsList.push_back("FLOAT");
    voltageInputShieldsList.push_back("GROUND");

    return;
}

QStringList SR830::getVoltageInputShieldsList() const
{
    return this->voltageInputShieldsList;
}

int SR830::voltageInputShieldsNumberFromString(const QString &voltageInputShields_string) const
{
    if (voltageInputShieldsList.indexOf(voltageInputShields_string.trimmed()) != -1)
        return voltageInputShieldsList.indexOf(voltageInputShields_string.trimmed());
    return -1;
}

QString SR830::voltageInputShieldsStringFromNumber(const int &voltageInputShields_number) const
{
    if (voltageInputShields_number >= 0 &&
        voltageInputShields_number < voltageInputShieldsList.length())
        return voltageInputShieldsList.at(voltageInputShields_number);
    return "";
}

bool SR830::setVoltageInputShields(const int &new_voltageInputShields) const
{
    if (new_voltageInputShields < 0 || new_voltageInputShields >= voltageInputShieldsList.size())
        return false;

    QString command = "IGND " + QString::number(new_voltageInputShields);
    return sendCommand(command);
}

bool SR830::setVoltageInputShields(const QString &new_voltageInputShields) const
{
    return setVoltageInputShields(voltageInputShieldsNumberFromString(new_voltageInputShields));
}

QString SR830::getVoltageInputShields() const
{
    return voltageInputShieldsStringFromNumber(ask("IGND?").toInt());
}

void SR830::initInputLineNotchFilterList()
{
    notchFilterList.push_back("No filter");
    notchFilterList.push_back("Line notch");
    notchFilterList.push_back("2xLine notch");
    notchFilterList.push_back("Both notch");

    return;
}

QStringList SR830::getInputLineNotchFilterList() const
{
    return this->notchFilterList;
}

int SR830::inputLineNotchFilterNumberFromString(const QString &inputLineNotchFilter_string) const
{
    if (notchFilterList.indexOf(inputLineNotchFilter_string.trimmed()) != -1)
        return notchFilterList.indexOf(inputLineNotchFilter_string.trimmed());
    return -1;
}

QString SR830::inputLineNotchFilterStringFromNumber(const int &inputLineNotchFilter_number) const
{
    if (inputLineNotchFilter_number >= 0 &&
        inputLineNotchFilter_number < notchFilterList.length())
        return notchFilterList.at(inputLineNotchFilter_number);
    return "";
}

bool SR830::setInputLineNotchFilter(const int &new_inputLineNotchFilter) const
{
    if (new_inputLineNotchFilter < 0 || new_inputLineNotchFilter >= notchFilterList.size())
        return false;

    QString command = "ILIN " + QString::number(new_inputLineNotchFilter);
    return sendCommand(command);
}

bool SR830::setInputLineNotchFilter(const QString &new_inputLineNotchFilter) const
{
    return setInputLineNotchFilter(inputLineNotchFilterNumberFromString(new_inputLineNotchFilter));
}

QString SR830::getInputLineNotchFilter() const
{
    return inputLineNotchFilterStringFromNumber(ask("ILIN?").toInt());
}

void SR830::initSensivityList()
{
    sensivityList.push_back(  "2 nV/fA");
    sensivityList.push_back(  "5 nV/fA");
    sensivityList.push_back( "10 nV/fA");
    sensivityList.push_back( "20 nV/fA");
    sensivityList.push_back( "50 nV/fA");
    sensivityList.push_back("100 nV/fA");
    sensivityList.push_back("200 nV/fA");
    sensivityList.push_back("500 nV/fA");
    sensivityList.push_back(  "1 uV/pA");
    sensivityList.push_back(  "2 uV/pA");
    sensivityList.push_back(  "5 uV/pA");
    sensivityList.push_back( "10 uV/pA");
    sensivityList.push_back( "20 uV/pA");
    sensivityList.push_back( "50 uV/pA");
    sensivityList.push_back("100 uV/pA");
    sensivityList.push_back("200 uV/pA");
    sensivityList.push_back("500 uV/pA");
    sensivityList.push_back(  "1 mV/nA");
    sensivityList.push_back(  "2 mV/nA");
    sensivityList.push_back(  "5 mV/nA");
    sensivityList.push_back( "10 mV/nA");
    sensivityList.push_back( "20 mV/nA");
    sensivityList.push_back( "50 mV/nA");
    sensivityList.push_back("100 mV/nA");
    sensivityList.push_back("200 mV/nA");
    sensivityList.push_back("500 mV/nA");
    sensivityList.push_back(  "1 V/uA");

    return;
}

QStringList SR830::getSensivityList() const
{
    return this->sensivityList;
}

int SR830::sensivityNumberFromString(const QString &sensivity_string) const
{
    if (sensivityList.indexOf(sensivity_string.trimmed()) != -1)
        return sensivityList.indexOf(sensivity_string.trimmed());
    return -1;
}

QString SR830::sensivityStringFromNumber(const int &sensivity_number) const
{
    if (sensivity_number >= 0 &&
        sensivity_number < sensivityList.length())
        return sensivityList.at(sensivity_number);
    return "";
}

bool SR830::setSensivity(const int &new_sensivity) const
{
    if (new_sensivity < 0 || new_sensivity >= sensivityList.size())
        return false;

    QString command = "SENS " + QString::number(new_sensivity);
    return sendCommand(command);
}

bool SR830::setSensivity(const QString &new_sensivity) const
{
    return setSensivity(sensivityNumberFromString(new_sensivity));
}

QString SR830::getSensivity() const
{
    return sensivityStringFromNumber(ask("SENS?").toInt());
}

void SR830::initReserveModeList()
{
    reserveModeList.push_back("High Reserve");
    reserveModeList.push_back("Normal");
    reserveModeList.push_back("Low Noise");

    return;
}

QStringList SR830::getReserveModeList() const
{
    return this->reserveModeList;
}

int SR830::reserveModeNumberFromString(const QString &reserveMode_string) const
{
    if (reserveModeList.indexOf(reserveMode_string.trimmed()) != -1)
        return reserveModeList.indexOf(reserveMode_string.trimmed());
    return -1;
}

QString SR830::reserveModeStringFromNumber(const int &reserveMode_number) const
{
    if (reserveMode_number >= 0 &&
        reserveMode_number < reserveModeList.length())
        return reserveModeList.at(reserveMode_number);
    return "";
}

bool SR830::setReserveMode(const int &new_reserveMode) const
{
    if (new_reserveMode < 0 || new_reserveMode >= reserveModeList.size())
        return false;

    QString command = "RMOD " + QString::number(new_reserveMode);
    return sendCommand(command);
}

bool SR830::setReserveMode(const QString &new_reserveMode) const
{
    return setReserveMode(reserveModeNumberFromString(new_reserveMode));
}

QString SR830::getReserveMode() const
{
    return reserveModeStringFromNumber(ask("RMOD?").toInt());
}

void SR830::initTimeConstantList()
{
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

QStringList SR830::getTimeConstantList() const
{
    return this->timeConstantList;
}

int SR830::timeConstantNumberFromString(const QString &timeConstant_string) const
{
    if (timeConstantList.indexOf(timeConstant_string.trimmed()) != -1)
        return timeConstantList.indexOf(timeConstant_string.trimmed());
    return -1;
}

QString SR830::timeConstantStringFromNumber(const int &timeConstant_number) const
{
    if (timeConstant_number >= 0 &&
        timeConstant_number < timeConstantList.length())
        return timeConstantList.at(timeConstant_number);
    return "";
}

bool SR830::setTimeConstant(const int &new_timeConstant) const
{
    if (new_timeConstant < 0 || new_timeConstant >= timeConstantList.size())
        return false;

    QString command = "OFLT " + QString::number(new_timeConstant);
    return sendCommand(command);
}

bool SR830::setTimeConstant(const QString &new_timeConstant) const
{
    return setTimeConstant(timeConstantNumberFromString(new_timeConstant));
}

QString SR830::getTimeConstant() const
{
    return timeConstantStringFromNumber(ask("OFLT?").toInt());
}

void SR830::initFilterList()
{
    filterList.push_back( "6 dB/oct");
    filterList.push_back("12 dB/oct");
    filterList.push_back("18 dB/oct");
    filterList.push_back("24 dB/oct");

    return;
}

QStringList SR830::getFilterList() const
{
    return this->filterList;
}

int SR830::filterNumberFromString(const QString &filter_string) const
{
    if (filterList.indexOf(filter_string.trimmed()) != -1)
        return filterList.indexOf(filter_string.trimmed());
    return -1;
}

QString SR830::filterStringFromNumber(const int &filter_number) const
{
    if (filter_number >= 0 &&
        filter_number < filterList.length())
        return filterList.at(filter_number);
    return "";
}

bool SR830::setFilter(const int &new_filter) const
{
    if (new_filter < 0 || new_filter >= filterList.size())
        return false;

    QString command = "OFSL " + QString::number(new_filter);
    return sendCommand(command);
}

bool SR830::setFilter(const QString &new_filter) const
{
    return setFilter(filterNumberFromString(new_filter));
}

QString SR830::getFilter() const
{
    return filterStringFromNumber(ask("OFSL?").toInt());
}

bool SR830::enableSychronousFilter(const bool &enable) const
{
    QString command = "SYNC ";
    if (enable)
        command += "1";
    else
        command += "0";

    return sendCommand(command);
}

bool SR830::isSychronousFilterEnabled() const
{
    return ask("SYNC?").toInt();
}

bool SR830::enableRS232outputInterface() const
{
    return sendCommand("OUTX 0");
}

void SR830::initSampleRateList()
{
    sampleRateList.push_back("62.5 mHz");
    sampleRateList.push_back( "125 mHz");
    sampleRateList.push_back( "250 mHz");
    sampleRateList.push_back( "500 mHz");
    sampleRateList.push_back(   "1 Hz");
    sampleRateList.push_back(   "2 Hz");
    sampleRateList.push_back(   "4 Hz");
    sampleRateList.push_back(   "8 Hz");
    sampleRateList.push_back(  "16 Hz");
    sampleRateList.push_back(  "32 Hz");
    sampleRateList.push_back(  "64 Hz");
    sampleRateList.push_back( "128 Hz");
    sampleRateList.push_back( "256 Hz");
    sampleRateList.push_back( "512 Hz");
    sampleRateList.push_back("Trigger");

    return;
}

QStringList SR830::getSampleRateList() const
{
    return this->sampleRateList;
}

int SR830::sampleRateNumberFromString(const QString &sampleRate_string) const
{
    if (sampleRateList.indexOf(sampleRate_string.trimmed()) != -1)
        return sampleRateList.indexOf(sampleRate_string.trimmed());
    return -1;
}

QString SR830::sampleRateStringFromNumber(const int &sampleRate_number) const
{
    if (sampleRate_number >= 0 &&
        sampleRate_number < sampleRateList.length())
        return sampleRateList.at(sampleRate_number);
    return "";
}

bool SR830::setSampleRate(const int &new_sampleRate) const
{
    if (new_sampleRate < 0 || new_sampleRate >= filterList.size())
        return false;

    QString command = "SRAT " + QString::number(new_sampleRate);
    return sendCommand(command);
}

bool SR830::setSampleRate(const QString &new_sampleRate) const
{
    return setSampleRate(sampleRateNumberFromString(new_sampleRate));
}

QString SR830::getSampleRate() const
{
    return sampleRateStringFromNumber(ask("SRAT?").toInt());
}

void SR830::initBufferModeList()
{
    bufferModeList.push_back("Shot");
    bufferModeList.push_back("Loop");

    return;
}

QStringList SR830::getBufferModeList() const
{
    return this->bufferModeList;
}

int SR830::bufferModeNumberFromString(const QString &bufferMode_string) const
{
    if (bufferModeList.indexOf(bufferMode_string.trimmed()) != -1)
        return bufferModeList.indexOf(bufferMode_string.trimmed());
    return -1;
}

QString SR830::bufferModeStringFromNumber(const int &bufferMode_number) const
{
    if (bufferMode_number >= 0 &&
        bufferMode_number < bufferModeList.length())
        return bufferModeList.at(bufferMode_number);
    return "";
}

bool SR830::setBufferMode(const int &new_bufferMode) const
{
    if (new_bufferMode < 0 || new_bufferMode >= bufferModeList.size())
        return false;

    QString command = "SEND " + QString::number(new_bufferMode);
    return sendCommand(command);
}

bool SR830::setBufferMode(const QString &new_bufferMode) const
{
    return setBufferMode(bufferModeNumberFromString(new_bufferMode));
}

QString SR830::getBufferMode() const
{
    return bufferModeStringFromNumber(ask("SEND?").toInt());
}

bool SR830::startBuffer() const
{
    return sendCommand("STRT");
}

bool SR830::pauseBuffer() const
{
    return sendCommand("PAUS");
}

bool SR830::stopBuffer() const
{
    return sendCommand("REST");
}

void SR830::initOutputNumberList()
{
    outputNumberList.push_back("X");
    outputNumberList.push_back("Y");
    outputNumberList.push_back("R");
    outputNumberList.push_back("Theta");
    outputNumberList.push_back("Aux In 1");
    outputNumberList.push_back("Aux In 2");
    outputNumberList.push_back("Aux In 3");
    outputNumberList.push_back("Aux In 4");
    outputNumberList.push_back("Reference Frequency");
    outputNumberList.push_back("CH1 display");
    outputNumberList.push_back("CH2 display");

    return;
}

QStringList SR830::getOutputNumberList() const
{
    return this->outputNumberList;
}

int SR830::outpNumberFromString(const QString &outp_string) const
{
    if (outputNumberList.indexOf(outp_string.trimmed()) != -1)
        return outputNumberList.indexOf(outp_string);
    return -1;
}

QString SR830::outpStringFromNumber(const int &outp_number) const
{
    if (outp_number >= 0 &&
        outp_number < outputNumberList.length())
        return outputNumberList.at(outp_number);
    return "";
}

double SR830::getOUTP(const int &i) const
{
    if (i < 0 || i >= outputNumberList.size())
        return 0;

    QString command = "OUTP? " + QString::number(i);
    QString response;
    sendQuery(command, response);
    return response.trimmed().toDouble();
}

double SR830::getOUTP(const QString &i) const
{
    if (outputNumberList.indexOf(i) != -1)
        return getOUTP(outputNumberList.indexOf(i));
    return 0;
}

double SR830::getX() const
{
    return getOUTP(1);
}

double SR830::getY() const
{
    return getOUTP(2);
}

double SR830::getR() const
{
    return getOUTP(3);
}

double SR830::getTheta() const
{
    return getOUTP(4);
}

bool SR830::getAB(const int & Apos, double &A, const int &Bpos, double &B) const
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

bool SR830::getABC(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C) const
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

bool SR830::getABCDE(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C, const int &Dpos, double &D, const int &Epos, double &E) const
{
    QString command = "SNAP? " + QString::number(Apos) + "," + QString::number(Bpos) + "," + QString::number(Cpos) + "," + QString::number(Dpos) + "," + QString::number(Epos);
    QString response;
    if (!sendQuery(command, response))
        return false;

    QStringList responseList = response.trimmed().split(',');
    QString _a = responseList.at(0);
    QString _b = responseList.at(1);
    QString _c = responseList.at(2);
    QString _d = responseList.at(3);
    QString _e = responseList.at(4);
    A = _a.trimmed().toDouble();
    B = _b.trimmed().toDouble();
    C = _c.trimmed().toDouble();
    D = _d.trimmed().toDouble();
    E = _e.trimmed().toDouble();

    return true;
}

bool SR830::getABCDEF(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C, const int &Dpos, double &D, const int &Epos, double &E, const int &Fpos, double &F) const
{
    QString command = "SNAP? " + QString::number(Apos) + "," + QString::number(Bpos) + "," + QString::number(Cpos) + "," + QString::number(Dpos) + "," + QString::number(Epos) + "," + QString::number(Fpos);
    QString response;
    if (!sendQuery(command, response))
        return false;

    QStringList responseList = response.trimmed().split(',');
    QString _a = responseList.at(0);
    QString _b = responseList.at(1);
    QString _c = responseList.at(2);
    QString _d = responseList.at(3);
    QString _e = responseList.at(4);
    QString _f = responseList.at(5);
    A = _a.trimmed().toDouble();
    B = _b.trimmed().toDouble();
    C = _c.trimmed().toDouble();
    D = _d.trimmed().toDouble();
    E = _e.trimmed().toDouble();
    F = _f.trimmed().toDouble();

    return true;
}

bool SR830::getXY(double &X, double &Y) const
{
    return getAB(1, X, 2, Y);
}

bool SR830::getRTheta(double &R, double &Theta) const
{
    return getAB(3, R, 4, Theta);
}

bool SR830::getRThetaF(double &R, double &Theta, double &F) const
{
    return getABC(3, R, 4, Theta, 9, F);
}

bool SR830::getXYRThetaF(double &X, double&Y, double &R, double &Theta, double &F) const
{
    return getABCDE(1, X, 2, Y, 3, R, 4, Theta, 9, F);
}

bool SR830::setDisplayData(const int &channel, const int &data) const
{
    return sendCommand("DDEF " + QString::number(channel) + "," + QString::number(data));
}

int SR830:: getBufferSize() const
{
    return ask("SPTS?").toInt();
}

double SR830::getPointFromBuffer(const int &channel, const int &number) const
{
    return ask("TRCA? " + QString::number(channel) + "," + QString::number(number) + ",1").toDouble();
}

std::vector < double > SR830::getChannelFromBuffer(const int &channel) const
{
    std::vector < double > ans;

    int size = getBufferSize();
    for (int i = 0; i < size; i++) {
        ans.push_back(getPointFromBuffer(channel, i));
    }

    return ans;
}

int SR830::getBuffer(std::vector<double> &ch1, std::vector<double> &ch2) const
{
    ch1.clear();
    ch2.clear();

    int size = getBufferSize();
    for (int i = 0; i < size; i++) {
        ch1.push_back(getPointFromBuffer(1, i));
        ch2.push_back(getPointFromBuffer(2, i));
    }

    return size;
}

void SR830::catchErrorSignal(const QString &s) const
{
    emit this->errorSignal(s);
}

void SR830::catchResponseSignal(const QString &s) const
{
    emit this->responseSignal(s);
}

void SR830::catchCommandSignal(const QString &s) const
{
    emit this->commandSignal(s);
}

void SR830::catchTimeoutSignal(const QString &s) const
{
    emit this->timeoutSignal(s);
}
