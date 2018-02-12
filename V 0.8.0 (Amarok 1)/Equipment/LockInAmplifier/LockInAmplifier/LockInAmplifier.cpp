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
    if (this->srs != nullptr)
        this->srs->~SRS();
    this->sr830 = nullptr;
    this->sr844 = nullptr;
    this->sr865 = nullptr;
    this->srs = nullptr;
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
        connect(this->srs,   SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString)),    Qt::DirectConnection);
        connect(this->srs,   SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->srs,   SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)),  Qt::DirectConnection);
        connect(this->srs,   SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)),  Qt::DirectConnection);
    }
    if (this->sr830 != nullptr) {
        connect(this->sr830, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString)),    Qt::DirectConnection);
        connect(this->sr830, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->sr830, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)),  Qt::DirectConnection);
        connect(this->sr830, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)),  Qt::DirectConnection);
    }
    if (this->sr844 != nullptr) {
        connect(this->sr844, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString)),    Qt::DirectConnection);
        connect(this->sr844, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->sr844, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)),  Qt::DirectConnection);
        connect(this->sr844, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)),  Qt::DirectConnection);
    }
    if (this->sr865 != nullptr) {
        connect(this->sr865, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString)),    Qt::DirectConnection);
        connect(this->sr865, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->sr865, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)),  Qt::DirectConnection);
        connect(this->sr865, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)),  Qt::DirectConnection);
    }

    return;
}

LockInAmplifier_State LockInAmplifier::getState()
{
    LockInAmplifier_State current_state;

    current_state.model = this->lockInAmplifierType;

    if (workWithInternalFrequency())
        current_state.internalFrequency = getInternalFrequency();
    if (workWithHarmonic())
        current_state.harmonic = getHarmonic();
    if (workWithHarmonicDual())
        current_state.harmonicDual = getHarmonicDual();
    if (workWithSineOutAmplitude())
        current_state.sineOutAmplitude = getSineOutAmplitude();
    if (workWithSineOffset())
        current_state.sineOffset = getSineOffset();
    if (workWithReferenceSource())
        current_state.referenceSource = getReferenceSource();
    if (workWithReferenceTriggerMode())
        current_state.referenceTriggerMode = getReferenceTriggerMode();
    if (workWithReferenceTriggerOutputZ())
        current_state.referenceTriggerOutputZ = getReferenceTriggerOutputZ();
    if (workWithSignalInput())
        current_state.signalInput = getSignalInput();
    if (workWithSignalInputZ())
        current_state.signalInputZ = getSignalInputZ();
    if (workWithVoltageInputMode())
        current_state.voltageInputMode = getVoltageInputMode();
    if (workWithVoltageInputCoupling())
        current_state.voltageInputCoupling = getVoltageInputCoupling();
    if (workWithVoltageInputShields())
        current_state.voltageInputShields = getVoltageInputShields();
    if (workWithVoltageInputRange())
        current_state.voltageInputRange = getVoltageInputRange();
    if (workWithCurrentInputGain())
        current_state.currentInputGain = getCurrentInputGain();
    if (workWithInputLineNotchFilter())
        current_state.inputLineNotchFilter = getInputLineNotchFilter();
    if (workWithSensivity())
        current_state.sensivity = getSensivity();
    if (workWithReserveMode())
        current_state.reserveMode = getReserveMode();
    if (workWithWideReserveMode())
        current_state.wideReserveMode = getWideReserveMode();
    if (workWithCloseReserveMode())
        current_state.closeReserveMode = getCloseReserveMode();
    if (workWithTimeConstant())
        current_state.timeConstant = getTimeConstant();
    if (workWithFilter())
        current_state.filter = getFilter();
    if (workWithSynchronousFilter())
        current_state.synchronousFilter = getSynchronousFilterEnabled();
    if (workWithAdvancedFilter())
        current_state.advancedFilter = getAdvancedFilterEnabled();
    if (workWithSampleRate())
        current_state.sampleRate = getSampleRate();
    if (workWithBufferMode())
        current_state.bufferMode = getBufferMode();

    return current_state;
}

bool LockInAmplifier::setState(const LockInAmplifier_State &new_state)
{
    bool result = true;

    if (new_state.model != this->lockInAmplifierType)
        result = false;
    if (workWithInternalFrequency())
        result &= this->setInternalFrequency(new_state.internalFrequency);
    if (workWithHarmonic())
        result &= this->setHarmonic(new_state.harmonic);
    if (workWithHarmonicDual())
        result &= this->setHarmonicDual(new_state.harmonicDual);
    if (workWithSineOutAmplitude())
        result &= this->setSineOutAmplitude(new_state.sineOutAmplitude);
    if (workWithSineOffset())
        result &= this->setSineOffset(new_state.sineOffset);
    if (workWithReferenceSource())
        result &= this->setReferenceSource(new_state.referenceSource);
    if (workWithReferenceTriggerMode())
        result &= this->setReferenceTriggerMode(new_state.referenceTriggerMode);
    if (workWithReferenceTriggerOutputZ())
        result &= this->setReferenceTriggerOutputZ(new_state.referenceTriggerOutputZ);
    if (workWithSignalInput())
        result &= this->setSignalInput(new_state.signalInput);
    if (workWithSignalInputZ())
        result &= this->setSignalInputZ(new_state.signalInputZ);
    if (workWithVoltageInputMode())
        result &= this->setVoltageInputMode(new_state.voltageInputMode);
    if (workWithVoltageInputCoupling())
        result &= this->setVoltageInputCoupling(new_state.voltageInputCoupling);
    if (workWithVoltageInputShields())
        result &= this->setVoltageInputShields(new_state.voltageInputShields);
    if (workWithVoltageInputRange())
        result &= this->setVoltageInputRange(new_state.voltageInputRange);
    if (workWithCurrentInputGain())
        result &= this->setCurrentInputGain(new_state.currentInputGain);
    if (workWithInputLineNotchFilter())
        result &= this->setInputLineNotchFilter(new_state.inputLineNotchFilter);
    if (workWithSensivity())
        result &= this->setSensivity(new_state.sensivity);
    if (workWithReserveMode())
        result &= this->setReserveMode(new_state.reserveMode);
    if (workWithWideReserveMode())
        result &= this->setWideReserveMode(new_state.wideReserveMode);
    if (workWithCloseReserveMode())
        result &= this->setCloseReserveMode(new_state.closeReserveMode);
    if (workWithTimeConstant())
        result &= this->setTimeConstant(new_state.timeConstant);
    if (workWithFilter())
        result &= this->setFilter(new_state.filter);
    if (workWithSynchronousFilter())
        result &= this->enableSynchronousFilter(new_state.synchronousFilter);
    if (workWithAdvancedFilter())
        result &= this->enableAdvancedFilter(new_state.advancedFilter);
    if (workWithSampleRate())
        result &= this->setSampleRate(new_state.sampleRate);
    if (workWithBufferMode())
        result &= this->setBufferMode(new_state.bufferMode);

    return result;
}

void LockInAmplifier::wait(const double &x) const
{
    QTest::qWait(_round(this->waitTime*x));

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

    return;
}

QString LockInAmplifier::getLockInAmplifierType() const
{
    return this->lockInAmplifierType;
}

bool LockInAmplifier::autoSetLockInAmplifierType(QSerialPort *serial)
{
    this->lockInAmplifierType = "";

    QString type = this->srs->autoDetect(serial);

    if (type.trimmed().isEmpty()) {
        this->srs->disconnect(serial);

        return false;
    }

    this->lockInAmplifierType = type;
    if (type == "SR830"){
        this->sr830 = new SR830();
        this->sr830->setSerial(serial);
        init();

        return true;
    }
    if (type == "SR844"){
        this->sr844 = new SR844();
        this->sr844->setSerial(serial);
        init();

        return true;
    }
    if (type == "SR865"){
        this->sr865 = new SR865();
        this->sr865->setSerial(serial);
        init();

        return true;
    }

    this->srs->disconnect(serial);

    return false;
}

bool LockInAmplifier::autoSetLockInAmplifierType(const QString &portName, const int &new_baudrate)
{
    QSerialPort *serial = new QSerialPort();

    serial->setPortName(portName);
    serial->setBaudRate(new_baudrate);

    serial->setParity(QSerialPort::NoParity); // TOCHANGE
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);

    return autoSetLockInAmplifierType(serial);
}

bool LockInAmplifier::autoSetLockInAmplifierType(const QString &portName)
{
//    this->lockInAmplifierType = "";

//    QString type = this->srs->autoDetect(portName);

//    if (type.trimmed().isEmpty()) {
//        this->srs->disconnect(serial);

//        return false;
//    }

//    this->lockInAmplifierType = type;
//    if (type == "SR830"){
//        this->sr830 = new SR830();
//        this->sr830->setConnection(portName, 19200);
//        init();

//        return true;
//    }
//    if (type == "SR844"){
//        this->sr844 = new SR844();
//        this->sr844->setConnection(portName, 19200);
//        init();

//        return true;
//    }
//    if (type == "SR865"){
//        this->sr865 = new SR865();
//        this->sr865->setConnection(portName, 19200);
//        init();

//        return true;
//    }

//    this->srs->disconnect(serial);

//    return false;

    QStringList baudrates = this->srs->getBaudrateList();
    for (int baud = 0; baud < baudrates.size(); baud++) {
        int baudrate = baudrates[baud].toInt();

        if (autoSetLockInAmplifierType(portName, baudrate))
            return true;
    }

    return false;
}

bool LockInAmplifier::setConnection(const QString &portName, const int &new_baudrate, const QString &new_lockInAmplifierType)
{
    if (new_lockInAmplifierType.trimmed().toUpper() == "SR830") {
        if (this->sr830 == nullptr)
            this->sr830 = new SR830();
        this->lockInAmplifierType = "SR830";
        init();

        return this->sr830->setConnection(portName, new_baudrate);
    }
    if (new_lockInAmplifierType.trimmed().toUpper() == "SR844") {
        if (this->sr844 == nullptr)
            this->sr844 = new SR844();
        this->lockInAmplifierType = "SR844";
        init();

        return this->sr844->setConnection(portName, new_baudrate);
    }
    if (new_lockInAmplifierType.trimmed().toUpper() == "SR865") {
        if (this->sr865 == nullptr)
            this->sr865 = new SR865();
        this->lockInAmplifierType = "SR865";
        init();

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
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getBaudrateList();
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
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->isValidBaudrate(new_baudrate);
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

bool LockInAmplifier::test() const
{
    bool ans = true;

    qDebug() << "Testling Lock-in Amplifier";

    setDefaultSettings();

    wait();

    if (workWithInternalFrequency()) {
        qDebug() << "Lock-in Amplifier internal frequency command test";

        if (!setReferenceSource("INT")) {
            qDebug() << "Lock-in Amplifier internal frequency settings command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (!setInternalFrequency(getMinInternalFrequency())) {
            qDebug() << "Lock-in Amplifier internal frequency command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (getInternalFrequency() == getMinInternalFrequency())
            qDebug() << "Min internal frequency passed";
        else {
            qDebug() << "Min internal frequency failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (!setInternalFrequency(getMaxInternalFrequency())) {
            qDebug() << "Lock-in Amplifier internal frequency command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (getInternalFrequency() == getMaxInternalFrequency())
            qDebug() << "Max internal frequency passed";
        else {
            qDebug() << "Max internal frequency failed";

            ans = false;
        }

        setDefaultInternalFrequency();
        wait();
    }

    if (workWithHarmonic()) {
        qDebug() << "Lock-in Amplifier harmonic command test";

        if (!setReferenceSource("INT")) {
            qDebug() << "Lock-in Amplifier internal reference command failed";

            ans = false;
        }

        wait();

        if (!setInternalFrequency(52000)) {
            qDebug() << "Lock-in Amplifier internal frequency command failed";

            ans = false;
        }

        wait();

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (!setHarmonic(getMinHarmonic())) {
            qDebug() << "Lock-in Amplifier harmonic command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (getHarmonic() == getMinHarmonic())
            qDebug() << "Min harmonic passed";
        else {
            qDebug() << "Min harmonic failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (!setHarmonic(getMaxHarmonic())) {
            qDebug() << "Lock-in Amplifier harmonic command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (getHarmonic() == getMaxHarmonic())
            qDebug() << "Max harmonic passed";
        else {
            qDebug() << "Max harmonic failed";

            ans = false;
        }

        setDefaultHarmonic();
        wait();

        setDefaultInternalFrequency();
        wait();
    }

    if (workWithHarmonicDual()) {
        qDebug() << "Lock-in Amplifier harmonic dual command test";

        if (!setHarmonicDual(getMinHarmonicDual())) {
            qDebug() << "Lock-in Amplifier harmonic dual command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (getHarmonicDual() == getMinHarmonicDual())
            qDebug() << "Min harmonic dual passed";
        else {
            qDebug() << "Min harmonic dual failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (!setHarmonicDual(getMaxHarmonicDual())) {
            qDebug() << "Lock-in Amplifier harmonic dual command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (getHarmonicDual() == getMaxHarmonicDual())
            qDebug() << "Max harmonic dual passed";
        else {
            qDebug() << "Max harmonic dual failed";

            ans = false;
        }

        setDefaultHarmonicDual();
        wait();
    }

    if (workWithSineOutAmplitude()) {
        qDebug() << "Lock-in Amplifier sine amplitude command test";

        if (this->lockInAmplifierType == "SR865") {
            if (!setSineOutAmplitude(1, "NV")) {
                qDebug() << "Lock-in Amplifier sine amplitude command failed";

                ans = false;
            }
        } else {
            if (!setSineOutAmplitude(getMinSineOutAmplitude())) {
                qDebug() << "Lock-in Amplifier sine amplitude command failed";

                ans = false;
            }
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (_abs(getSineOutAmplitude() - getMinSineOutAmplitude()) < this->Err)
            qDebug() << "Min sine amplitude passed";
        else {
            qDebug() << "Min sine amplitude failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (!setSineOutAmplitude(getMaxSineOutAmplitude())) {
            qDebug() << "Lock-in Amplifier sine amplitude command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (_abs(getSineOutAmplitude() - getMaxSineOutAmplitude()) < this->Err)
            qDebug() << "Max sine amplitude passed";
        else {
            qDebug() << "Max sine amplitude failed";

            ans = false;
        }

        setDefaultSineAmplitude();
        wait();
    }

    if (workWithSineOffset()) {
        qDebug() << "Lock-in Amplifier sine offset command test";

        if (!setSineOffset(getMinSineOffset())) {
            qDebug() << "Lock-in Amplifier sine offset command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (_abs(getSineOffset() - getMinSineOffset()) < this->Err)
            qDebug() << "Min sine offset passed";
        else {
            qDebug() << "Min sine offset failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (!setSineOffset(getMaxSineOffset())) {
            qDebug() << "Lock-in Amplifier sine offset command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (_abs(getSineOffset() - getMaxSineOffset()) < this->Err)
            qDebug() << "Max sine amplitude passed";
        else {
            qDebug() << "Max sine amplitude failed";

            ans = false;
        }

        setDefaultSineOffset();
        wait();
    }

    if (workWithReferenceSource()) {
        qDebug() << "Lock-in Amplifier reference source command test";

        foreach (const QString &referenceSource, getReferenceSourceList()) {
            if (!setReferenceSource(referenceSource)) {
                qDebug() << "Lock-in Amplifier reference source command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getReferenceSource() == referenceSource)
                qDebug() << "Reference source passed";
            else {
                qDebug() << "Reference source failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultReferenceSource();
        wait();
    }

    if (workWithReferenceTriggerMode()) {
        qDebug() << "Lock-in Amplifier reference trigger mode command test";

        foreach (const QString &referenceTriggerMode, getReferenceTriggerModeList()) {
            if (!setReferenceTriggerMode(referenceTriggerMode)) {
                qDebug() << "Lock-in Amplifier reference trigger mode command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getReferenceTriggerMode() == referenceTriggerMode)
                qDebug() << "Reference trigger mode passed";
            else {
                qDebug() << "Reference trigger mode failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultReferenceTriggerMode();
        wait();
    }

    if (workWithReferenceTriggerOutputZ()) {
        qDebug() << "Lock-in Amplifier reference trigger output Z command test";

        foreach (const QString &referenceTriggerOutputZ, getReferenceTriggerOutputZList()) {
            if (!setReferenceTriggerOutputZ(referenceTriggerOutputZ)) {
                qDebug() << "Lock-in Amplifier reference trigger output Z command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getReferenceTriggerOutputZ() == referenceTriggerOutputZ)
                qDebug() << "Reference trigger output Z passed";
            else {
                qDebug() << "Reference trigger output Z failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultReferenceTriggerOutputZ();
        wait();
    }

    if (workWithSignalInput()) {
        qDebug() << "Lock-in Amplifier signal input command test";

        if (!setSensivity("10 mV/nA")) {
            qDebug() << "Lock-in Amplifier sensivity command failed";

            ans = false;
        }

        foreach (const QString &signalInput, getSignalInputList()) {
            if (!setSignalInput(signalInput)) {
                qDebug() << "Lock-in Amplifier signal input command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getSignalInput() == signalInput)
                qDebug() << "Signal input passed";
            else {
                qDebug() << "Signal input failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultSignalInput();
        wait();

        setDefaultSensivity();
        wait();
    }

    if (workWithSignalInputZ()) {
        qDebug() << "Lock-in Amplifier signal input Z command test";

        foreach (const QString &signalInputZ, getSignalInputZList()) {
            if (!setSignalInputZ(signalInputZ)) {
                qDebug() << "Lock-in Amplifier signal input Z command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getSignalInputZ() == signalInputZ)
                qDebug() << "Signal input Z passed";
            else {
                qDebug() << "Signal input Z failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultSignalInputZ();
        wait();
    }

    if (workWithVoltageInputMode()) {
        qDebug() << "Lock-in Amplifier voltage input mode command test";

        foreach (const QString &voltageInputMode, getVoltageInputModeList()) {
            if (!setVoltageInputMode(voltageInputMode)) {
                qDebug() << "Lock-in Amplifier voltage input mode command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getVoltageInputMode() == voltageInputMode)
                qDebug() << "Voltage input mode passed";
            else {
                qDebug() << "Voltage input mode failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultVoltageInputMode();
        wait();
    }

    if (workWithVoltageInputCoupling()) {
        qDebug() << "Lock-in Amplifier voltage input coupling command test";

        foreach (const QString &voltageInputCoupling, getVoltageInputCouplingList()) {
            if (!setVoltageInputCoupling(voltageInputCoupling)) {
                qDebug() << "Lock-in Amplifier voltage input coupling command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getVoltageInputCoupling() == voltageInputCoupling)
                qDebug() << "Voltage input coupling passed";
            else {
                qDebug() << "Voltage input coupling failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultVoltageInputCoupling();
        wait();
    }

    if (workWithVoltageInputShields()) {
        qDebug() << "Lock-in Amplifier voltage input shields command test";

        foreach (const QString &voltageInputShield, getVoltageInputShieldsList()) {
            if (!setVoltageInputShields(voltageInputShield)) {
                qDebug() << "Lock-in Amplifier voltage input shields command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getVoltageInputShields() == voltageInputShield)
                qDebug() << "Voltage input shields passed";
            else {
                qDebug() << "Voltage input shields failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultVoltageInputShields();
        wait();
    }

    if (workWithVoltageInputRange()) {
        qDebug() << "Lock-in Amplifier voltage input range command test";

        foreach (const QString &voltageInputRange, getVoltageInputRangeList()) {
            if (!setVoltageInputRange(voltageInputRange)) {
                qDebug() << "Lock-in Amplifier voltage input range command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getVoltageInputRange() == voltageInputRange)
                qDebug() << "Voltage input range passed";
            else {
                qDebug() << "Voltage input range failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultVoltageInputRange();
        wait();
    }

    if (workWithCurrentInputGain()) {
        qDebug() << "Lock-in Amplifier current input gain command test";

        foreach (const QString &currentInputGain, getCurrentInputGainList()) {
            if (!setCurrentInputGain(currentInputGain)) {
                qDebug() << "Lock-in Amplifier current input gain command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getCurrentInputGain() == currentInputGain)
                qDebug() << "Current input gain passed";
            else {
                qDebug() << "Current input gain failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultCurrentInputGain();
        wait();
    }

    if (workWithInputLineNotchFilter()) {
        qDebug() << "Lock-in Amplifier line noch filter command test";

        foreach (const QString &lineNochFilter, getInputLineNotchFilterList()) {
            if (!setInputLineNotchFilter(lineNochFilter)) {
                qDebug() << "Lock-in Amplifier line noch filter command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getInputLineNotchFilter() == lineNochFilter)
                qDebug() << "Line noch filter passed";
            else {
                qDebug() << "Line noch filter failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultInputLineNotchFilter();
        wait();
    }

    if (workWithSensivity()) {
        qDebug() << "Lock-in Amplifier sensivity command test";

        foreach (const QString &sensivity, getSensivityList()) {
            if (!setSensivity(sensivity)) {
                qDebug() << "Lock-in Amplifier sensivity command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getSensivity() == sensivity)
                qDebug() << "Sensivity passed";
            else {
                qDebug() << "Sensivity failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultSensivity();
        wait();
    }

    if (workWithReserveMode()) {
        qDebug() << "Lock-in Amplifier reserve mode command test";

        foreach (const QString &reserveMode, getReserveModeList()) {
            if (!setReserveMode(reserveMode)) {
                qDebug() << "Lock-in Amplifier reserve mode command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getReserveMode() == reserveMode)
                qDebug() << "Reserve mode passed";
            else {
                qDebug() << "Reserve mode failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultReserveMode();
        wait();
    }

    if (workWithWideReserveMode()) {
        qDebug() << "Lock-in Amplifier wide reserve mode command test";

        foreach (const QString &wideReserveMode, getWideReserveModeList()) {
            if (!setWideReserveMode(wideReserveMode)) {
                qDebug() << "Lock-in Amplifier wide reserve mode command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getWideReserveMode() == wideReserveMode)
                qDebug() << "Wide reserve mode passed";
            else {
                qDebug() << "Wide reserve mode failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultWideReserveMode();
        wait();
    }

    if (workWithWideReserveMode()) {
        qDebug() << "Lock-in Amplifier close reserve mode command test";

        foreach (const QString &closeReserveMode, getCloseReserveModeList()) {
            if (!setCloseReserveMode(closeReserveMode)) {
                qDebug() << "Lock-in Amplifier close reserve mode command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getCloseReserveMode() == closeReserveMode)
                qDebug() << "Close reserve mode passed";
            else {
                qDebug() << "Close reserve mode failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultCloseReserveMode();
        wait();
    }

    if (workWithTimeConstant()) {
        qDebug() << "Lock-in Amplifier time constant command test";

        if (!setReferenceSource("INT")) {
            qDebug() << "Lock-in Amplifier reference command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        if (!setInternalFrequency(getMinInternalFrequency())) {
            qDebug() << "Lock-in Amplifier min internal frequency command failed";

            ans = false;
        }

        if (this->lockInAmplifierType != "SR865")
            wait(this->longWait);

        foreach (const QString &timeConstant, getTimeConstantList()) {
            if (!setTimeConstant(timeConstant)) {
                qDebug() << "Lock-in Amplifier time constant command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getTimeConstant() == timeConstant)
                qDebug() << "Time constant passed";
            else {
                qDebug() << "Time constant failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultTimeConstant();
        wait();

        setDefaultInternalFrequency();
        wait();

        setDefaultReferenceSource();
        wait();
    }

    if (workWithFilter()) {
        qDebug() << "Lock-in Amplifier filter command test";

        foreach (const QString &filter, getFilterList()) {
            if (!setFilter(filter)) {
                qDebug() << "Lock-in Amplifier filter command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getFilter() == filter)
                qDebug() << "Filter passed";
            else {
                qDebug() << "Filter failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultFilter();
        wait();
    }

    if (workWithSynchronousFilter()) {
        qDebug() << "Lock-in Amplifier synchronous filter command test";

        for (int i = 1; i >= 0; i--) {
            if (i == 0)
                qDebug() << "Lock-in Amplifier synchronous filter disbling";
            else
                qDebug() << "Lock-in Amplifier synchronous filter enabling";

            if (!enableSynchronousFilter(i)) {
                qDebug() << "Lock-in Amplifier synchronous filter comand failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getSynchronousFilterEnabled() == i)
                qDebug() << "Lock-in Amplifier synchronous filter settings accepted";
            else {
                qDebug() << "Lock-in Amplifier synchronous filter settings not accepted";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultSynchronousFilter();
        wait();
    }

    if (workWithAdvancedFilter()) {
        qDebug() << "Lock-in Amplifier advanced filter command test";

        for (int i = 1; i >= 0; i--) {
            if (i == 0)
                qDebug() << "Lock-in Amplifier advanced filter disbling";
            else
                qDebug() << "Lock-in Amplifier advanced filter enabling";

            if (!enableAdvancedFilter(i)) {
                qDebug() << "Lock-in Amplifier advanced filter comand failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getAdvancedFilterEnabled() == i)
                qDebug() << "Lock-in Amplifier advanced filter settings accepted";
            else {
                qDebug() << "Lock-in Amplifier advanced filter settings not accepted";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultAdvancedFilter();
        wait();
    }

    if (workWithSampleRate()) {
        qDebug() << "Lock-in Amplifier sample rate command test";

        foreach (const QString &sampleRate, getSampleRateList()) {
            if (!setSampleRate(sampleRate)) {
                qDebug() << "Lock-in Amplifier sample rate command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getSampleRate() == sampleRate)
                qDebug() << "Sample rate passed";
            else {
                qDebug() << "Sample rate failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultSampleRate();
        wait();
    }

    if (workWithBufferMode()) {
        qDebug() << "Lock-in Amplifier buffer mode command test";

        foreach (const QString &bufferMode, getBufferModeList()) {
            if (!setBufferMode(bufferMode)) {
                qDebug() << "Lock-in Amplifier buffer mode command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);

            if (getBufferMode() == bufferMode)
                qDebug() << "Buffer mode passed";
            else {
                qDebug() << "Buffer mode failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        setDefaultBufferMode();
        wait();
    }

    if (true) {
        qDebug() << "Lock-in Amplifier output command test";

        foreach (const QString &output, getOutputNumberList()) {
            if (getOUTP(output) && !getOUTP(output)) {
                qDebug() << "Lock-in Amplifier output command failed";

                ans = false;
            }

            if (this->lockInAmplifierType != "SR865")
                wait(this->longWait);
        }

        wait();
    }

    return ans;
}

bool LockInAmplifier::setDefaultSettings() const
{
    bool ans = true;

    if (workWithInternalFrequency())
        ans &= setDefaultInternalFrequency();
    if (workWithHarmonic())
        ans &= setDefaultHarmonic();
    if (workWithHarmonicDual())
        ans &= setDefaultHarmonicDual();
    if (workWithSineOutAmplitude())
        ans &= setDefaultSineAmplitude();
    if (workWithSineOffset())
        ans &= setDefaultSineOffset();
    if (workWithReferenceSource())
        ans &= setDefaultReferenceSource();
    if (workWithReferenceTriggerMode())
        ans &= setDefaultReferenceTriggerMode();
    if (workWithReferenceTriggerOutputZ())
        ans &= setDefaultReferenceTriggerOutputZ();
    if (workWithSignalInput())
        ans &= setDefaultSignalInput();
    if (workWithSignalInputZ())
        ans &= setDefaultSignalInputZ();
    if (workWithVoltageInputMode())
        ans &= setDefaultVoltageInputMode();
    if (workWithVoltageInputCoupling())
        ans &= setDefaultVoltageInputCoupling();
    if (workWithVoltageInputShields())
        ans &= setDefaultVoltageInputShields();
    if (workWithVoltageInputRange())
        ans &= setDefaultVoltageInputRange();
    if (workWithCurrentInputGain())
        ans &= setDefaultCurrentInputGain();
    if (workWithInputLineNotchFilter())
        ans &= setDefaultInputLineNotchFilter();
    if (workWithSensivity())
        ans &= setDefaultSensivity();
    if (workWithReserveMode())
        ans &= setDefaultReserveMode();
    if (workWithWideReserveMode())
        ans &= setDefaultWideReserveMode();
    if (workWithCloseReserveMode())
        ans &= setDefaultCloseReserveMode();
    if (workWithTimeConstant())
        ans &= setDefaultTimeConstant();
    if (workWithFilter())
        ans &= setDefaultFilter();
    if (workWithSynchronousFilter())
        ans &= setDefaultSynchronousFilter();
    if (workWithAdvancedFilter())
        ans &= setDefaultAdvancedFilter();
    if (workWithEnablingRS232outputInterface())
        ans &= setDefaultRS232outputInterface();
    if (workWithSampleRate())
        ans &= setDefaultSampleRate();
    if (workWithBufferMode())
        ans &= setDefaultBufferMode();

    return ans;
}

double LockInAmplifier::getAverageInputTime() const
{
    if (this->lockInAmplifierType == "SR830")
        return 1;
    if (this->lockInAmplifierType == "SR844")
        return 1;
    if (this->lockInAmplifierType == "SR865")
        return 1;
    return 1E5;
}

double LockInAmplifier::getAverageOutputTime() const
{
    if (this->lockInAmplifierType == "SR830")
        return 50;
    if (this->lockInAmplifierType == "SR844")
        return 50;
    if (this->lockInAmplifierType == "SR865")
        return 150;
    return 1E5;
}

bool LockInAmplifier::workWithFrequencyPostfix() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultFrequencyPostfix() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "HZ";
    return "";
}

QStringList LockInAmplifier::getFrequencyPostfixList() const
{
    if (this->lockInAmplifierType == "SR830")
        return QStringList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getFrequencyPostfixList();
    return QStringList();
}

bool LockInAmplifier::workWithVoltagePostfix() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultVoltagePostfix() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "V";
    return "";
}

QStringList LockInAmplifier::getVoltagePostfixList() const
{
    if (this->lockInAmplifierType == "SR830")
        return QStringList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltagePostfixList();
    return QStringList();
}

bool LockInAmplifier::workWithInternalFrequency() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

double LockInAmplifier::getDefaultInternalFrequency() const
{
    if (this->lockInAmplifierType == "SR830")
        return 100000;
    if (this->lockInAmplifierType == "SR844")
        return 100000;
    if (this->lockInAmplifierType == "SR865")
        return 100000;
    return 0;
}

bool LockInAmplifier::setDefaultInternalFrequency() const
{
    return setInternalFrequency(getDefaultInternalFrequency(), getDefaultFrequencyPostfix());
}

bool LockInAmplifier::isValidInternalFrequency(const double &new_frequency, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->isValidInternalFrequency(new_frequency);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->isValidInternalFrequency(new_frequency);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidInternalFrequency(new_frequency, postfix);
    return false;
}

double LockInAmplifier::getMinInternalFrequency() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMinInternalFrequency();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getMinInternalFrequency();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMinInternalFrequency();
    return -1;
}

double LockInAmplifier::getMaxInternalFrequency() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMaxInternalFrequency();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getMaxInternalFrequency();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMaxInternalFrequency();
    return -1;
}

bool LockInAmplifier::setInternalFrequency(const double &new_frequency, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setInternalFrequency(new_frequency);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setInternalFrequency(new_frequency);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setInternalFrequency(new_frequency, postfix);
    return false;
}

double LockInAmplifier::getInternalFrequency() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getFrequency();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getFrequency();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getInternalFrequency();
    return -1;
}

double LockInAmplifier::getExternalFrequency() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getFrequency();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getFrequency();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getExternalFrequency();
    return -1;
}

double LockInAmplifier::getFrequency() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getFrequency();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getFrequency();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getFrequency();
    return -1;
}

double LockInAmplifier::getFrequencyDetect() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getFrequency();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getFrequencyDetect();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getFrequencyDetect();
    return -1;
}

bool LockInAmplifier::workWithHarmonic() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

int LockInAmplifier::getDefaultHarmonic() const
{
    if (this->lockInAmplifierType == "SR830")
        return 1;
    if (this->lockInAmplifierType == "SR844")
        return 1;
    if (this->lockInAmplifierType == "SR865")
        return 1;
    return -1;
}

bool LockInAmplifier::setDefaultHarmonic() const
{
    return setHarmonic(getDefaultHarmonic());
}

int LockInAmplifier::getMinHarmonic() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMinHarmonic();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getMinHarmonic();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMinHarmonic();
    return -1;
}

int LockInAmplifier::getMaxHarmonic() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMaxHarmonic();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getMaxHarmonic();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMaxHarmonic();
    return -1;
}

bool LockInAmplifier::isValidHarmonic(const int &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->isValidHarmonic(i);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->isValidHarmonic(i);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidHarmonic(i);
    return false;
}

bool LockInAmplifier::setHarmonic(const int &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setHarmonic(i);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setHarmonic(i);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setHarmonic(i);
    return false;
}

int LockInAmplifier::getHarmonic() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getHarmonic();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getHarmonic();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getHarmonic();
    return -1;
}

bool LockInAmplifier::workWithHarmonicDual() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

int LockInAmplifier::getDefaultHarmonicDual() const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return 1;
    return -1;
}

bool LockInAmplifier::setDefaultHarmonicDual() const
{
    return setHarmonicDual(getDefaultHarmonicDual());
}

int LockInAmplifier::getMinHarmonicDual() const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMinHarmonicDual();
    return -1;
}

int LockInAmplifier::getMaxHarmonicDual() const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMaxHarmonicDual();
    return -1;
}

bool LockInAmplifier::isValidHarmonicDual(const int &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidHarmonicDual(i);
    return false;
}

bool LockInAmplifier::setHarmonicDual(const int &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setHarmonicDual(i);
    return false;
}

int LockInAmplifier::getHarmonicDual() const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getHarmonicDual();
    return -1;
}

bool LockInAmplifier::workWithSineOutAmplitude() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

double LockInAmplifier::getDefaultSineAmplitude() const
{
    if (this->lockInAmplifierType == "SR830")
        return 0.1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return 0.1;
    return -1;
}

bool LockInAmplifier::setDefaultSineAmplitude() const
{
    return setSineOutAmplitude(getDefaultSineAmplitude());
}

bool LockInAmplifier::isValidSineOutAmplitude(const double &new_sineAmplitude, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->isValidSineOutAmplitude(new_sineAmplitude);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidSineOutAmplitude(new_sineAmplitude, postfix);
    return false;
}

double LockInAmplifier::getMinSineOutAmplitude() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMinSineOutAmplitude();
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMinSineOutAmplitude();
    return -1;
}

double LockInAmplifier::getMaxSineOutAmplitude() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getMaxSineOutAmplitude();
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMaxSineOutAmplitude();
    return -1;
}

bool LockInAmplifier::setSineOutAmplitude(const double &new_sineAmplitude, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSineOutAmplitude(new_sineAmplitude);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSineOutAmplitude(new_sineAmplitude, postfix);
    return false;
}

double LockInAmplifier::getSineOutAmplitude() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSineOutAmplitude();
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSineOutAmplitude();
    return -1;
}

bool LockInAmplifier::workWithSineOffset() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

double LockInAmplifier::getDefaultSineOffset() const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return 0;
    return -1;
}

bool LockInAmplifier::setDefaultSineOffset() const
{
    return setSineOffset(getDefaultSineOffset());
}

bool LockInAmplifier::isValidSineOffset(const double &new_offset, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->isValidSineOffset(new_offset, postfix);
    return false;
}

double LockInAmplifier::getMinSineOffset() const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMinSineOffset();
    return -1;
}

double LockInAmplifier::getMaxSineOffset() const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getMaxSineOffset();
    return -1;
}

bool LockInAmplifier::setSineOffset(const double &new_offset, const QString &postfix) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSineOffset(new_offset, postfix);
    return false;
}

double LockInAmplifier::getSineOffset() const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSineOffset();
    return -1;
}

bool LockInAmplifier::workWithReferenceSource() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultReferenceSource() const
{
    if (this->lockInAmplifierType == "SR830")
        return "EXT";
    if (this->lockInAmplifierType == "SR844")
        return "EXT";
    if (this->lockInAmplifierType == "SR865")
        return "EXT";
    return "";
}

bool LockInAmplifier::setDefaultReferenceSource() const
{
    return setReferenceSource(getDefaultReferenceSource());
}

QStringList LockInAmplifier::getReferenceSourceList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReferenceSourceList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getReferenceSourceList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceSourceList();
    return QStringList();
}

int LockInAmplifier::refSourceNumberFromString(const QString &ref_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->refSourceNumberFromString(ref_string);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->refSourceNumberFromString(ref_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refSourceNumberFromString(ref_string);
    return -1;
}

QString LockInAmplifier::refSourceStringFromNumber(const int &ref_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->refSourceStringFromNumber(ref_number);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->refSourceStringFromNumber(ref_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refSourceStringFromNumber(ref_number);
    return "";
}

bool LockInAmplifier::setReferenceSource(const int &new_source) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReferenceSource(new_source);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setReferenceSource(new_source);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceSource(new_source);
    return false;
}

bool LockInAmplifier::setReferenceSource(const QString &new_source) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReferenceSource(new_source);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setReferenceSource(new_source);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceSource(new_source);
    return false;
}

QString LockInAmplifier::getReferenceSource() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReferenceSource();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getReferenceSource();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceSource();
    return "";
}

bool LockInAmplifier::workWithReferenceTriggerMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultReferenceTriggerMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return "TTL Rising";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "POSTTL";
    return "";
}

bool LockInAmplifier::setDefaultReferenceTriggerMode() const
{
    return setReferenceTriggerMode(getDefaultReferenceTriggerMode());
}

QStringList LockInAmplifier::getReferenceTriggerModeList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReferenceTriggerModeList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceTriggerModeList();
    return QStringList();
}

int LockInAmplifier::refModeNumberFromString(const QString &mode_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->refModeNumberFromString(mode_string);
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refModeNumberFromString(mode_string);
    return -1;
}

QString LockInAmplifier::refModeStringFromNumber(const int &mode_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->refModeStringFromNumber(mode_number);
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refModeStringFromNumber(mode_number);
    return "";
}

bool LockInAmplifier::setReferenceTriggerMode(const int &new_mode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReferenceTriggerMode(new_mode);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceTriggerMode(new_mode);
    return false;
}

bool LockInAmplifier::setReferenceTriggerMode(const QString &new_mode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReferenceTriggerMode(new_mode);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceTriggerMode(new_mode);
    return false;
}

QString LockInAmplifier::getReferenceTriggerMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReferenceTriggerMode();
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceTriggerMode();
    return "";
}

bool LockInAmplifier::workWithReferenceTriggerOutputZ() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultReferenceTriggerOutputZ() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "50Ohms";
    if (this->lockInAmplifierType == "SR865")
        return "50OHMS";
    return "";
}

bool LockInAmplifier::setDefaultReferenceTriggerOutputZ() const
{
    return setReferenceTriggerOutputZ(getDefaultReferenceTriggerOutputZ());
}

QStringList LockInAmplifier::getReferenceTriggerOutputZList() const
{
    if (this->lockInAmplifierType == "SR830")
        return QStringList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getReferenceTriggerOutputZList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceTriggerOutputZList();
    return QStringList();
}

int LockInAmplifier::refOutputNumberFromString(const QString &outpZ_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->refTriggerOutputZNumberFromString(outpZ_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refOutputZNumberFromString(outpZ_string);
    return -1;
}

QString LockInAmplifier::refOutputStringFromNumber(const int &outpZ_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->refTriggerOutputZStringFromNumber(outpZ_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->refOutputZStringFromNumber(outpZ_number);
    return "";
}

bool LockInAmplifier::setReferenceTriggerOutputZ(const int &new_outpZ) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setReferenceTriggerOutputZ(new_outpZ);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceTriggerOutputZ(new_outpZ);
    return false;
}

bool LockInAmplifier::setReferenceTriggerOutputZ(const QString &new_outpZ) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setReferenceTriggerOutputZ(new_outpZ);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setReferenceTriggerOutputZ(new_outpZ);
    return false;
}

QString LockInAmplifier::getReferenceTriggerOutputZ() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getReferenceTriggerOutputZ();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getReferenceTriggerOutputZ();
    return "";
}

bool LockInAmplifier::workWithSignalInput() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultSignalInput() const
{
    if (this->lockInAmplifierType == "SR830")
        return "Vol A";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "VOLTAGE";
    return "";
}

bool LockInAmplifier::setDefaultSignalInput() const
{
    return setSignalInput(getDefaultSignalInput());
}

QStringList LockInAmplifier::getSignalInputList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSignalInputList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSignalInputList();
    return QStringList();
}

int LockInAmplifier::signalInputNumberFromString(const QString &signalInput_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->signalInputNumberFromString(signalInput_string);
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->signalInputNumberFromString(signalInput_string);
    return -1;
}

QString LockInAmplifier::signalInputStringFromNumber(const int &signalInput_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->signalInputStringFromNumber(signalInput_number);
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->signalInputStringFromNumber(signalInput_number);
    return "";
}

bool LockInAmplifier::setSignalInput(const int &new_signalInput) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSignalInput(new_signalInput);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSignalInput(new_signalInput);
    return false;
}

bool LockInAmplifier::setSignalInput(const QString &new_signalInput) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSignalInput(new_signalInput);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSignalInput(new_signalInput);
    return false;
}

QString LockInAmplifier::getSignalInput() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSignalInput();
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSignalInput();
    return "";
}

bool LockInAmplifier::workWithSignalInputZ() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

QString LockInAmplifier::getDefaultSignalInputZ() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "1MOhms";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setDefaultSignalInputZ() const
{
    return setSignalInputZ(getDefaultSignalInputZ());
}

QStringList LockInAmplifier::getSignalInputZList() const
{
    if (this->lockInAmplifierType == "SR830")
        return QStringList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getSignalInputZList();
    if (this->lockInAmplifierType == "SR865")
        return QStringList();
    return QStringList();
}

int LockInAmplifier::signalInputZNumberFromString(const QString &signalInputZ_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->signalInputZNumberFromString(signalInputZ_string);
    if (this->lockInAmplifierType == "SR865")
        return -1;
    return -1;
}

QString LockInAmplifier::signalInputZStringFromNumber(const int &signalInputZ_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->signalInputZStringFromNumber(signalInputZ_number);
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setSignalInputZ(const int &new_signalInputZ) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setSignalInputZ(new_signalInputZ);
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::setSignalInputZ(const QString &new_signalInputZ) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setSignalInputZ(new_signalInputZ);
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

QString LockInAmplifier::getSignalInputZ() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getSignalInputZ();
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::workWithVoltageInputMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultVoltageInputMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "A";
    return "";
}

bool LockInAmplifier::setDefaultVoltageInputMode() const
{
    return setVoltageInputMode(getDefaultVoltageInputMode());
}

QStringList LockInAmplifier::getVoltageInputModeList() const
{
    if (this->lockInAmplifierType == "SR830")
        return QStringList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputModeList();
    return QStringList();
}

int LockInAmplifier::voltageInputModeNumberFromString(const QString &voltageInputMode_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputModeNumberFromString(voltageInputMode_string);
    return -1;
}

QString LockInAmplifier::voltageInputModeStringFromNumber(const int &voltageInputMode_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputModeStringFromNumber(voltageInputMode_number);
    return "";
}

bool LockInAmplifier::setVoltageInputMode(const int &new_voltageInputMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputMode(new_voltageInputMode);
    return false;
}

bool LockInAmplifier::setVoltageInputMode(const QString &new_voltageInputMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputMode(new_voltageInputMode);
    return false;
}

QString LockInAmplifier::getVoltageInputMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputMode();
    return "";
}

bool LockInAmplifier::workWithVoltageInputCoupling() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultVoltageInputCoupling() const
{
    if (this->lockInAmplifierType == "SR830")
        return "AC";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "AC";
    return "";
}

bool LockInAmplifier::setDefaultVoltageInputCoupling() const
{
    return setVoltageInputCoupling(getDefaultVoltageInputCoupling());
}

QStringList LockInAmplifier::getVoltageInputCouplingList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getVoltageInputCouplingList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputCouplingList();
    return QStringList();
}

int LockInAmplifier::voltageInputCouplingNumberFromString(const QString &voltageInputCoupling_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->voltageInputCouplingNumberFromString(voltageInputCoupling_string);
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputCouplingNumberFromString(voltageInputCoupling_string);
    return -1;
}

QString LockInAmplifier::voltageInputCouplingStringFromNumber(const int &voltageInputCoupling_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->voltageInputCouplingStringFromNumber(voltageInputCoupling_number);
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputCouplingStringFromNumber(voltageInputCoupling_number);
    return "";
}

bool LockInAmplifier::setVoltageInputCoupling(const int &new_voltageInputCoupling) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setVoltageInputCoupling(new_voltageInputCoupling);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputCoupling(new_voltageInputCoupling);
    return false;
}

bool LockInAmplifier::setVoltageInputCoupling(const QString &new_voltageInputCoupling) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setVoltageInputCoupling(new_voltageInputCoupling);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputCoupling(new_voltageInputCoupling);
    return false;
}

QString LockInAmplifier::getVoltageInputCoupling() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getVoltageInputCoupling();
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputCoupling();
    return "";
}

bool LockInAmplifier::workWithVoltageInputShields() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultVoltageInputShields() const
{
    if (this->lockInAmplifierType == "SR830")
        return "GROUND";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "GROUND";
    return "";
}

bool LockInAmplifier::setDefaultVoltageInputShields() const
{
    return setVoltageInputShields(getDefaultVoltageInputShields());
}

QStringList LockInAmplifier::getVoltageInputShieldsList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getVoltageInputShieldsList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputShieldsList();
    return QStringList();
}

int LockInAmplifier::voltageInputShieldsNumberFromString(const QString &voltageInputShields_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->voltageInputShieldsNumberFromString(voltageInputShields_string);
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputShieldsNumberFromString(voltageInputShields_string);
    return -1;
}

QString LockInAmplifier::voltageInputShieldsStringFromNumber(const int &voltageInputShields_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->voltageInputShieldsStringFromNumber(voltageInputShields_number);
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputShieldsStringFromNumber(voltageInputShields_number);
    return "";
}

bool LockInAmplifier::setVoltageInputShields(const int &new_voltageInputShields) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setVoltageInputShields(new_voltageInputShields);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputShields(new_voltageInputShields);
    return false;
}

bool LockInAmplifier::setVoltageInputShields(const QString &new_voltageInputShields) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setVoltageInputShields(new_voltageInputShields);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputShields(new_voltageInputShields);
    return false;
}

QString LockInAmplifier::getVoltageInputShields() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getVoltageInputShields();
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputShields();
    return "";
}

bool LockInAmplifier::workWithVoltageInputRange() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultVoltageInputRange() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "1V";
    return "";
}

bool LockInAmplifier::setDefaultVoltageInputRange() const
{
    return setVoltageInputRange(getDefaultVoltageInputRange());
}

QStringList LockInAmplifier::getVoltageInputRangeList() const
{
    if (this->lockInAmplifierType == "SR830")
        return QStringList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputRangeList();
    return QStringList();
}

int LockInAmplifier::voltageInputRangeNumberFromString(const QString &voltageInputRange_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputRangeNumberFromString(voltageInputRange_string);
    return -1;
}

QString LockInAmplifier::voltageInputRangeStringFromNumber(const int &voltageInputRange_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->voltageInputRangeStringFromNumber(voltageInputRange_number);
    return "";
}

bool LockInAmplifier::setVoltageInputRange(const int &new_voltageInputRange) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputRange(new_voltageInputRange);
    return false;
}

bool LockInAmplifier::setVoltageInputRange(const QString &new_voltageInputRange) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setVoltageInputRange(new_voltageInputRange);
    return false;
}

QString LockInAmplifier::getVoltageInputRange() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getVoltageInputRange();
    return "";
}

QString LockInAmplifier::getAutoVoltageInputRange(const double &r) const
{
    if (this->lockInAmplifierType != "SR865")
        return "";

    double temp_r = r*this->inputRangeReserve;
    if (temp_r < 0.01)
            return "10M";
    if (temp_r < 0.03)
            return "30M";
    if (temp_r < 0.1)
            return "10M";
    if (temp_r < 0.3)
            return "30M";
    return "1V";
}

bool LockInAmplifier::setAutoVoltageInputRange(const double &r) const
{
    return setVoltageInputRange(getAutoVoltageInputRange(r));
}

bool LockInAmplifier::workWithCurrentInputGain() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultCurrentInputGain() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "1uA";
    return "";
}

bool LockInAmplifier::setDefaultCurrentInputGain() const
{
    return setCurrentInputGain(getDefaultCurrentInputGain());
}

QStringList LockInAmplifier::getCurrentInputGainList() const
{
    if (this->lockInAmplifierType == "SR830")
        return QStringList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getCurrentInputGainList();
    return QStringList();
}

int LockInAmplifier::currentInputGainNumberFromString(const QString &currentInputGain_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->currentInputGainNumberFromString(currentInputGain_string);
    return -1;
}

QString LockInAmplifier::currentInputGainStringFromNumber(const int &currentInputGain_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->currentInputGainStringFromNumber(currentInputGain_number);
    return "";
}

bool LockInAmplifier::setCurrentInputGain(const int &new_currentInputGain) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setCurrentInputGain(new_currentInputGain);
    return false;
}

bool LockInAmplifier::setCurrentInputGain(const QString &new_currentInputGain) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setCurrentInputGain(new_currentInputGain);
    return false;
}

QString LockInAmplifier::getCurrentInputGain() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getCurrentInputGain();
    return "";
}

bool LockInAmplifier::workWithSignalStrength() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

int LockInAmplifier::getSignalStrength() const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSignalStrength();
    return -1;
}

bool LockInAmplifier::workWithInputLineNotchFilter() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

QString LockInAmplifier::getDefaultInputLineNotchFilter() const
{
    if (this->lockInAmplifierType == "SR830")
        return "No filter";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setDefaultInputLineNotchFilter() const
{
    return setInputLineNotchFilter(getDefaultInputLineNotchFilter());
}

QStringList LockInAmplifier::getInputLineNotchFilterList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getInputLineNotchFilterList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return QStringList();
    return QStringList();
}

int LockInAmplifier::inputLineNotchFilterNumberFromString(const QString &inputLineNotchFilter_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->inputLineNotchFilterNumberFromString(inputLineNotchFilter_string);
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return -1;
    return -1;
}

QString LockInAmplifier::inputLineNotchFilterStringFromNumber(const int &inputLineNotchFilter_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->inputLineNotchFilterStringFromNumber(inputLineNotchFilter_number);
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setInputLineNotchFilter(const int &new_inputLineNotchFilter) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setInputLineNotchFilter(new_inputLineNotchFilter);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::setInputLineNotchFilter(const QString &new_inputLineNotchFilter) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setInputLineNotchFilter(new_inputLineNotchFilter);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

QString LockInAmplifier::getInputLineNotchFilter() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getInputLineNotchFilter();
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::workWithSensivity() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultSensivity() const
{
    if (this->lockInAmplifierType == "SR830")
        return "1 V/uA";
    if (this->lockInAmplifierType == "SR844")
        return "1 V/uA";
    if (this->lockInAmplifierType == "SR865")
        return "1 V/uA";
    return "";
}

bool LockInAmplifier::setDefaultSensivity() const
{
    return setSensivity(getDefaultSensivity());
}

QStringList LockInAmplifier::getSensivityList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSensivityList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getSensivityList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSensivityList();
    return QStringList();
}

int LockInAmplifier::sensivityNumberFromString(const QString &sensivity_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->sensivityNumberFromString(sensivity_string);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->sensivityNumberFromString(sensivity_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->sensivityNumberFromString(sensivity_string);
    return -1;
}

QString LockInAmplifier::sensivityStringFromNumber(const int &sensivity_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->sensivityStringFromNumber(sensivity_number);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->sensivityStringFromNumber(sensivity_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->sensivityStringFromNumber(sensivity_number);
    return "";
}

bool LockInAmplifier::setSensivity(const int &new_sensivity) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSensivity(new_sensivity);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setSensivity(new_sensivity);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSensivity(new_sensivity);
    return false;
}

bool LockInAmplifier::setSensivity(const QString &new_sensivity) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSensivity(new_sensivity);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setSensivity(new_sensivity);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setSensivity(new_sensivity);
    return false;
}

QString LockInAmplifier::getSensivity() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSensivity();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getSensivity();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSensivity();
    return "";
}

QString LockInAmplifier::getAutoSensivity(const double &r) const
{
    double temp_r = r*this->sensivityReserve;

    if (temp_r < 1E-9)
        if (this->lockInAmplifierType == "SR865")
            return "1 nV/fA";
    if (temp_r < 2E-9)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return   "2 nV/fA";
    if (temp_r < 5E-9)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return   "5 nV/fA";
    if (temp_r < 1E-8)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return  "10 nV/fA";
    if (temp_r < 2E-8)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return  "20 nV/fA";
    if (temp_r < 5E-8)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return  "50 nV/fA";
    if (temp_r < 1E-7)
        return "100 nV/fA";
    if (temp_r < 2E-7)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return "200 nV/fA";
    if (temp_r < 3E-7)
        if (this->lockInAmplifierType == "SR844")
            return "300 nV/fA";
    if (temp_r < 5E-7)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return "500 nV/fA";
    if (temp_r < 1E-6)
        return   "1 uV/pA";
    if (temp_r < 2E-6)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return   "2 uV/pA";
    if (temp_r < 3E-6)
        if (this->lockInAmplifierType == "SR844")
            return "3 uV/pA";
    if (temp_r < 5E-6)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return   "5 uV/pA";
    if (temp_r < 1E-5)
        return  "10 uV/pA";
    if (temp_r < 2E-5)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return  "20 uV/pA";
    if (temp_r < 3E-5)
        if (this->lockInAmplifierType == "SR844")
            return "30 uV/pA";
    if (temp_r < 5E-5)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return  "50 uV/pA";
    if (temp_r < 1E-4)
        return "100 uV/pA";
    if (temp_r < 2E-4)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return "200 uV/pA";
    if (temp_r < 3E-4)
        if (this->lockInAmplifierType == "SR844")
            return "300 uV/pA";
    if (temp_r < 5E-4)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return "500 uV/pA";
    if (temp_r < 1E-3)
        return   "1 mV/nA";
    if (temp_r < 2E-3)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return   "2 mV/nA";
    if (temp_r < 3E-3)
        if (this->lockInAmplifierType == "SR844")
            return "3 mV/nA";
    if (temp_r < 5E-3)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return   "5 mV/nA";
    if (temp_r < 1E-2)
        return  "10 mV/nA";
    if (temp_r < 2E-2)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return  "20 mV/nA";
    if (temp_r < 3E-2)
        if (this->lockInAmplifierType == "SR844")
            return "30 mV/nA";
    if (temp_r < 5E-2)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return  "50 mV/nA";
    if (temp_r < 1E-1)
        return "100 mV/nA";
    if (temp_r < 2E-1)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return "200 mV/nA";
    if (temp_r < 3E-1)
        if (this->lockInAmplifierType == "SR844")
            return "300 mV/nA";
    if (temp_r < 5E-1)
        if (this->lockInAmplifierType == "SR865" ||
            this->lockInAmplifierType == "SR830")
            return "500 mV/nA";
    return "1 V/uA";
}

bool LockInAmplifier::setAutoSensivity(const double &r) const
{
    return setSensivity(getAutoSensivity(r));
}

bool LockInAmplifier::workWithReserveMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

QString LockInAmplifier::getDefaultReserveMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return "Normal";
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setDefaultReserveMode() const
{
    return setReserveMode(getDefaultReserveMode());
}

QStringList LockInAmplifier::getReserveModeList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReserveModeList();
    if (this->lockInAmplifierType == "SR844")
        return QStringList();
    if (this->lockInAmplifierType == "SR865")
        return QStringList();
    return QStringList();
}

int LockInAmplifier::reserveModeNumberFromString(const QString &reserveMode_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->reserveModeNumberFromString(reserveMode_string);
    if (this->lockInAmplifierType == "SR844")
        return -1;
    if (this->lockInAmplifierType == "SR865")
        return -1;
    return -1;
}

QString LockInAmplifier::reserveModeStringFromNumber(const int &reserveMode_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->reserveModeStringFromNumber(reserveMode_number);
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setReserveMode(const int &new_reserveMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReserveMode(new_reserveMode);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::setReserveMode(const QString &new_reserveMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setReserveMode(new_reserveMode);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

QString LockInAmplifier::getReserveMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getReserveMode();
    if (this->lockInAmplifierType == "SR844")
        return "";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::workWithWideReserveMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

QString LockInAmplifier::getDefaultWideReserveMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "Normal";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setDefaultWideReserveMode() const
{
    return setWideReserveMode(getDefaultWideReserveMode());
}

QStringList LockInAmplifier::getWideReserveModeList() const
{
    if (this->lockInAmplifierType == "SR830")
        return QStringList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getWideReserveModeList();
    if (this->lockInAmplifierType == "SR865")
        return QStringList();
    return QStringList();
}

int LockInAmplifier::wideReserveModeNumberFromString(const QString &wideReserveMode_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->wideReserveModeNumberFromString(wideReserveMode_string);
    if (this->lockInAmplifierType == "SR865")
        return -1;
    return -1;
}

QString LockInAmplifier::wideReserveModeStringFromNumber(const int &wideReserveMode_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->wideReserveModeStringFromNumber(wideReserveMode_number);
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setWideReserveMode(const int &new_wideReserveMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setWideReserveMode(new_wideReserveMode);
    if (this->lockInAmplifierType == "Sr865")
        return false;
    return false;
}

bool LockInAmplifier::setWideReserveMode(const QString &new_wideReserveMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setWideReserveMode(new_wideReserveMode);
    if (this->lockInAmplifierType == "Sr865")
        return false;
    return false;
}

QString LockInAmplifier::getWideReserveMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getWideReserveMode();
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::workWithCloseReserveMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

QString LockInAmplifier::getDefaultCloseReserveMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return "Normal";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setDefaultCloseReserveMode() const
{
    return setCloseReserveMode(getDefaultCloseReserveMode());
}

QStringList LockInAmplifier::getCloseReserveModeList() const
{
    if (this->lockInAmplifierType == "SR830")
        return QStringList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getCloseReserveModeList();
    if (this->lockInAmplifierType == "SR865")
        return QStringList();
    return QStringList();
}

int LockInAmplifier::closeReserveModeNumberFromString(const QString &closeReserveMode_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return -1;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->closeReserveModeNumberFromString(closeReserveMode_string);
    if (this->lockInAmplifierType == "SR865")
        return -1;
    return -1;

}

QString LockInAmplifier::closeReserveModeStringFromNumber(const int &closeReserveMode_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->closeReserveModeStringFromNumber(closeReserveMode_number);
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setCloseReserveMode(const int &new_closeReserveMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setCloseReserveMode(new_closeReserveMode);
    if (this->lockInAmplifierType == "Sr865")
        return false;
    return false;
}

bool LockInAmplifier::setCloseReserveMode(const QString &new_closeReserveMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setCloseReserveMode(new_closeReserveMode);
    if (this->lockInAmplifierType == "Sr865")
        return false;
    return false;
}

QString LockInAmplifier::getCloseReserveMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return "";
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getCloseReserveMode();
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::workWithTimeConstant() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultTimeConstant() const
{
    if (this->lockInAmplifierType == "SR830")
        return "30 ms";
    if (this->lockInAmplifierType == "SR844")
        return "30 ms";
    if (this->lockInAmplifierType == "SR865")
        return "30 ms";
    return "";
}

bool LockInAmplifier::setDefaultTimeConstant() const
{
    return setTimeConstant(getDefaultTimeConstant());
}

QStringList LockInAmplifier::getTimeConstantList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getTimeConstantList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getTimeConstantList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getTimeConstantList();
    return QStringList();
}

int LockInAmplifier::timeConstantNumberFromString(const QString &timeConstant_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->timeConstantNumberFromString(timeConstant_string);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->timeConstantNumberFromString(timeConstant_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->timeConstantNumberFromString(timeConstant_string);
    return -1;
}

QString LockInAmplifier::timeConstantStringFromNumber(const int &timeConstant_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->timeConstantStringFromNumber(timeConstant_number);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->timeConstantStringFromNumber(timeConstant_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->timeConstantStringFromNumber(timeConstant_number);
    return "";
}

bool LockInAmplifier::setTimeConstant(const int &new_timeConstant) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setTimeConstant(new_timeConstant);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setTimeConstant(new_timeConstant);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setTimeConstant(new_timeConstant);
    return false;
}

bool LockInAmplifier::setTimeConstant(const QString &new_timeConstant) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setTimeConstant(new_timeConstant);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setTimeConstant(new_timeConstant);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setTimeConstant(new_timeConstant);
    return false;
}

QString LockInAmplifier::getTimeConstant() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getTimeConstant();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getTimeConstant();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getTimeConstant();
    return "";
}

bool LockInAmplifier::workWithFilter() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

QString LockInAmplifier::getDefaultFilter() const
{
    if (this->lockInAmplifierType == "SR830")
        return "24 dB/oct";
    if (this->lockInAmplifierType == "SR844")
        return "24 dB/oct";
    if (this->lockInAmplifierType == "SR865")
        return "24 dB/oct";
    return "";
}

bool LockInAmplifier::setDefaultFilter() const
{
    return setFilter(getDefaultFilter());
}

QStringList LockInAmplifier::getFilterList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getFilterList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getFilterList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getFilterList();
    return QStringList();
}

int LockInAmplifier::filterNumberFromString(const QString &filter_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->filterNumberFromString(filter_string);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->filterNumberFromString(filter_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->filterNumberFromString(filter_string);
    return -1;
}

QString LockInAmplifier::filterStringFromNumber(const int &filter_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->filterStringFromNumber(filter_number);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->filterStringFromNumber(filter_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->filterStringFromNumber(filter_number);
    return "";
}

bool LockInAmplifier::setFilter(const int &new_filter) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setFilter(new_filter);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setFilter(new_filter);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setFilter(new_filter);
    return false;
}

bool LockInAmplifier::setFilter(const QString &new_filter) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setFilter(new_filter);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setFilter(new_filter);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->setFilter(new_filter);
    return false;
}

QString LockInAmplifier::getFilter() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getFilter();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getFilter();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getFilter();
    return "";
}

bool LockInAmplifier::workWithSynchronousFilter() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

bool LockInAmplifier::setDefaultSynchronousFilter() const
{
    return enableSynchronousFilter(false);
}

bool LockInAmplifier::enableSynchronousFilter(const bool &enable) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->enableSynchronousFilter(enable);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->enableSynchronousFilter(enable);
    return false;
}

bool LockInAmplifier::getSynchronousFilterEnabled() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSynchronousFilterEnabled();
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getSynchronousFilterEnabled();
    return false;
}

bool LockInAmplifier::workWithAdvancedFilter() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

bool LockInAmplifier::setDefaultAdvancedFilter() const
{
    return enableAdvancedFilter(false);
}

bool LockInAmplifier::enableAdvancedFilter(const bool &enable) const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->enableAdvancedFilterFilter(enable);
    return false;
}

bool LockInAmplifier::getAdvancedFilterEnabled() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getAdvancedFilterEnabled();
    return false;
}

bool LockInAmplifier::workWithEnablingRS232outputInterface() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::setDefaultRS232outputInterface() const
{
    return enableRS232outputInterface();
}

bool LockInAmplifier::enableRS232outputInterface() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->enableRS232outputInterface();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->enableRS232outputInterface();
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

bool LockInAmplifier::workWithAutoGain() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::autoGain() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->autoGain();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->autoGain();
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::workWithAutoReserve() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::autoReserve() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->autoReserve();
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::workWithAutoWideReserve() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::autoWideReserve() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->autoWideReserve();
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::workWithAutoCloseReserve() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::autoCloseReserve() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->autoCloseReserve();
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::workWithAutoPhase() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

bool LockInAmplifier::autoPhase() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->autoPhase();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->autoPhase();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->autoPhase();
    return false;
}

bool LockInAmplifier::workWithAutoOffset() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::autoOffset(const int &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->autoOffset(i);
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::workWithAutoOffsetAll() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::autoOffsetAll() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->autoOffsetAll();
    if (this->lockInAmplifierType == "SR844")
        return this->sr830->autoOffsetAll();
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::workWithAutoRange() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

bool LockInAmplifier::autoRange() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->autoRange();
    return false;
}

bool LockInAmplifier::workWithAutoScale() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return true;
    return false;
}

bool LockInAmplifier::autoScale() const
{
    if (this->lockInAmplifierType == "SR830")
        return false;
    if (this->lockInAmplifierType == "SR844")
        return false;
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->autoScale();
    return false;
}

bool LockInAmplifier::autoAll() const
{
    bool ans = true;

    if (this->lockInAmplifierType == "SR830") {
        ans &= this->sr830->autoGain();
        ans &= this->sr830->autoReserve();
        ans &= this->sr830->autoPhase();
        ans &= this->sr830->autoOffsetAll();
    }
    if (this->lockInAmplifierType == "SR844") {
        ans &= this->sr844->autoWideReserve();
        ans &= this->sr844->autoCloseReserve();
        ans &= this->sr844->autoGain();
        ans &= this->sr844->autoPhase();
        ans &= this->sr844->autoOffsetAll();
    }
    if (this->lockInAmplifierType == "SR865") {
        ans &= this->sr865->autoPhase();
        ans &= this->sr865->autoRange();
        ans &= this->sr865->autoScale();
    }

    return ans;
}

bool LockInAmplifier::workWithSampleRate() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

QString LockInAmplifier::getDefaultSampleRate() const
{
    if (this->lockInAmplifierType == "SR830")
        return "8 Hz";
    if (this->lockInAmplifierType == "SR844")
        return "8 Hz";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setDefaultSampleRate() const
{
    return setSampleRate(getDefaultSampleRate());
}

QStringList LockInAmplifier::getSampleRateList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSampleRateList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getSampleRateList();
    if (this->lockInAmplifierType == "865")
        return QStringList();
    return QStringList();
}

int LockInAmplifier::sampleRateNumberFromString(const QString &sampleRate_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->sampleRateNumberFromString(sampleRate_string);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->sampleRateNumberFromString(sampleRate_string);
    if (this->lockInAmplifierType == "865")
        return -1;
    return -1;
}

QString LockInAmplifier::sampleRateStringFromNumber(const int &sampleRate_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->sampleRateStringFromNumber(sampleRate_number);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->sampleRateStringFromNumber(sampleRate_number);
    if (this->lockInAmplifierType == "865")
        return "";
    return "";
}

bool LockInAmplifier::setSampleRate(const int &new_sampleRate) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSampleRate(new_sampleRate);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setSampleRate(new_sampleRate);
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

bool LockInAmplifier::setSampleRate(const QString &new_sampleRate) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setSampleRate(new_sampleRate);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setSampleRate(new_sampleRate);
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

QString LockInAmplifier::getSampleRate() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getSampleRate();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getSampleRate();
    if (this->lockInAmplifierType == "865")
        return "";
    return "";
}

bool LockInAmplifier::workWithBufferMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

QString LockInAmplifier::getDefaultBufferMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return "Shot";
    if (this->lockInAmplifierType == "SR844")
        return "Shot";
    if (this->lockInAmplifierType == "SR865")
        return "";
    return "";
}

bool LockInAmplifier::setDefaultBufferMode() const
{
    return setBufferMode(getDefaultBufferMode());
}

QStringList LockInAmplifier::getBufferModeList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getBufferModeList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getBufferModeList();
    if (this->lockInAmplifierType == "865")
        return QStringList();
    return QStringList();
}

int LockInAmplifier::bufferModeNumberFromString(const QString &bufferMode_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->bufferModeNumberFromString(bufferMode_string);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->bufferModeNumberFromString(bufferMode_string);
    if (this->lockInAmplifierType == "865")
        return -1;
    return -1;
}

QString LockInAmplifier::bufferModeStringFromNumber(const int &bufferMode_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->bufferModeStringFromNumber(bufferMode_number);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->bufferModeStringFromNumber(bufferMode_number);
    if (this->lockInAmplifierType == "865")
        return "";
    return "";
}

bool LockInAmplifier::setBufferMode(const int &new_bufferMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setBufferMode(new_bufferMode);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setBufferMode(new_bufferMode);
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

bool LockInAmplifier::setBufferMode(const QString &new_bufferMode) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setBufferMode(new_bufferMode);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setBufferMode(new_bufferMode);
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

QString LockInAmplifier::getBufferMode() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getBufferMode();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getBufferMode();
    if (this->lockInAmplifierType == "865")
        return "";
    return "";
}

bool LockInAmplifier::workWithDisplayData() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

bool LockInAmplifier::setDisplayData(const int &channel, const int &data) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setDisplayData(channel, data);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setDisplayData(channel, data);
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

bool LockInAmplifier::workWithBuffer() const
{
    if (this->lockInAmplifierType == "SR830")
        return true;
    if (this->lockInAmplifierType == "SR844")
        return true;
    if (this->lockInAmplifierType == "SR865")
        return false;
    return false;
}

int LockInAmplifier::getBufferSize() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getBufferSize();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getBufferSize();
    if (this->lockInAmplifierType == "865")
        return -1;
    return -1;
}

double LockInAmplifier::getPointFromBuffer(const int &channel, const int &number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->setDisplayData(channel, number);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->setDisplayData(channel, number);
    if (this->lockInAmplifierType == "865")
        return -1;
    return -1;
}

std::vector < double > LockInAmplifier::getChannelFromBuffer(const int &channel) const
{
    std::vector < double > res;
    res.clear();

    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getChannelFromBuffer(channel);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getChannelFromBuffer(channel);
    if (this->lockInAmplifierType == "865")
        return res;
    return res;
}

int LockInAmplifier::getBuffer(std::vector < double > &ch1, std::vector < double > &ch2) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getBuffer(ch1, ch2);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getBuffer(ch1, ch2);
    if (this->lockInAmplifierType == "865")
        return -1;
    return -1;
}

bool LockInAmplifier::startBuffer() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->startBuffer();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->startBuffer();
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

bool LockInAmplifier::pauseBuffer() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->pauseBuffer();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->pauseBuffer();
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

bool LockInAmplifier::stopBuffer() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->stopBuffer();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->stopBuffer();
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

QStringList LockInAmplifier::getOutputNumberList() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getOutputNumberList();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getOutputNumberList();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getOutputNumberList();
    return QStringList();
}

int LockInAmplifier::outpNumberFromString(const QString &outp_string) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->outpNumberFromString(outp_string);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->outpNumberFromString(outp_string);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->outpNumberFromString(outp_string);
    return -1;
}

QString LockInAmplifier::outpStringFromNumber(const int &outp_number) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->outpStringFromNumber(outp_number);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->outpStringFromNumber(outp_number);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->outpStringFromNumber(outp_number);
    return "";
}

double LockInAmplifier::getOUTP(const int &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getOUTP(i);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getOUTP(i);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getOUTP(i);
    return -1;
}

double LockInAmplifier::getOUTP(const QString &i) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getOUTP(i);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getOUTP(i);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getOUTP(i);
    return -1;
}

double LockInAmplifier::getX() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getX();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getX();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getX();
    return -1;
}

double LockInAmplifier::getY() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getY();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getY();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getY();
    return -1;
}

double LockInAmplifier::getR() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getR();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getR();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getR();
    return -1;
}

double LockInAmplifier::getTheta() const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getTheta();
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getTheta();
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getTheta();
    return -1;
}

bool LockInAmplifier::getAB(const int &Apos, double &A, const int &Bpos, double &B) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getAB(Apos, A, Bpos, B);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getAB(Apos, A, Bpos, B);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getAB(Apos, A, Bpos, B);
    return false;
}

bool LockInAmplifier::getABC(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getABC(Apos, A, Bpos, B, Cpos, C);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getABC(Apos, A, Bpos, B, Cpos, C);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getABC(Apos, A, Bpos, B, Cpos, C);
    return false;
}

bool LockInAmplifier::getABCDE(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C, const int &Dpos, double &D, const int &Epos, double &E) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getABCDE(Apos, A, Bpos, B, Cpos, C, Dpos, D, Epos, E);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getABCDE(Apos, A, Bpos, B, Cpos, C, Dpos, D, Epos, E);
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

bool LockInAmplifier::getABCDEF(const int &Apos, double &A, const int &Bpos, double &B, const int &Cpos, double &C, const int &Dpos, double &D, const int &Epos, double &E, const int &Fpos, double &F) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getABCDEF(Apos, A, Bpos, B, Cpos, C, Dpos, D, Epos, E, Fpos, F);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getABCDEF(Apos, A, Bpos, B, Cpos, C, Dpos, D, Epos, E, Fpos, F);
    if (this->lockInAmplifierType == "865")
        return false;
    return false;
}

bool LockInAmplifier::getXY(double &X, double &Y) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getXY(X, Y);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getXY(X, Y);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getXY(X, Y);
    return false;
}

bool LockInAmplifier::getRTheta(double &R, double &Theta) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getRTheta(R, Theta);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getRTheta(R, Theta);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getRTheta(R, Theta);
    return false;
}

bool LockInAmplifier::getRThetaFint(double &R, double &Theta, double &F) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getRThetaF(R, Theta, F);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getRThetaF(R, Theta, F);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getRThetaFint(R, Theta, F);
    return false;
}

bool LockInAmplifier::getRThetaFext(double &R, double &Theta, double &F) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getRThetaF(R, Theta, F);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getRThetaF(R, Theta, F);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getRThetaFext(R, Theta, F);
    return false;
}

bool LockInAmplifier::getRThetaF(double &R, double &Theta, double &F) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getRThetaF(R, Theta, F);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getRThetaF(R, Theta, F);
    if (this->lockInAmplifierType == "SR865")
        return this->sr865->getRThetaF(R, Theta, F);
    return false;
}

bool LockInAmplifier::getXYRThetaF(double &X, double&Y, double &R, double &Theta, double &F) const
{
    if (this->lockInAmplifierType == "SR830")
        return this->sr830->getXYRThetaF(X, Y, R, Theta, F);
    if (this->lockInAmplifierType == "SR844")
        return this->sr844->getXYRThetaF(X, Y, R, Theta, F);
    if (this->lockInAmplifierType == "865")
        return false;
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
