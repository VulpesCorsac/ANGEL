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

    this->baudrateList.clear();
    this->referenceSourceList.clear();
    this->referenceTriggerModeList.clear();
    this->signalInputList.clear();
    this->voltageInputModeList.clear();
    this->voltageInputCouplingList.clear();
    this->voltageInputShieldsList.clear();
    this->notchFilterList.clear();
    this->sensivityList.clear();
    this->reserveModeList.clear();
    this->timeConstantList.clear();
    this->filterList.clear();
    this->sampleRateList.clear();
    this->bufferModeList.clear();
    this->outputNumberList.clear();
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

    connect(this->srs, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString))   , Qt::DirectConnection);
    connect(this->srs, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
    connect(this->srs, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)) , Qt::DirectConnection);
    connect(this->srs, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)) , Qt::DirectConnection);
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
    this->baudrateList.clear();

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

int SR830::numberFromString(const QStringList &list, const QString &string) const
{
    return this->srs->numberFromString(list, string);
}

QString SR830::stringFromNumber(const QStringList &list, const int &number) const
{
    return this->srs->stringFromNumber(list, number);
}

bool SR830::isValidString(const QStringList &list, const QString &string) const
{
    return this->srs->isValidString(list, string);
}

bool SR830::isValidNumber(const QStringList &list, const int &number) const
{
    return this->srs->isValidNumber(list, number);
}

double SR830::getMinInternalFrequency() const
{
    return this->minInternalFrequency;
}

double SR830::getMaxInternalFrequency() const
{
    return this->maxInternalFrequency;
}

bool SR830::isValidInternalFrequency(const double &new_frequency) const
{
    return (new_frequency >= this->minInternalFrequency &&
            new_frequency <= this->maxInternalFrequency);
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

double SR830::getMinSineOutAmplitude() const
{
    return this->minSineOutputAmplitude;
}

double SR830::getMaxSineOutAmplitude() const
{
    return this->maxSineOutputAmplitude;
}

bool SR830::isValidSineOutAmplitude(const double &new_sineAmplitude) const
{
    return (new_sineAmplitude >= this->minSineOutputAmplitude &&
            new_sineAmplitude <= this->maxSineOutputAmplitude);
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
    this->referenceSourceList.clear();

    this->referenceSourceList.push_back("EXT");
    this->referenceSourceList.push_back("INT");

    return;
}

QStringList SR830::getReferenceSourceList() const
{
    return this->referenceSourceList;
}

int SR830::refSourceNumberFromString(const QString &ref_string) const
{
    return numberFromString(this->referenceSourceList, ref_string);
}

QString SR830::refSourceStringFromNumber(const int &ref_number) const
{
    return stringFromNumber(this->referenceSourceList, ref_number);
}

bool SR830::setReferenceSource(const int &new_source) const
{
    if (!isValidNumber(this->referenceSourceList, new_source))
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
    this->referenceTriggerModeList.clear();

    this->referenceTriggerModeList.push_back("SIN");
    this->referenceTriggerModeList.push_back("TTL Rising");
    this->referenceTriggerModeList.push_back("TTL Falling");

    return;
}

QStringList SR830::getReferenceTriggerModeList() const
{
    return this->referenceTriggerModeList;
}

int SR830::refModeNumberFromString(const QString &mode_string) const
{
    return numberFromString(this->referenceTriggerModeList, mode_string);
}

QString SR830::refModeStringFromNumber(const int &mode_number) const
{
    return stringFromNumber(this->referenceTriggerModeList, mode_number);
}

bool SR830::setReferenceTriggerMode(const int &new_mode) const
{
    if (!isValidNumber(this->referenceTriggerModeList, new_mode))
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
    this->signalInputList.clear();

    this->signalInputList.push_back("Vol A");
    this->signalInputList.push_back("Vol A-B");
    this->signalInputList.push_back("Cur 1MOhm");
    this->signalInputList.push_back("Cur 100MOhm");

    return;
}

QStringList SR830::getSignalInputList() const
{
    return this->signalInputList;
}

int SR830::signalInputNumberFromString(const QString &signalInput_string) const
{
    return numberFromString(this->signalInputList, signalInput_string);
}

QString SR830::signalInputStringFromNumber(const int &signalInput_number) const
{
    return stringFromNumber(this->signalInputList, signalInput_number);
}

bool SR830::setSignalInput(const int &new_signalInput) const
{
    if (!isValidNumber(this->signalInputList, new_signalInput))
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
    this->voltageInputCouplingList.clear();

    this->voltageInputCouplingList.push_back("AC");
    this->voltageInputCouplingList.push_back("DC");

    return;
}

QStringList SR830::getVoltageInputCouplingList() const
{
    return this->voltageInputCouplingList;
}

int SR830::voltageInputCouplingNumberFromString(const QString &voltageInputCoupling_string) const
{
    return numberFromString(this->voltageInputCouplingList, voltageInputCoupling_string);
}

QString SR830::voltageInputCouplingStringFromNumber(const int &voltageInputCoupling_number) const
{
    return stringFromNumber(this->voltageInputCouplingList, voltageInputCoupling_number);
}

bool SR830::setVoltageInputCoupling(const int &new_voltageInputCoupling) const
{
    if (!isValidNumber(this->voltageInputCouplingList, new_voltageInputCoupling))
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
    this->voltageInputShieldsList.clear();

    this->voltageInputShieldsList.push_back("FLOAT");
    this->voltageInputShieldsList.push_back("GROUND");

    return;
}

QStringList SR830::getVoltageInputShieldsList() const
{
    return this->voltageInputShieldsList;
}

int SR830::voltageInputShieldsNumberFromString(const QString &voltageInputShields_string) const
{
    return numberFromString(this->voltageInputShieldsList, voltageInputShields_string);
}

QString SR830::voltageInputShieldsStringFromNumber(const int &voltageInputShields_number) const
{
    return stringFromNumber(this->voltageInputShieldsList, voltageInputShields_number);
}

bool SR830::setVoltageInputShields(const int &new_voltageInputShields) const
{
    if (!isValidNumber(this->voltageInputShieldsList, new_voltageInputShields))
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
    this->notchFilterList.clear();

    this->notchFilterList.push_back("No filter");
    this->notchFilterList.push_back("Line notch");
    this->notchFilterList.push_back("2xLine notch");
    this->notchFilterList.push_back("Both notch");

    return;
}

QStringList SR830::getInputLineNotchFilterList() const
{
    return this->notchFilterList;
}

int SR830::inputLineNotchFilterNumberFromString(const QString &inputLineNotchFilter_string) const
{
    return numberFromString(this->notchFilterList, inputLineNotchFilter_string);
}

QString SR830::inputLineNotchFilterStringFromNumber(const int &inputLineNotchFilter_number) const
{
    return stringFromNumber(this->notchFilterList, inputLineNotchFilter_number);
}

bool SR830::setInputLineNotchFilter(const int &new_inputLineNotchFilter) const
{
    if (!isValidNumber(this->notchFilterList, new_inputLineNotchFilter))
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
    this->sensivityList.clear();

    this->sensivityList.push_back(  "2 nV/fA");
    this->sensivityList.push_back(  "5 nV/fA");
    this->sensivityList.push_back( "10 nV/fA");
    this->sensivityList.push_back( "20 nV/fA");
    this->sensivityList.push_back( "50 nV/fA");
    this->sensivityList.push_back("100 nV/fA");
    this->sensivityList.push_back("200 nV/fA");
    this->sensivityList.push_back("500 nV/fA");
    this->sensivityList.push_back(  "1 uV/pA");
    this->sensivityList.push_back(  "2 uV/pA");
    this->sensivityList.push_back(  "5 uV/pA");
    this->sensivityList.push_back( "10 uV/pA");
    this->sensivityList.push_back( "20 uV/pA");
    this->sensivityList.push_back( "50 uV/pA");
    this->sensivityList.push_back("100 uV/pA");
    this->sensivityList.push_back("200 uV/pA");
    this->sensivityList.push_back("500 uV/pA");
    this->sensivityList.push_back(  "1 mV/nA");
    this->sensivityList.push_back(  "2 mV/nA");
    this->sensivityList.push_back(  "5 mV/nA");
    this->sensivityList.push_back( "10 mV/nA");
    this->sensivityList.push_back( "20 mV/nA");
    this->sensivityList.push_back( "50 mV/nA");
    this->sensivityList.push_back("100 mV/nA");
    this->sensivityList.push_back("200 mV/nA");
    this->sensivityList.push_back("500 mV/nA");
    this->sensivityList.push_back(  "1 V/uA");

    return;
}

QStringList SR830::getSensivityList() const
{
    return this->sensivityList;
}

int SR830::sensivityNumberFromString(const QString &sensivity_string) const
{
    return numberFromString(this->sensivityList, sensivity_string);
}

QString SR830::sensivityStringFromNumber(const int &sensivity_number) const
{
    return stringFromNumber(this->sensivityList, sensivity_number);
}

bool SR830::setSensivity(const int &new_sensivity) const
{
    if (!isValidNumber(this->sensivityList, new_sensivity))
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
    this->reserveModeList.clear();

    this->reserveModeList.push_back("High Reserve");
    this->reserveModeList.push_back("Normal");
    this->reserveModeList.push_back("Low Noise");

    return;
}

QStringList SR830::getReserveModeList() const
{
    return this->reserveModeList;
}

int SR830::reserveModeNumberFromString(const QString &reserveMode_string) const
{
    return numberFromString(this->reserveModeList, reserveMode_string);
}

QString SR830::reserveModeStringFromNumber(const int &reserveMode_number) const
{
    return stringFromNumber(this->reserveModeList, reserveMode_number);
}

bool SR830::setReserveMode(const int &new_reserveMode) const
{
    if (!isValidNumber(this->reserveModeList, new_reserveMode))
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
    this->timeConstantList.clear();

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

QStringList SR830::getTimeConstantList() const
{
    return this->timeConstantList;
}

int SR830::timeConstantNumberFromString(const QString &timeConstant_string) const
{
    return numberFromString(this->timeConstantList, timeConstant_string);
}

QString SR830::timeConstantStringFromNumber(const int &timeConstant_number) const
{
    return stringFromNumber(this->timeConstantList, timeConstant_number);
}

bool SR830::setTimeConstant(const int &new_timeConstant) const
{
    if (!isValidNumber(this->timeConstantList, new_timeConstant))
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
    this->filterList.clear();

    this->filterList.push_back( "6 dB/oct");
    this->filterList.push_back("12 dB/oct");
    this->filterList.push_back("18 dB/oct");
    this->filterList.push_back("24 dB/oct");

    return;
}

QStringList SR830::getFilterList() const
{
    return this->filterList;
}

int SR830::filterNumberFromString(const QString &filter_string) const
{
    return numberFromString(this->filterList, filter_string);
}

QString SR830::filterStringFromNumber(const int &filter_number) const
{
    return stringFromNumber(this->filterList, filter_number);
}

bool SR830::setFilter(const int &new_filter) const
{
    if (!isValidNumber(this->filterList, new_filter))
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

bool SR830::enableSynchronousFilter(const bool &enable) const
{
    QString command = "SYNC ";
    if (enable)
        command += "1";
    else
        command += "0";

    return sendCommand(command);
}

bool SR830::getSynchronousFilterEnabled() const
{
    return ask("SYNC?").toInt();
}

bool SR830::enableRS232outputInterface() const
{
    return sendCommand("OUTX 0");
}

bool SR830::autoGain() const
{
    return sendCommand("AGAN");
}

bool SR830::autoReserve() const
{
    return sendCommand("ARSV");
}

bool SR830::autoPhase() const
{
    return sendCommand("APHS");
}

bool SR830::autoOffset(const int &i) const
{
    if (i < 1 || i > 3)
        return false;

    return sendCommand("AGAN" + QString::number(i));
}

bool SR830::autoOffsetAll() const
{
    bool ans = true;

    for (int i = 1; i <= 3; i++)
        ans &= autoOffset(i);

    return ans;
}

void SR830::initSampleRateList()
{
    this->sampleRateList.clear();

    this->sampleRateList.push_back("62.5 mHz");
    this->sampleRateList.push_back( "125 mHz");
    this->sampleRateList.push_back( "250 mHz");
    this->sampleRateList.push_back( "500 mHz");
    this->sampleRateList.push_back(   "1 Hz");
    this->sampleRateList.push_back(   "2 Hz");
    this->sampleRateList.push_back(   "4 Hz");
    this->sampleRateList.push_back(   "8 Hz");
    this->sampleRateList.push_back(  "16 Hz");
    this->sampleRateList.push_back(  "32 Hz");
    this->sampleRateList.push_back(  "64 Hz");
    this->sampleRateList.push_back( "128 Hz");
    this->sampleRateList.push_back( "256 Hz");
    this->sampleRateList.push_back( "512 Hz");
    this->sampleRateList.push_back("Trigger");

    return;
}

QStringList SR830::getSampleRateList() const
{
    return this->sampleRateList;
}

int SR830::sampleRateNumberFromString(const QString &sampleRate_string) const
{
    return numberFromString(this->sampleRateList, sampleRate_string);
}

QString SR830::sampleRateStringFromNumber(const int &sampleRate_number) const
{
    return stringFromNumber(this->sampleRateList, sampleRate_number);
}

bool SR830::setSampleRate(const int &new_sampleRate) const
{
    if (!isValidNumber(this->sampleRateList, new_sampleRate))
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
    this->bufferModeList.clear();

    this->bufferModeList.push_back("Shot");
    this->bufferModeList.push_back("Loop");

    return;
}

QStringList SR830::getBufferModeList() const
{
    return this->bufferModeList;
}

int SR830::bufferModeNumberFromString(const QString &bufferMode_string) const
{
    return numberFromString(this->bufferModeList, bufferMode_string);
}

QString SR830::bufferModeStringFromNumber(const int &bufferMode_number) const
{
    return stringFromNumber(this->bufferModeList, bufferMode_number);
}

bool SR830::setBufferMode(const int &new_bufferMode) const
{
    if (!isValidNumber(this->bufferModeList, new_bufferMode))
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
    this->outputNumberList.clear();

    this->outputNumberList.push_back("X");
    this->outputNumberList.push_back("Y");
    this->outputNumberList.push_back("R");
    this->outputNumberList.push_back("Theta");
    this->outputNumberList.push_back("Aux In 1");
    this->outputNumberList.push_back("Aux In 2");
    this->outputNumberList.push_back("Aux In 3");
    this->outputNumberList.push_back("Aux In 4");
    this->outputNumberList.push_back("Reference Frequency");
    this->outputNumberList.push_back("CH1 display");
    this->outputNumberList.push_back("CH2 display");

    return;
}

QStringList SR830::getOutputNumberList() const
{
    return this->outputNumberList;
}

int SR830::outpNumberFromString(const QString &outp_string) const
{
    return numberFromString(this->outputNumberList, outp_string);
}

QString SR830::outpStringFromNumber(const int &outp_number) const
{
    return stringFromNumber(this->outputNumberList, outp_number);
}

double SR830::getOUTP(const int &i) const
{
    if (!isValidNumber(this->outputNumberList, i))
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
