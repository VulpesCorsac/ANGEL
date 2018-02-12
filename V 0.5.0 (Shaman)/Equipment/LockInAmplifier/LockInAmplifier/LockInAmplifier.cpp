#include "LockInAmplifier.h"

LockInAmplifier::LockInAmplifier(QObject *parent) : QObject(parent)
{
    toNULL();
}

LockInAmplifier::LockInAmplifier(const QString &portName, const int &new_baudrate, const QString &new_lockInAmplifierType)
{
    toNULL();
    setConnection(portName, new_baudrate, new_lockInAmplifierType);
}

LockInAmplifier::LockInAmplifier(QSerialPort *serial, const QString &new_lockInAmplifierType)
{
    toNULL();
    if (new_lockInAmplifierType.trimmed().toUpper() == "SR830") {
        if (this->sr830 == nullptr)
            this->sr830 = new SR830(serial->portName(), serial->baudRate());
        else
            this->sr830->setSerial(serial);
    }
    if (new_lockInAmplifierType.trimmed().toUpper() == "SR844") {
        if (this->sr844 == nullptr)
            this->sr844 = new SR844(serial->portName(), serial->baudRate());
        else
            this->sr844->setSerial(serial);
    }
    if (new_lockInAmplifierType.trimmed().toUpper() == "SR865") {
        if (this->sr865 == nullptr)
            this->sr865 = new SR865(serial->portName(), serial->baudRate());
        else
            this->sr865->setSerial(serial);
    }
    if (new_lockInAmplifierType.trimmed().isEmpty() ||
        new_lockInAmplifierType.trimmed().toUpper() == "AUTO") {
        autoSetLockInAmplifierType(serial);
    }
}

LockInAmplifier::LockInAmplifier(SR830* new_sr830)
{
    toNULL();
    setSR830(new_sr830);
}

LockInAmplifier::LockInAmplifier(SR844* new_sr844)
{
    toNULL();
    setSR844(new_sr844);
}

LockInAmplifier::LockInAmplifier(SR865* new_sr865)
{
    toNULL();
    setSR865(new_sr865);
}

LockInAmplifier::~LockInAmplifier()
{
    this->lockInAmplifierType = "";
    if (this->sr830 != nullptr)
        this->sr830->~SR830();
    if (this->sr844 != nullptr)
        this->sr844->~SR844();
    if (this->sr865 != nullptr)
        this->sr865->~SR865();
    this->sr830 = nullptr;
    this->sr844 = nullptr;
    this->sr865 = nullptr;
}

void LockInAmplifier::toNULL()
{
    if (this->sr830 != nullptr) {
        this->sr830 = nullptr;
    }
    if (this->sr844 != nullptr) {
        this->sr844 = nullptr;
    }
    if (this->sr865 != nullptr) {
        this->sr865 = nullptr;
    }
    if (this->srs != nullptr) {
        this->srs = nullptr;
    }

    init();

    return;
}

void LockInAmplifier::init()
{
    if (this->srs == nullptr)
        this->srs = new SRS();

    if (this->srs != nullptr) {
        connect(this->srs, SIGNAL(errorSignal(QString)), this, SLOT(catchErrorSignal(QString)), Qt::DirectConnection);
        connect(this->srs, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->srs, SIGNAL(commandSignal(QString)), this, SLOT(catchCommandSignal(QString)), Qt::DirectConnection);
        connect(this->srs, SIGNAL(timeoutSignal(QString)), this, SLOT(catchTimeoutSignal(QString)), Qt::DirectConnection);
    }
    if (this->sr830 != nullptr) {
        connect(this->sr830, SIGNAL(errorSignal(QString)), this, SLOT(catchErrorSignal(QString)), Qt::DirectConnection);
        connect(this->sr830, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->sr830, SIGNAL(commandSignal(QString)), this, SLOT(catchCommandSignal(QString)), Qt::DirectConnection);
        connect(this->sr830, SIGNAL(timeoutSignal(QString)), this, SLOT(catchTimeoutSignal(QString)), Qt::DirectConnection);
    }
    if (this->sr844 != nullptr) {
        connect(this->sr844, SIGNAL(errorSignal(QString)), this, SLOT(catchErrorSignal(QString)), Qt::DirectConnection);
        connect(this->sr844, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->sr844, SIGNAL(commandSignal(QString)), this, SLOT(catchCommandSignal(QString)), Qt::DirectConnection);
        connect(this->sr844, SIGNAL(timeoutSignal(QString)), this, SLOT(catchTimeoutSignal(QString)), Qt::DirectConnection);
    }
    if (this->sr865 != nullptr) {
        connect(this->sr865, SIGNAL(errorSignal(QString)), this, SLOT(catchErrorSignal(QString)), Qt::DirectConnection);
        connect(this->sr865, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->sr865, SIGNAL(commandSignal(QString)), this, SLOT(catchCommandSignal(QString)), Qt::DirectConnection);
        connect(this->sr865, SIGNAL(timeoutSignal(QString)), this, SLOT(catchTimeoutSignal(QString)), Qt::DirectConnection);
    }

    return;
}

void LockInAmplifier::setSR830(SR830* new_sr830)
{
    this->sr830 = new_sr830;
    init();

    return;
}

SR830* LockInAmplifier::getSR830() const
{
    return this->sr830;
}

void LockInAmplifier::setSR844(SR844* new_sr844)
{
    this->sr844 = new_sr844;
    init();

    return;
}

SR844* LockInAmplifier::getSR844() const
{
    return this->sr844;
}

void LockInAmplifier::setSR865(SR865* new_sr865)
{
    this->sr865 = new_sr865;
    init();

    return;
}

SR865* LockInAmplifier::getSR865() const
{
    return this->sr865;
}

bool LockInAmplifier::openSerial(QSerialPort *serial) const
{
    return this->srs->openSerial(serial);
}

QStringList LockInAmplifier::getSupportedTypes() const
{
    QStringList ans;

    ans.push_back("SR830");
    ans.push_back("SR844");
    ans.push_back("SR865");
    ans.push_back("AUTO");

    return ans;
}

void LockInAmplifier::setLockInAmplifierType(const QString &new_lockInAmplifierType)
{
    this->lockInAmplifierType = new_lockInAmplifierType;
}

QString LockInAmplifier::getLockInAmplifierType() const
{
    return this->lockInAmplifierType;
}

bool LockInAmplifier::autoSetLockInAmplifierType(QSerialPort *serial)
{

}

bool LockInAmplifier::setConnection(const QString &portName, const int &new_baudrate, const QString &new_lockInAmplifierType)
{
    if (new_lockInAmplifierType.trimmed().toUpper() == "SR830") {
        if (this->sr830 == nullptr)
            this->sr830 = new SR830();
        this->lockInAmplifierType = "SR830";
        return this->sr830->setConnection(portName, new_baudrate);
    }
    if (new_lockInAmplifierType.trimmed().toUpper() == "SR844") {
        if (this->sr844 == nullptr)
            this->sr844 = new SR844();
        this->lockInAmplifierType = "SR844";
        return this->sr844->setConnection(portName, new_baudrate);
    }
    if (new_lockInAmplifierType.trimmed().toUpper() == "SR865") {
        if (this->sr865 == nullptr)
            this->sr865 = new SR865();
        this->lockInAmplifierType = "SR865";
        return this->sr865->setConnection(portName, new_baudrate);
    }
    if (new_lockInAmplifierType.trimmed().isEmpty() ||
        new_lockInAmplifierType.trimmed().toUpper() == "AUTO") {
        QSerialPort *serial = new QSerialPort();
        serial->setPortName(portName);
        serial->setBaudRate(new_baudrate);
        return autoSetLockInAmplifierType(serial);
    }
    return false;
}

bool LockInAmplifier::disconnect() const
{
    if (this->lockInAmplifierType == "SR830") {
        this->sr830->disconnect();
        return true;
    }
    if (this->lockInAmplifierType == "SR844") {
        this->sr844->disconnect();
        return true;
    }
    if (this->lockInAmplifierType == "SR865") {
        this->sr865->disconnect();
        return true;
    }
    return false;
}

QStringList LockInAmplifier::getBaudrateList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getBaudrateList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getBaudrateList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getBaudrateList();
    return QStringList();
}

bool LockInAmplifier::isValidBaudrateAll(const int &new_baudrate) const
{
    SR830 temp830;
    SR844 temp844;
    SR865 temp865;
    bool answer = temp830.isValidBaudrate(new_baudrate);
    answer &= temp844.isValidBaudrate(new_baudrate);
    answer &= temp865.isValidBaudrate(new_baudrate);
    return answer;
}

bool LockInAmplifier::isValidBaudrate(const int &new_baudrate) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->isValidBaudrate(new_baudrate);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->isValidBaudrate(new_baudrate);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidBaudrate(new_baudrate);
    return false;
}

bool LockInAmplifier::changeBaudrate(const int &new_baudrate)
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->changeBaudrate(new_baudrate);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->changeBaudrate(new_baudrate);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->changeBaudrate(new_baudrate);
    return false;
}

bool LockInAmplifier::isActive() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->isActive();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->isActive();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isActive();
    return false;
}

bool LockInAmplifier::send(const QString &command, QString &response, const bool &wait_for_response) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->send(command, response, wait_for_response);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->send(command, response, wait_for_response);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->send(command, response, wait_for_response);
    return false;
}

bool LockInAmplifier::sendCommand(const QString &command) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->sendCommand(command);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->sendCommand(command);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->sendCommand(command);
    return false;
}

bool LockInAmplifier::sendQuery(const QString &command, QString &response) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->sendQuery(command, response);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->sendQuery(command, response);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->sendQuery(command, response);
    return false;
}

bool LockInAmplifier::getIDN(QString &idn) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getIDN(idn);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getIDN(idn);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getIDN(idn);
    return false;
}

QStringList LockInAmplifier::getFrequencyPostfixList() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getFrequencyPostfixList();
    return QStringList();
}

QStringList LockInAmplifier::getVoltagePostfixList() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltagePostfixList();
    return QStringList();
}

bool LockInAmplifier::isValidInternalFrequency(const double &new_frequency, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->isValidInternalFrequency(new_frequency);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->isValidInternalFrequency(new_frequency);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidInternalFrequency(new_frequency, postfix);
    return false;
}

double LockInAmplifier::getMinInternalFrequency() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMinInternalFrequency();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getMinInternalFrequency();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMinInternalFrequency();
    return -1;
}

double LockInAmplifier::getMaxInternalFrequency() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMaxInternalFrequency();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getMaxInternalFrequency();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMaxInternalFrequency();
    return -1;
}

bool LockInAmplifier::setInternalFrequency(const double &new_frequency, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setInternalFrequency(new_frequency);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setInternalFrequency(new_frequency);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setInternalFrequency(new_frequency, postfix);
    return false;
}

double LockInAmplifier::getInternalFrequency() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getInternalFrequency();
    return -1;
}

double LockInAmplifier::getExternalFrequency() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getExternalFrequency();
    return -1;
}

double LockInAmplifier::getFrequency() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getFrequency();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getFrequency();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getFrequency();
    return -1;
}

double LockInAmplifier::getFrequencyDetect() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getFrequencyDetect();
    return -1;
}

int LockInAmplifier::getMinHarmonic() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMinHarmonic();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getMinHarmonic();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMinHarmonic();
    return -1;
}

int LockInAmplifier::getMaxHarmonic() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMaxHarmonic();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getMaxHarmonic();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMaxHarmonic();
    return -1;
}

bool LockInAmplifier::isValidHarmonic(const int &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->isValidHarmonic(i);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->isValidHarmonic(i);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidHarmonic(i);
    return false;
}

bool LockInAmplifier::setHarmonic(const int &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setHarmonic(i);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setHarmonic(i);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setHarmonic(i);
    return false;
}

int LockInAmplifier::getHarmonic() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getHarmonic();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getHarmonic();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getHarmonic();
    return -1;
}

int LockInAmplifier::getMinHarmonicDual() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMinHarmonicDual();
    return -1;
}

int LockInAmplifier::getMaxHarmonicDual() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMaxHarmonicDual();
    return -1;
}

bool LockInAmplifier::isValidHarmonicDual(const int &i) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidHarmonicDual(i);
    return false;
}

bool LockInAmplifier::setHarmonicDual(const int &i) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setHarmonicDual(i);
    return false;
}

int LockInAmplifier::getHarmonicDual() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getHarmonicDual();
    return -1;
}

bool LockInAmplifier::isValidSineOutAmplitude(const double &new_sineAmplitude, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->isValidSineOutAmplitude(new_sineAmplitude);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->isValidSineOutAmplitude(new_sineAmplitude);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidSineOutAmplitude(new_sineAmplitude, postfix);
    return false;
}

double LockInAmplifier::getMinSineOutAmplitude() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMinSineOutAmplitude();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getMinSineOutAmplitude();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMinSineOutAmplitude();
    return -1;
}

double LockInAmplifier::getMaxSineOutAmplitude() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMaxSineOutAmplitude();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getMaxSineOutAmplitude();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMaxSineOutAmplitude();
    return -1;
}

bool LockInAmplifier::setSineOutAmplitude(const double &new_sineAmplitude, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSineOutAmplitude(new_sineAmplitude);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setSineOutAmplitude(new_sineAmplitude);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSineOutAmplitude(new_sineAmplitude, postfix);
    return false;
}

double LockInAmplifier::getSineOutAmplitude() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSineOutAmplitude();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getSineOutAmplitude();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSineOutAmplitude();
    return -1;
}

bool LockInAmplifier::isValidSineOffset(const double &new_offset, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidSineOffset(new_offset, postfix);
    return false;
}

double LockInAmplifier::getMinSineOffset() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMinSineOffset();
    return -1;
}

double LockInAmplifier::getMaxSineOffset() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMaxSineOffset();
    return -1;
}

bool LockInAmplifier::setSineOffset(const double &new_offset, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSineOffset(new_offset, postfix);
    return false;
}

double LockInAmplifier::getSineOffset() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSineOffset();
    return -1;
}

QStringList LockInAmplifier::getReferenceSourceList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReferenceSourceList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getReferenceSourceList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceSourceList();
    return QStringList();
}

int LockInAmplifier::refSourceNumberFromString(const QString &ref_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->refSourceNumberFromString(ref_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->refSourceNumberFromString(ref_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refSourceNumberFromString(ref_string);
    return -1;
}

QString LockInAmplifier::refSourceStringFromNumber(const int &ref_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->refSourceStringFromNumber(ref_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->refSourceStringFromNumber(ref_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refSourceStringFromNumber(ref_number);
    return "";
}

bool LockInAmplifier::setReferenceSource(const int &new_source) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReferenceSource(new_source);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setReferenceSource(new_source);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceSource(new_source);
    return false;
}

bool LockInAmplifier::setReferenceSource(const QString &new_source) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReferenceSource(new_source);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setReferenceSource(new_source);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceSource(new_source);
    return false;
}

QString LockInAmplifier::getReferenceSource() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReferenceSource();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getReferenceSource();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceSource();
    return "";
}

QStringList LockInAmplifier::getReferenceTriggerModeList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReferenceTriggerModeList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getReferenceTriggerModeList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceTriggerModeList();
    return QStringList();
}

int LockInAmplifier::refModeNumberFromString(const QString &mode_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->refModeNumberFromString(mode_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->refModeNumberFromString(mode_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refModeNumberFromString(mode_string);
    return -1;
}

QString LockInAmplifier::refModeStringFromNumber(const int &mode_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->refModeStringFromNumber(mode_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->refModeStringFromNumber(mode_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refModeStringFromNumber(mode_number);
    return "";
}

bool LockInAmplifier::setReferenceTriggerMode(const int &new_mode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReferenceTriggerMode(new_mode);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setReferenceTriggerMode(new_mode);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceTriggerMode(new_mode);
    return false;
}

bool LockInAmplifier::setReferenceTriggerMode(const QString &new_mode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReferenceTriggerMode(new_mode);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setReferenceTriggerMode(new_mode);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceTriggerMode(new_mode);
    return false;
}

QString LockInAmplifier::getReferenceTriggerMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReferenceTriggerMode();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getReferenceTriggerMode();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceTriggerMode();
    return "";
}

QStringList LockInAmplifier::getReferenceTriggerOutputZList() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceTriggerOutputZList();
    return QStringList();
}

int LockInAmplifier::refOutputNumberFromString(const QString &outpZ_string) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refOutputNumberFromString(outpZ_string);
    return -1;
}

QString LockInAmplifier::refOutputStringFromNumber(const int &outpZ_number) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refOutputStringFromNumber(outpZ_number);
    return "";
}

bool LockInAmplifier::setReferenceTriggerOutputZ(const int &new_outpZ) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceTriggerOutputZ(new_outpZ);
    return false;
}

bool LockInAmplifier::setReferenceTriggerOutputZ(const QString &new_outpZ) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceTriggerOutputZ(new_outpZ);
    return false;
}

QString LockInAmplifier::getReferenceTriggerOutputZ() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceTriggerOutputZ();
    return "";
}

QStringList LockInAmplifier::getSignalInputList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSignalInputList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getSignalInputList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSignalInputList();
    return QStringList();
}

int LockInAmplifier::signalInputNumberFromString(const QString &signalInput_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->signalInputNumberFromString(signalInput_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->signalInputNumberFromString(signalInput_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->signalInputNumberFromString(signalInput_string);
    return -1;
}

QString LockInAmplifier::signalInputStringFromNumber(const int &signalInput_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->signalInputStringFromNumber(signalInput_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->signalInputStringFromNumber(signalInput_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->signalInputStringFromNumber(signalInput_number);
    return "";
}

bool LockInAmplifier::setSignalInput(const int &new_signalInput) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSignalInput(new_signalInput);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setSignalInput(new_signalInput);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSignalInput(new_signalInput);
    return false;
}

bool LockInAmplifier::setSignalInput(const QString &new_signalInput) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSignalInput(new_signalInput);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setSignalInput(new_signalInput);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSignalInput(new_signalInput);
    return false;
}

QString LockInAmplifier::getSignalInput() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSignalInput();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getSignalInput();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSignalInput();
    return "";
}

QStringList LockInAmplifier::getVoltageInputModeList() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputModeList();
    return QStringList();
}

int LockInAmplifier::voltageInputModeNumberFromString(const QString &voltageInputMode_string) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputModeNumberFromString(voltageInputMode_string);
    return -1;
}

QString LockInAmplifier::voltageInputModeStringFromNumber(const int &voltageInputMode_number) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputModeStringFromNumber(voltageInputMode_number);
    return "";
}

bool LockInAmplifier::setVoltageInputMode(const int &new_voltageInputMode) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputMode(new_voltageInputMode);
    return false;
}

bool LockInAmplifier::setVoltageInputMode(const QString &new_voltageInputMode) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputMode(new_voltageInputMode);
    return false;
}

QString LockInAmplifier::getVoltageInputMode() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputMode();
    return "";
}

QStringList LockInAmplifier::getVoltageInputCouplingList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getVoltageInputCouplingList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getVoltageInputCouplingList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputCouplingList();
    return QStringList();
}

int LockInAmplifier::voltageInputCouplingNumberFromString(const QString &voltageInputCoupling_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->voltageInputCouplingNumberFromString(voltageInputCoupling_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->voltageInputCouplingNumberFromString(voltageInputCoupling_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputCouplingNumberFromString(voltageInputCoupling_string);
    return -1;
}

QString LockInAmplifier::voltageInputCouplingStringFromNumber(const int &voltageInputCoupling_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->voltageInputCouplingStringFromNumber(voltageInputCoupling_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->voltageInputCouplingStringFromNumber(voltageInputCoupling_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputCouplingStringFromNumber(voltageInputCoupling_number);
    return "";
}

bool LockInAmplifier::setVoltageInputCoupling(const int &new_voltageInputCoupling) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setVoltageInputCoupling(new_voltageInputCoupling);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setVoltageInputCoupling(new_voltageInputCoupling);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputCoupling(new_voltageInputCoupling);
    return false;
}

bool LockInAmplifier::setVoltageInputCoupling(const QString &new_voltageInputCoupling) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setVoltageInputCoupling(new_voltageInputCoupling);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setVoltageInputCoupling(new_voltageInputCoupling);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputCoupling(new_voltageInputCoupling);
    return false;
}

QString LockInAmplifier::getVoltageInputCoupling() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getVoltageInputCoupling();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getVoltageInputCoupling();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputCoupling();
    return "";
}

QStringList LockInAmplifier::getVoltageInputShieldsList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getVoltageInputShieldsList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getVoltageInputShieldsList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputShieldsList();
    return QStringList();
}

int LockInAmplifier::voltageInputShieldsNumberFromString(const QString &voltageInputShields_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->voltageInputShieldsNumberFromString(voltageInputShields_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->voltageInputShieldsNumberFromString(voltageInputShields_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputShieldsNumberFromString(voltageInputShields_string);
    return -1;
}

QString LockInAmplifier::voltageInputShieldsStringFromNumber(const int &voltageInputShields_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->voltageInputShieldsStringFromNumber(voltageInputShields_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->voltageInputShieldsStringFromNumber(voltageInputShields_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputShieldsStringFromNumber(voltageInputShields_number);
    return "";
}

bool LockInAmplifier::setVoltageInputShields(const int &new_voltageInputShields) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setVoltageInputShields(new_voltageInputShields);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setVoltageInputShields(new_voltageInputShields);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputShields(new_voltageInputShields);
    return false;
}

bool LockInAmplifier::setVoltageInputShields(const QString &new_voltageInputShields) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setVoltageInputShields(new_voltageInputShields);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setVoltageInputShields(new_voltageInputShields);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputShields(new_voltageInputShields);
    return false;
}

QString LockInAmplifier::getVoltageInputShields() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getVoltageInputShields();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getVoltageInputShields();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputShields();
    return "";
}

QStringList LockInAmplifier::getVoltageInputRangeList() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputRangeList();
    return QStringList();
}

int LockInAmplifier::voltageInputRangeNumberFromString(const QString &voltageInputRange_string) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputRangeNumberFromString(voltageInputRange_string);
    return -1;
}

QString LockInAmplifier::voltageInputRangeStringFromNumber(const int &voltageInputRange_number) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputRangeStringFromNumber(voltageInputRange_number);
    return "";
}

bool LockInAmplifier::setVoltageInputRange(const int &new_voltageInputRange) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputRange(new_voltageInputRange);
    return false;
}

bool LockInAmplifier::setVoltageInputRange(const QString &new_voltageInputRange) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputRange(new_voltageInputRange);
    return false;
}

QString LockInAmplifier::getVoltageInputRange() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputRange();
    return "";
}

QStringList LockInAmplifier::getCurrentInputGainList() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getCurrentInputGainList();
    return QStringList();
}

int LockInAmplifier::currentInputGainNumberFromString(const QString &currentInputGain_string) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->currentInputGainNumberFromString(currentInputGain_string);
    return -1;
}

QString LockInAmplifier::currentInputGainStringFromNumber(const int &currentInputGain_number) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->currentInputGainStringFromNumber(currentInputGain_number);
    return "";
}

bool LockInAmplifier::setCurrentInputGain(const int &new_currentInputGain) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setCurrentInputGain(new_currentInputGain);
    return false;
}

bool LockInAmplifier::setCurrentInputGain(const QString &new_currentInputGain) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setCurrentInputGain(new_currentInputGain);
    return false;
}

QString LockInAmplifier::getCurrentInputGain() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getCurrentInputGain();
    return "";
}

int LockInAmplifier::getSignalStrength() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSignalStrength();
    return -1;
}

QStringList LockInAmplifier::getInputLineNotchFilterList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getInputLineNotchFilterList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getInputLineNotchFilterList();
    return QStringList();
}

int LockInAmplifier::inputLineNotchFilterNumberFromString(const QString &inputLineNotchFilter_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->inputLineNotchFilterNumberFromString(inputLineNotchFilter_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->inputLineNotchFilterNumberFromString(inputLineNotchFilter_string);
    return -1;
}

QString LockInAmplifier::inputLineNotchFilterStringFromNumber(const int &inputLineNotchFilter_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->inputLineNotchFilterStringFromNumber(inputLineNotchFilter_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->inputLineNotchFilterStringFromNumber(inputLineNotchFilter_number);
    return "";
}

bool LockInAmplifier::setInputLineNotchFilter(const int &new_inputLineNotchFilter) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setInputLineNotchFilter(new_inputLineNotchFilter);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setInputLineNotchFilter(new_inputLineNotchFilter);
    return false;
}

bool LockInAmplifier::setInputLineNotchFilter(const QString &new_inputLineNotchFilter) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setInputLineNotchFilter(new_inputLineNotchFilter);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setInputLineNotchFilter(new_inputLineNotchFilter);
    return false;
}

QString LockInAmplifier::getInputLineNotchFilter() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getInputLineNotchFilter();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getInputLineNotchFilter();
    return "";
}

QStringList LockInAmplifier::getSensivityList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSensivityList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getSensivityList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSensivityList();
    return QStringList();
}

int LockInAmplifier::sensivityNumberFromString(const QString &sensivity_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->sensivityNumberFromString(sensivity_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->sensivityNumberFromString(sensivity_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->sensivityNumberFromString(sensivity_string);
    return -1;
}

QString LockInAmplifier::sensivityStringFromNumber(const int &sensivity_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->sensivityStringFromNumber(sensivity_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->sensivityStringFromNumber(sensivity_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->sensivityStringFromNumber(sensivity_number);
    return "";
}

bool LockInAmplifier::setSensivity(const int &new_sensivity) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSensivity(new_sensivity);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setSensivity(new_sensivity);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSensivity(new_sensivity);
    return false;
}

bool LockInAmplifier::setSensivity(const QString &new_sensivity) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSensivity(new_sensivity);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setSensivity(new_sensivity);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSensivity(new_sensivity);
    return false;
}

QString LockInAmplifier::getSensivity() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSensivity();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getSensivity();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSensivity();
    return "";
}

QStringList LockInAmplifier::getReserveModeList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReserveModeList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getReserveModeList();
    return QStringList();
}

int LockInAmplifier::reserveModeNumberFromString(const QString &reserveMode_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->reserveModeNumberFromString(reserveMode_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->reserveModeNumberFromString(reserveMode_string);
    return -1;
}

QString LockInAmplifier::reserveModeStringFromNumber(const int &reserveMode_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->reserveModeStringFromNumber(reserveMode_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->reserveModeStringFromNumber(reserveMode_number);
    return "";
}

bool LockInAmplifier::setReserveMode(const int &new_reserveMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReserveMode(new_reserveMode);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setReserveMode(new_reserveMode);
    return false;
}

bool LockInAmplifier::setReserveMode(const QString &new_reserveMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReserveMode(new_reserveMode);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setReserveMode(new_reserveMode);
    return false;
}

QString LockInAmplifier::getReserveMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReserveMode();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getReserveMode();
    return "";
}

QStringList LockInAmplifier::getTimeConstantList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getTimeConstantList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getTimeConstantList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getTimeConstantList();
    return QStringList();
}

int LockInAmplifier::timeConstantNumberFromString(const QString &timeConstant_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->timeConstantNumberFromString(timeConstant_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->timeConstantNumberFromString(timeConstant_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->timeConstantNumberFromString(timeConstant_string);
    return -1;
}

QString LockInAmplifier::timeConstantStringFromNumber(const int &timeConstant_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->timeConstantStringFromNumber(timeConstant_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->timeConstantStringFromNumber(timeConstant_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->timeConstantStringFromNumber(timeConstant_number);
    return "";
}

bool LockInAmplifier::setTimeConstant(const int &new_timeConstant) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setTimeConstant(new_timeConstant);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setTimeConstant(new_timeConstant);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setTimeConstant(new_timeConstant);
    return false;
}

bool LockInAmplifier::setTimeConstant(const QString &new_timeConstant) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setTimeConstant(new_timeConstant);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setTimeConstant(new_timeConstant);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setTimeConstant(new_timeConstant);
    return false;
}

QString LockInAmplifier::getTimeConstant() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getTimeConstant();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getTimeConstant();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getTimeConstant();
    return "";
}

QStringList LockInAmplifier::getFilterList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getFilterList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getFilterList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getFilterList();
    return QStringList();
}

int LockInAmplifier::filterNumberFromString(const QString &filter_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->filterNumberFromString(filter_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->filterNumberFromString(filter_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->filterNumberFromString(filter_string);
    return -1;
}

QString LockInAmplifier::filterStringFromNumber(const int &filter_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->filterStringFromNumber(filter_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->filterStringFromNumber(filter_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->filterStringFromNumber(filter_number);
    return "";
}

bool LockInAmplifier::setFilter(const int &new_filter) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setFilter(new_filter);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setFilter(new_filter);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setFilter(new_filter);
    return false;
}

bool LockInAmplifier::setFilter(const QString &new_filter) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setFilter(new_filter);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setFilter(new_filter);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setFilter(new_filter);
    return false;
}

QString LockInAmplifier::getFilter() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getFilter();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getFilter();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getFilter();
    return "";
}

bool LockInAmplifier::enableSychronousFilter(const bool &enable) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->enableSychronousFilter(enable);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->enableSychronousFilter(enable);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->enableSychronousFilter(enable);
    return false;
}

bool LockInAmplifier::isSychronousFilterEnabled() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->isSychronousFilterEnabled();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->isSychronousFilterEnabled();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isSychronousFilterEnabled();
    return false;
}

bool LockInAmplifier::enableAdvancedFilterFilter(const bool &enable) const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->enableAdvancedFilterFilter(enable);
    return false;
}

bool LockInAmplifier::isAdvancedFilterEnabled() const
{
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isAdvancedFilterEnabled();
    return false;
}

bool LockInAmplifier::enableRS232outputInterface() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->enableRS232outputInterface();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->enableRS232outputInterface();
    return false;
}

QStringList LockInAmplifier::getSampleRateList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSampleRateList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getSampleRateList();
    return QStringList();
}

int LockInAmplifier::sampleRateNumberFromString(const QString &sampleRate_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->sampleRateNumberFromString(sampleRate_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->sampleRateNumberFromString(sampleRate_string);
    return -1;
}

QString LockInAmplifier::sampleRateStringFromNumber(const int &sampleRate_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->sampleRateStringFromNumber(sampleRate_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->sampleRateStringFromNumber(sampleRate_number);
    return "";
}

bool LockInAmplifier::setSampleRate(const int &new_sampleRate) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSampleRate(new_sampleRate);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setSampleRate(new_sampleRate);
    return false;
}

bool LockInAmplifier::setSampleRate(const QString &new_sampleRate) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSampleRate(new_sampleRate);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setSampleRate(new_sampleRate);
    return false;
}

QString LockInAmplifier::getSampleRate() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSampleRate();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getSampleRate();
    return "";
}

QStringList LockInAmplifier::getBufferModeList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getBufferModeList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getBufferModeList();
    return QStringList();
}

int LockInAmplifier::bufferModeNumberFromString(const QString &bufferMode_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->bufferModeNumberFromString(bufferMode_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->bufferModeNumberFromString(bufferMode_string);
    return -1;
}

QString LockInAmplifier::bufferModeStringFromNumber(const int &bufferMode_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->bufferModeStringFromNumber(bufferMode_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->bufferModeStringFromNumber(bufferMode_number);
    return "";
}

bool LockInAmplifier::setBufferMode(const int &new_bufferMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setBufferMode(new_bufferMode);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setBufferMode(new_bufferMode);
    return false;
}

bool LockInAmplifier::setBufferMode(const QString &new_bufferMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setBufferMode(new_bufferMode);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setBufferMode(new_bufferMode);
    return false;
}

QString LockInAmplifier::getBufferMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getBufferMode();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getBufferMode();
    return "";
}

bool LockInAmplifier::setDisplayData(const int &channel, const int &data) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setDisplayData(channel, data);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setDisplayData(channel, data);
    return false;
}

int LockInAmplifier::getBufferSize() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getBufferSize();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getBufferSize();
    return -1;
}

double LockInAmplifier::getPointFromBuffer(const int &channel, const int &number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setDisplayData(channel, number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->setDisplayData(channel, number);
    return -1;
}

std::vector < double > LockInAmplifier::getChannelFromBuffer(const int &channel) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getChannelFromBuffer(channel);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getChannelFromBuffer(channel);
    std::vector < double > res;
    return res;
}

int LockInAmplifier::getBuffer(std::vector < double > &ch1, std::vector < double > &ch2) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getBuffer(ch1, ch2);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getBuffer(ch1, ch2);
    return -1;
}

bool LockInAmplifier::startBuffer() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->startBuffer();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->startBuffer();
    return false;
}

bool LockInAmplifier::pauseBuffer() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->pauseBuffer();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->pauseBuffer();
    return false;
}

bool LockInAmplifier::stopBuffer() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->stopBuffer();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->stopBuffer();
    return false;
}

QStringList LockInAmplifier::getOutputNumberList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getOutputNumberList();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getOutputNumberList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getOutputNumberList();
    return QStringList();
}

int LockInAmplifier::outpNumberFromString(const QString &outp_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->outpNumberFromString(outp_string);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->outpNumberFromString(outp_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->outpNumberFromString(outp_string);
    return -1;
}

QString LockInAmplifier::outpStringFromNumber(const int &outp_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->outpStringFromNumber(outp_number);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->outpStringFromNumber(outp_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->outpStringFromNumber(outp_number);
    return "";
}

double LockInAmplifier::getOUTP(const int &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getOUTP(i);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getOUTP(i);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getOUTP(i);
    return 0;
}

double LockInAmplifier::getOUTP(const QString &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getOUTP(i);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getOUTP(i);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getOUTP(i);
    return 0;
}

double LockInAmplifier::getX() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getX();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getX();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getX();
    return 0;
}

double LockInAmplifier::getY() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getY();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getY();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getY();
    return 0;
}

double LockInAmplifier::getR() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getR();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getR();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getR();
    return 0;
}

double LockInAmplifier::getTheta() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getTheta();
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getTheta();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getTheta();
    return 0;
}

bool LockInAmplifier::getAB(const int &Apos, double &A, const int &Bpos, double &B) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getAB(Apos, A, Bpos, B);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getAB(Apos, A, Bpos, B);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getAB(Apos, A, Bpos, B);
    return false;
}

bool LockInAmplifier::getABC(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getABC(Apos, A, Bpos, B, Cpos, C);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getABC(Apos, A, Bpos, B, Cpos, C);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getABC(Apos, A, Bpos, B, Cpos, C);
    return false;
}

bool LockInAmplifier::getABCDE(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C, const int &Dpos, double &D, const int &Epos, double &E) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getABCDE(Apos, A, Bpos, B, Cpos, C, Dpos, D, Epos, E);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getABCDE(Apos, A, Bpos, B, Cpos, C, Dpos, D, Epos, E);
    return false;
}

bool LockInAmplifier::getABCDEF(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C, const int &Dpos, double &D, const int &Epos, double &E, const int &Fpos, double &F) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getABCDEF(Apos, A, Bpos, B, Cpos, C, Dpos, D, Epos, E, Fpos, F);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getABCDEF(Apos, A, Bpos, B, Cpos, C, Dpos, D, Epos, E, Fpos, F);
    return false;
}

bool LockInAmplifier::getXY(double &X, double &Y) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getXY(X, Y);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getXY(X, Y);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getXY(X, Y);
    return false;
}

bool LockInAmplifier::getRTheta(double &R, double &Theta) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getRTheta(R, Theta);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getRTheta(R, Theta);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getRTheta(R, Theta);
    return false;
}

bool LockInAmplifier::getRThetaFint(double &R, double &Theta, double &F) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getRThetaF(R, Theta, F);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getRThetaF(R, Theta, F);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getRThetaFint(R, Theta, F);
    return false;
}

bool LockInAmplifier::getRThetaFext(double &R, double &Theta, double &F) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getRThetaF(R, Theta, F);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getRThetaF(R, Theta, F);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getRThetaFext(R, Theta, F);
    return false;
}

bool LockInAmplifier::getRThetaF(double &R, double &Theta, double &F) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getRThetaF(R, Theta, F);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getRThetaF(R, Theta, F);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getRThetaF(R, Theta, F);
    return false;
}

bool LockInAmplifier::getXYRThetaF(double &X, double&Y, double &R, double &Theta, double &F) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getXYRThetaF(X, Y, R, Theta, F);
//    if (this->lockInAmplifierType == "SR844")
//        return this->sr844->getXYRThetaF(X, Y, R, Theta, F);
    return false;
}

void LockInAmplifier::catchErrorSignal(const QString &s) const
{
    emit this->errorSignal(s);
}

void LockInAmplifier::catchResponseSignal(const QString &s) const
{
    emit this->responseSignal(s);
}

void LockInAmplifier::catchCommandSignal(const QString &s) const
{
    emit this->commandSignal(s);
}

void LockInAmplifier::catchTimeoutSignal(const QString &s) const
{
    emit this->timeoutSignal(s);
}
