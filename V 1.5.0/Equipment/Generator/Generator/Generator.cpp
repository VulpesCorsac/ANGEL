#include "Generator.h"

Generator::Generator(QObject *parent) : QObject(parent)
{
    toNULL();
}

Generator::Generator(const QString &portName, const int &baudrate, const int &stopBit, const int &parity, const QString &new_generatorModel)
{
    toNULL();
    setConnection(portName, baudrate, stopBit, parity, new_generatorModel);
}

Generator::Generator(QSerialPort *serial, const QString &new_generatorModel)
{
    toNULL();
    QString _generatorModel = new_generatorModel.trimmed().toUpper();
    if (_generatorModel == "DS335") {
        if (this->ds335 == nullptr)
            this->ds335 = new DS335(serial->portName(), serial->baudRate());
        else
            this->ds335->setSerial(serial);
    }
    if (_generatorModel == "DS345") {
        if (this->ds345 == nullptr)
            this->ds345 = new DS345(serial->portName(), serial->baudRate());
        else
            this->ds345->setSerial(serial);
    }
    if (_generatorModel.isEmpty() ||
        _generatorModel == "AUTO")
        autoSetGeneratorModel(serial);
}

Generator::Generator(DS335 *new_ds335)
{
    toNULL();
    setDS335(new_ds335);
}

Generator::Generator(DS345 *new_ds345)
{
    toNULL();
    setDS345(new_ds345);
}

Generator::~Generator()
{
    this->generatorModel.clear();
    if (this->ds335 != nullptr)
        this->ds335->~DS335();
    delete this->ds335;
    if (this->ds345 != nullptr)
        this->ds345->~DS345();
    delete this->ds345;
    if (this->srs != nullptr)
        this->srs->~SRS();
    delete this->srs;

    this->ds335 = nullptr;
    this->ds345 = nullptr;
    this->srs = nullptr;
}

void Generator::toNULL()
{
    if (this->ds335 != nullptr) {
        this->ds335 = nullptr;
    }
    if (this->ds345 != nullptr) {
        this->ds345 = nullptr;
    }
    if (this->srs != nullptr) {
        this->srs = nullptr;
    }

    init();

    return;
}

void Generator::init()
{
    if (this->srs == nullptr)
        this->srs = new SRS();

    if (this->srs != nullptr) {
        connect(this->srs,   SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString)),    Qt::DirectConnection);
        connect(this->srs,   SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->srs,   SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)),  Qt::DirectConnection);
        connect(this->srs,   SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)),  Qt::DirectConnection);
    }
    if (this->ds335 != nullptr) {
        connect(this->ds335, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString)),    Qt::DirectConnection);
        connect(this->ds335, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->ds335, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)),  Qt::DirectConnection);
        connect(this->ds335, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)),  Qt::DirectConnection);
    }
    if (this->ds345 != nullptr) {
        connect(this->ds345, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString)),    Qt::DirectConnection);
        connect(this->ds345, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
        connect(this->ds345, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)),  Qt::DirectConnection);
        connect(this->ds345, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)),  Qt::DirectConnection);
    }

    return;
}

Generator_State Generator::getState()
{
    Generator_State current_state;

    current_state.model = this->generatorModel;

    if (workWithAmplitude())
        current_state.amplitude = getAmplitude("VR");
    if (workWithOffset())
        current_state.offset = getOffset();
    if (workWithFrequency())
        current_state.frequency = getFrequency();
    if (workWithFunction())
        current_state.function = getFunction();
    if (workWithInverse())
        current_state.inverse = getInverse();
    if (workWithSynchronization())
        current_state.synchronization = getSynchronization();
    if (workWithOutputImpedance())
        current_state.outputImpedance = getOutputImpedance();
    if (workWithAMDepth())
        current_state.amDepth = getAMDepth();
    if (workWithFMSpan())
        current_state.fmSpan = getFMSpan();
    if (workWithModulationFunction())
        current_state.modulationFunction = getModulationFunction();
    if (workWithModulationEnabled())
        current_state.modulationEnabled = getModulationEnabled();
    if (workWithModulationType())
        current_state.modulationType = getModulationType();
    if (workWithModulationRate())
        current_state.modulationRate = getModulationRate();
    if (workWithModulationSpan())
        current_state.modulationSpan = getModulationSpan();

    return current_state;
}

bool Generator::setState(const Generator_State &state)
{
    bool result = true;

    if (state.model != this->generatorModel)
        result = false;
    if (workWithAmplitude())
        result &= this->setAmplitude(state.amplitude, "VR");
    if (workWithOffset())
        result &= this->setOffset(state.offset);
    if (workWithFrequency())
        result &= this->setFrequency(state.frequency);
    if (workWithFunction())
        result &= this->setFunction(state.function);
    if (workWithInverse())
        result &= this->setInverse(state.inverse);
    if (workWithSynchronization())
        result &= this->setSynchronization(state.synchronization);
    if (workWithOutputImpedance())
        result &= this->setOutputImpedance(state.outputImpedance);
    if (workWithAMDepth())
        result &= this->setAMDepth(state.amDepth);
    if (workWithFMSpan())
        result &= this->setFMSpan(state.fmSpan);
    if (workWithModulationFunction())
        result &= this->setModulationFunction(state.modulationFunction);
    if (workWithModulationEnabled())
        result &= this->setModulationEnabled(state.modulationEnabled);
    if (workWithModulationType())
        result &= this->setModulationType(state.modulationType);
    if (workWithModulationRate())
        result &= this->setModulationRate(state.modulationRate);
    if (workWithModulationSpan())
        result &= this->setModulationSpan(state.modulationSpan);

    return result;
}

void Generator::wait(const double &x) const
{
    QTest::qWait(_round(this->waitTime*x));

    return;
}

void Generator::setDS335(DS335 *new_ds335)
{
    this->ds335 = new_ds335;
    init();

    return;
}

DS335* Generator::getDS335() const
{
    return this->ds335;
}

void Generator::setDS345(DS345 *new_ds345)
{
    this->ds345 = new_ds345;
    init();

    return;
}

DS345* Generator::getDS345() const
{
    return this->ds345;
}

bool Generator::openSerial(QSerialPort *serial) const
{
    return this->srs->openSerial(serial);
}

QStringList Generator::getSupportedModels() const
{
    QStringList ans;

    ans.push_back("DS335");
    ans.push_back("DS345");

    ans.push_back("AUTO");

    return ans;
}

void Generator::setGeneratorModel(const QString &new_generatorModel)
{
    this->generatorModel = new_generatorModel;

    return;
}

QString Generator::getGeneratorModel() const
{
    return this->generatorModel;
}

bool Generator::autoSetGeneratorModel(QSerialPort *serial)
{
    this->generatorModel.clear();

    QString model = this->srs->autoDetect(serial);

    if (model.trimmed().isEmpty()) {
        this->srs->disconnect(serial);

        return false;
    }

    this->generatorModel = model;
    if (model == "DS335") {
        this->ds335 = new DS335();
        this->ds335->setSerial(serial);
        init();

        return true;
    }
    if (model == "DS345") {
        this->ds345 = new DS345();
        this->ds345->setSerial(serial);
        init();

        return true;
    }

    this->srs->disconnect(serial);

    return false;
}

bool Generator::autoSetGeneratorModel(const QString &portName, const int &baudrate)
{
    QSerialPort *serial = new QSerialPort();

    serial->setPortName(portName);
    serial->setBaudRate(baudrate);
    serial->setDataBits(QSerialPort::Data8);

    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    if(autoSetGeneratorModel(serial))
        return true;
    serial->setParity(QSerialPort::OddParity);
    serial->setStopBits(QSerialPort::OneStop);
    if(autoSetGeneratorModel(serial))
        return true;
    serial->setParity(QSerialPort::EvenParity);
    serial->setStopBits(QSerialPort::OneStop);
    if(autoSetGeneratorModel(serial))
        return true;
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::TwoStop);
    if(autoSetGeneratorModel(serial))
        return true;
    serial->setParity(QSerialPort::OddParity);
    serial->setStopBits(QSerialPort::TwoStop);
    if(autoSetGeneratorModel(serial))
        return true;
    serial->setParity(QSerialPort::EvenParity);
    serial->setStopBits(QSerialPort::TwoStop);
    if(autoSetGeneratorModel(serial))
        return true;

    return autoSetGeneratorModel(serial);
}

bool Generator::autoSetGeneratorModel(const QString &portName)
{
    QStringList baudrates = this->srs->getBaudrateList();
    for (int baud = 0;baud < baudrates.size();baud++) {
        int baudrate = baudrates[baud].toInt();

        if (autoSetGeneratorModel(portName, baudrate))
            return true;
    }

    return false;
}

bool Generator::setConnection(const QString &portName, const int &baudrate, const int &stopBit, const int &parity, const QString &new_generatorModel)
{
	QString _generatorModel = new_generatorModel.trimmed().toUpper();
    if (_generatorModel == "DS335") {
        if (this->ds335 == nullptr)
            this->ds335 = new DS335();
        this->generatorModel = "DS335";
        init();

        return this->ds335->setConnection(portName, baudrate, stopBit, parity);
    }
    if (_generatorModel == "DS345") {
        if (this->ds345 == nullptr)
            this->ds345 = new DS345();
        this->generatorModel = "DS345";
        init();

        return this->ds345->setConnection(portName, baudrate, stopBit, parity);
    }
    if (_generatorModel.isEmpty() ||
        _generatorModel == "AUTO") {
        QSerialPort *serial = new QSerialPort();
        serial->setPortName(portName);
        serial->setBaudRate(baudrate);

        if (stopBit == 1)
            serial->setStopBits(QSerialPort::OneStop);
        if (stopBit == 2)
            serial->setStopBits(QSerialPort::TwoStop);
        if (stopBit == 3)
            serial->setStopBits(QSerialPort::OneAndHalfStop);

        if (parity == 0)
            serial->setParity(QSerialPort::NoParity);
        if (parity == 1)
            serial->setParity(QSerialPort::OddParity);
        if (parity == 2)
            serial->setParity(QSerialPort::EvenParity);
        
        return autoSetGeneratorModel(serial);
    }

    return false;
}

bool Generator::setConnectionBrute(const QString &portName, const QString &new_generatorModel)
{
    QStringList baudRates = this->srs->getBaudrateList();
    for (int i = 0;i < baudRates.length();i++) {
        if (setConnection(portName, baudRates[i].toInt(), 1, 0, new_generatorModel))
            return true;
        if (setConnection(portName, baudRates[i].toInt(), 1, 1, new_generatorModel))
            return true;
        if (setConnection(portName, baudRates[i].toInt(), 1, 2, new_generatorModel))
            return true;
        if (setConnection(portName, baudRates[i].toInt(), 2, 0, new_generatorModel))
            return true;
        if (setConnection(portName, baudRates[i].toInt(), 2, 1, new_generatorModel))
            return true;
        if (setConnection(portName, baudRates[i].toInt(), 2, 2, new_generatorModel))
            return true;
    }

    return false;
}

bool Generator::disconnect() const
{
    if (this->generatorModel == "DS335") {
        this->ds335->disconnect();

        return true;
    }
    if (this->generatorModel == "DS345") {
        this->ds345->disconnect();

        return true;
    }

    return false;
}

QStringList Generator::getBaudrateList() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getBaudrateList();
    if (this->generatorModel == "DS345")
        return this->ds345->getBaudrateList();
    return QStringList();
}

bool Generator::isValidBaudrateAll(const int &baudrate) const
{
    DS335 temp335;
    DS345 temp345;
    bool answer = temp335.isValidBaudrate(baudrate);
    answer &= temp345.isValidBaudrate(baudrate);
    return answer;
}

bool Generator::isValidBaudrate(const int &baudrate) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->isValidBaudrate(baudrate);
    if (this->generatorModel == "DS345")
        return this->ds345->isValidBaudrate(baudrate);
    return false;
}

bool Generator::changeBaudrate(const int &baudrate)
{
    if (this->generatorModel == "DS335")
        return this->ds335->changeBaudrate(baudrate);
    if (this->generatorModel == "DS345")
        return this->ds345->changeBaudrate(baudrate);
    return false;
}

void Generator::setWriteTimeout(const int &new_writeTimeout)
{
    if (this->generatorModel == "DS335")
        return this->ds335->setWriteTimeout(new_writeTimeout);
    if (this->generatorModel == "DS345")
        return this->ds345->setWriteTimeout(new_writeTimeout);
    return;
}

int Generator::getWriteTimeout() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getWriteTimeout();
    if (this->generatorModel == "DS345")
        return this->ds345->getWriteTimeout();
    return 0;
}

void Generator::setReadTimeout(const int &new_readTimeout)
{
    if (this->generatorModel == "DS335")
        return this->ds335->setReadTimeout(new_readTimeout);
    if (this->generatorModel == "DS345")
        return this->ds345->setReadTimeout(new_readTimeout);
    return;
}

int Generator::getReadTimeout() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getReadTimeout();
    if (this->generatorModel == "DS345")
        return this->ds345->getReadTimeout();
    return 0;
}

void Generator::setReadWaitTimeout(const int &new_readWaitTimeout)
{
    if (this->generatorModel == "DS335")
        return this->ds335->setReadWaitTimeout(new_readWaitTimeout);
    if (this->generatorModel == "DS345")
        return this->ds345->setReadWaitTimeout(new_readWaitTimeout);
    return;
}

int Generator::getReadWaitTimeout() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getReadWaitTimeout();
    if (this->generatorModel == "DS345")
        return this->ds345->getReadWaitTimeout();
    return 0;
}

void Generator::setAttemptsToConnect(const int &new_attemptsToConnect)
{
    if (this->generatorModel == "DS335")
        return this->ds335->setAttemptsToConnect(new_attemptsToConnect);
    if (this->generatorModel == "DS345")
        return this->ds345->setAttemptsToConnect(new_attemptsToConnect);
    return;
}

int Generator::getAttemptsToConnect() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getAttemptsToConnect();
    if (this->generatorModel == "DS345")
        return this->ds345->getAttemptsToConnect();
    return 0;
}

bool Generator::isActive() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->isActive();
    if (this->generatorModel == "DS345")
        return this->ds345->isActive();
    return false;
}

bool Generator::send(const QString &command, QString &response, const bool &wait_for_response) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->send(command, response, wait_for_response);
    if (this->generatorModel == "DS345")
        return this->ds345->send(command, response, wait_for_response);
    return false;
}

bool Generator::sendCommand(const QString &command) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->sendCommand(command);
    if (this->generatorModel == "DS345")
        return this->ds345->sendCommand(command);
    return false;
}

bool Generator::sendQuery(const QString &command, QString &response) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->sendQuery(command, response);
    if (this->generatorModel == "DS345")
        return this->ds345->sendQuery(command, response);
    return false;
}

QString Generator::ask(const QString &command) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->ask(command);
    if (this->generatorModel == "DS345")
        return this->ds345->ask(command);
    return "";
}

bool Generator::getIDN(QString &idn) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getIDN(idn);
    if (this->generatorModel == "DS345")
        return this->ds345->getIDN(idn);
    return false;
}

bool Generator::test() const
{
    bool ans = true;

    qDebug() << "Testing Generator";

    setDefaultSettings();

    wait();

    if (workWithInverse()) {
        for (int i = 1;i >= 0;i--) {
            if (i == 0)
                qDebug() << "Generator inverse disbling";
            else
                qDebug() << "Generator inverse enabling";

            if (!setInverse(i)) {
                qDebug() << "Generator inverse comand failed";

                ans = false;
            }

            wait();

            if (getInverse() == (bool) i)
                qDebug() << "Generator inverse settings accepted";
            else {
                qDebug() << "Generator inverse settings not accepted";

                ans = false;
            }
        }

        setDefaultInverse();
        wait();
    }

    if (workWithSynchronization()) {
        for (int i = 0;i <= 1;i++) {
            if (i == 0)
                qDebug() << "Generator synchronization disbling";
            else
                qDebug() << "Generator synchronization enabling";

            if (!setSynchronization(i)) {
                qDebug() << "Generator synchronization comand failed";

                ans = false;
            }

            wait();

            if (getSynchronization() == (bool) i)
                qDebug() << "Generator synchronization settings accepted";
            else {
                qDebug() << "Generator synchronization settings not accepted";

                ans = false;
            }
        }

        setDefaultSynchronization();
        wait();
    }

   if (workWithModulationEnabled()) {
        for (int i = 1;i >= 0;i--) {
            if (i == 0)
                qDebug() << "Generator modulation disbling";
            else
                qDebug() << "Generator modulation enabling";

            if (!setModulationEnabled(i)) {
                qDebug() << "Generator modulation comand failed";

                ans = false;
            }

            wait();

            if (getModulationEnabled() == (bool) i)
                qDebug() << "Generator modulation settings accepted";
            else {
                qDebug() << "Generator modulation settings not accepted";

                ans = false;
            }
        }

        setDefaultModulationEnabled();
        wait();
    }

    if (workWithAMDepth()) {
        if (!setAMDepth(getMinAMDepth())) {
            qDebug() << "Generator AM depth command failed";

            ans = false;
        }

        wait();

        if (_abs(getAMDepth() - getMinAMDepth()) < this->Err)
            qDebug() << "Min AM depth passed";
        else {
            qDebug() << "Min AM depth failed";

            ans = false;
        }

        wait();

        if (!setAMDepth(getMaxAMDepth())) {
            qDebug() << "Generator AM depth command failed";

            ans = false;
        }

        wait();

        if (_abs(getAMDepth() - getMaxAMDepth()) < this->Err)
            qDebug() << "Max AM depth passed";
        else {
            qDebug() << "Max AM depth failed";

            ans = false;
        }

        setDefaultAMDepth();
        wait();
    }

    if (workWithModulationRate()) {
        if (!setModulationRate(getMinModulationRate())) {
            qDebug() << "Generator modulation rate command failed";

            ans = false;
        }

        wait();

        if (_abs(getModulationRate() - getMinModulationRate()) < this->Err)
            qDebug() << "Min modulation rate passed";
        else {
            qDebug() << "Min modulation rate failed";

            ans = false;
        }

        wait();

        if (!setModulationRate(getMaxModulationRate())) {
            qDebug() << "Generator modulation rate command failed";

            ans = false;
        }

        wait();

        if (_abs(getModulationRate() - getMaxModulationRate()) < this->Err)
            qDebug() << "Max modulation rate passed";
        else {
            qDebug() << "Max modulation rate failed";

            ans = false;
        }

        setDefaultModulationRate();
        wait();
    }

    if (workWithModulationType()) {
        foreach (const QString &type, getModulationTypeList()) {
            qDebug() << "Generator modulation type was changed to" << type;

            if (!setModulationType(type)) {
                qDebug() << "Generator modulation type command failed";

                ans = false;
            }

            wait();

            if (getModulationType() == type)
                qDebug() << "Modulation type passed";
            else {
                qDebug() << "Modulation type failed";

                ans = false;
            }
        }

        setDefaultModulationType();
        wait();
    }

    if (workWithOffset()) {
        if (!setFunction(getDefaultFunction())) {
            qDebug() << "Generator amplitude command failed";

            ans = false;
        }

        wait();

        if (!setAmplitude(getMinAmplitude(getDefaultFunction(), "VP"), "VP")) {
            qDebug() << "Generator amplitude command failed";

            ans = false;
        }

        wait();

        if (!setOffset(getMinOffset())) {
            qDebug() << "Generator offset command failed";

            ans = false;
        }

        wait();

        if (_abs(getOffset() - getMinOffset()) < this->Err)
            qDebug() << "Min offset passed";
        else {
            qDebug() << "Min offset failed";

            ans = false;
        }

        wait();

        if (!setOffset(getMaxOffset()/5)) {
            qDebug() << "Generator offset command failed";

            ans = false;
        }

        wait();

        if (_abs(getOffset() - getMaxOffset()/5) < this->Err)
            qDebug() << "Max offset passed";
        else {
            qDebug() << "Max offset failed";

            ans = false;
        }

        setDefaultOffset();
        wait();
    }

    wait();

      foreach (const QString &waveform, getFunctionList()) {
        qDebug() << "Generator waveform was changed to" << waveform;

        if (!setFunction(waveform)) {
            qDebug() << "Generator waveform command failed";

            ans = false;
        }

        wait();

        if (waveform != "NOISE") {

            if (!setFrequency(getMinFrequency(waveform))) {
                qDebug() << "Generator frequency command failed";

                ans = false;
            }

            wait();

            if (_abs(getFrequency() - getMinFrequency(waveform)) < this->Err)
                qDebug() << "Min frequency passed";
            else {
                qDebug() << "Min frequency failed";

                ans = false;
            }

            wait();

            if (!setFrequency(getMaxFrequency(waveform))) {
                qDebug() << "Generator frequency command failed";

                ans = false;
            }

            wait();

            if (_abs(getFrequency() - getMaxFrequency(waveform)) < this->Err)
                qDebug() << "Max frequency passed";
            else {
                qDebug() << "Max frequency failed";

                ans = false;
            }

            wait();
        }

        foreach (const QString &unit, getAmplitudeTypeList()) {
            qDebug() << "Generator amplitude type was set to" << unit;

            wait();

            if (workWithOutputImpedance()) {
                foreach (const QString &outputZ, getOutputImpedanceList()) {
                    qDebug() << "Generator outputZ was changed to" << outputZ;

                    if (!setOutputImpedance(outputZ)) {
                        qDebug() << "Generator output impedance command failed";

                        ans = false;
                    }

                    wait();

                    if (unit == "VR") {
                        if (!setAmplitude(getMinAmplitude(waveform, outputZ, true), unit)) {
                            qDebug() << "Generator amplitude command failed";

                            ans = false;
                        }

                        wait();

                        qDebug() << "Min amplitude =" << getMinAmplitude(waveform, outputZ, true);
                        qDebug() << "Current amplitude =" << getAmplitude(unit);

                        if (_abs(getAmplitude(unit) -  getMinAmplitude(waveform, outputZ, true)) < this->Err)
                            qDebug() << "Min amplitude passed";
                        else {
                            qDebug() << "Min amplitude failed";

                            ans = false;
                        }

                        wait();

                        if (!setAmplitude(getMaxAmplitude(waveform, outputZ, true), unit)) {
                            qDebug() << "Generator amplitude command failed";

                            ans = false;
                        }

                        wait();

                        qDebug() << "Max amplitude =" << getMaxAmplitude(waveform, outputZ, true);
                        qDebug() << "Current amplitude =" << getAmplitude(unit);

                        if (_abs(getAmplitude(unit) -  getMaxAmplitude(waveform, outputZ, true)) < this->Err)
                            qDebug() << "Max amplitude passed";
                        else {
                            qDebug() << "Max amplitude failed";

                            ans = false;
                        }

                        wait();
                    }
                    if (unit == "VP") {
                        if (!setAmplitude(getMinAmplitude(waveform, outputZ, false), unit)) {
                            qDebug() << "Generator amplitude command failed";

                            ans = false;
                        }

                        wait();

                        qDebug() << "Min amplitude =" << getMinAmplitude(waveform, outputZ, false);
                        qDebug() << "Current amplitude =" << getAmplitude(unit);

                        if (_abs(getAmplitude(unit) -  getMinAmplitude(waveform, outputZ, false)) < this->Err)
                            qDebug() << "Min amplitude passed";
                        else {
                            qDebug() << "Min amplitude failed";

                            ans = false;
                        }

                        wait();

                        if (!setAmplitude(getMaxAmplitude(waveform, outputZ, false), unit)) {
                            qDebug() << "Generator amplitude command failed";

                            ans = false;
                        }

                        wait();

                        qDebug() << "Max amplitude =" << getMaxAmplitude(waveform, outputZ, false);
                        qDebug() << "Current amplitude =" << getAmplitude(unit);

                        if (_abs(getAmplitude(unit) -  getMaxAmplitude(waveform, outputZ, false)) < this->Err)
                            qDebug() << "Max amplitude passed";
                        else {
                            qDebug() << "Max amplitude failed";

                            ans = false;
                        }

                        wait();
                    }

                    wait();
                }
            } else {
                if (!setAmplitude(getMinAmplitude(waveform, unit), unit)) {
                    qDebug() << "Generator amplitude command failed";

                    ans = false;
                }

                wait();

                if (_abs(getAmplitude(unit) - getMinAmplitude(waveform, unit)) < this->Err)
                    qDebug() << "Min amplitude passed";
                else {
                    qDebug() << "Min amplitude failed";

                    ans = false;
                }

                wait();

                if (!setAmplitude(getMaxAmplitude(waveform, unit), unit)) {
                    qDebug() << "Generator amplitude command failed";

                    ans = false;
                }

                wait();

                if (_abs(getAmplitude(unit) - getMaxAmplitude(waveform, unit)) < this->Err)
                    qDebug() << "Max amplitude passed";
                else {
                    qDebug() << "Max amplitude failed";

                    ans = false;
                }

                wait();
            }
        }

        wait();

        if (workWithFMSpan()) {
            if (waveform != "NOISE") {
                if (!setFMSpan(getMinFMSpan(waveform))) {
                    qDebug() << "Generator FM span command failed";

                    ans = false;
                }

                wait();

                if (_abs(getFMSpan() - getMinFMSpan(waveform)) < this->Err)
                    qDebug() << "Min FM span passed";
                else {
                    qDebug() << "Min FM span failed";

                    ans = false;
                }

                wait();
/*
                if (!setFMSpan(getMaxFMSpan(waveform))) {
                    qDebug() << "Generator FM span command failed";

                    ans = false;
                }

                wait();

                if (_abs(getFMSpan() - getMaxFMSpan(waveform)) < this->Err)
                    qDebug() << "Max FM span passed";
                else {
                    qDebug() << "Max FM span failed";

                    ans = false;
                }
//*/
                setDefaultFMSpan();
                wait();
            }
        }

        if (workWithModulationSpan()) {
            if (waveform != "NOISE" &&
                waveform != "ARBITRARY") {
                if (!setModulationSpan(getMinModulationSpan(waveform))) {
                    qDebug() << "Generator modulation span command failed";

                    ans = false;
                }

                wait();

                if (_abs(getModulationSpan() - getMinModulationSpan(waveform)) < this->Err)
                    qDebug() << "Min modulation span passed";
                else {
                    qDebug() << "Min modulation span failed";

                    ans = false;
                }

                wait();
/*
                if (!setModulationSpan(getMaxModulationSpan(waveform))) {
                    qDebug() << "Generator modulation span command failed";

                    ans = false;
                }

                wait();

                if (_abs(getModulationSpan() - getMaxModulationSpan(waveform)) < this->Err)
                    qDebug() << "Max modulation span passed";
                else {
                    qDebug() << "Max modulation span failed";

                    ans = false;
                }
//*/
                setDefaultModulationSpan();
                wait();
            }
        }
    }

    return ans;
}

bool Generator::setDefaultSettings() const
{
    bool ans = true;

    if (workWithFunction())
        ans &= setDefaultFunction();
    if (workWithAmplitude())
        ans &= setDefaultAmplitude();
    if (workWithOffset())
        ans &= setDefaultOffset();
    if (workWithFrequency())
        ans &= setDefaultFrequency();
    if (workWithInverse())
        ans &= setDefaultInverse();
    if (workWithSynchronization())
        ans &= setDefaultSynchronization();
    if (workWithOutputImpedance())
        ans &= setDefaultOutputImpedance();
    if (workWithAMDepth())
        ans &= setDefaultAMDepth();
    if (workWithFMSpan())
        ans &= setDefaultFMSpan();
    if (workWithModulationFunction())
        ans &= setDefaultModulationFunction();
    if (workWithModulationEnabled())
        ans &= setDefaultModulationEnabled();
    if (workWithModulationType())
        ans &= setDefaultModulationType();
    if (workWithModulationRate())
        ans &= setDefaultModulationRate();
    if (workWithModulationSpan())
        ans &= setDefaultModulationSpan();

    return ans;
}

double Generator::getAverageInputTime() const
{
    if (this->generatorModel == "DS335")
        return 1;
    if (this->generatorModel == "DS345")
        return 1;
    return 1E5;
}

double Generator::getAverageOutputTime() const
{
    if (this->generatorModel == "DS335")
        return 30;
    if (this->generatorModel == "DS345")
        return 30;
    return 1E5;
}

bool Generator::workWithAmplitude() const
{
    if (this->generatorModel == "DS335")
        return true;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

double Generator::getDefaultAmplitude() const
{
    if (this->generatorModel == "DS335")
        return 1;
    if (this->generatorModel == "DS345")
        return 1;
    return -1;
}

bool Generator::setDefaultAmplitude() const
{
    return setAmplitude(getDefaultAmplitude(), "VR");
}

QStringList Generator::getAmplitudeTypeList() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getAmplitudeTypeList();
    if (this->generatorModel == "DS345")
        return this->ds345->getAmplitudeTypeList();
    return QStringList();
}

double Generator::getMinAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getMinAmplitude(waveform, outputZ, VRMS);
    if (this->generatorModel == "DS345") {
        if (VRMS)
            return this->ds345->getMinAmplitude(waveform, "VR");
        else
            return this->ds345->getMinAmplitude(waveform, "VP");
    }
    return -1;
}

double Generator::getMinAmplitude(const QString &waveform, const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->getMinAmplitude(waveform, getOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getMinAmplitude(waveform, getOutputImpedance(), true);
        else
            return -1;
    }
    if (this->generatorModel == "DS345")
        return this->ds345->getMinAmplitude(waveform, unit);
    return -1;
}

double Generator::getMinAmplitudeCurrent(const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->getMinAmplitude(getFunction(), getOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getMinAmplitude(getFunction(), getOutputImpedance(), false);
        else
            return -1;
    }
    if (this->generatorModel == "DS345")
        return this->ds345->getMinAmplitude(getFunction(), unit);
    return -1;
}

double Generator::getMaxAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getMaxAmplitude(waveform, outputZ, VRMS);
    if (this->generatorModel == "DS345") {
        if (VRMS)
            return this->ds345->getMaxAmplitude(waveform, "VR");
        else
            return this->ds345->getMaxAmplitude(waveform, "VP");
    }
    return -1;
}

double Generator::getMaxAmplitude(const QString &waveform, const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->getMaxAmplitude(waveform, getDefaultOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getMaxAmplitude(waveform, getDefaultOutputImpedance(), true);
        else
            return -1;
    }
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxAmplitude(waveform, unit);
    return -1;
}

double Generator::getMaxAmplitudeCurrent(const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->getMaxAmplitude(getFunction(), getOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getMaxAmplitude(getFunction(), getOutputImpedance(), false);
        else
            return -1;
    }
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxAmplitude(getFunction(), unit);
    return -1;
}

double Generator::getStepAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getStepAmplitude(waveform, outputZ, VRMS);
    if (this->generatorModel == "DS345") {
        if (VRMS)
            return this->ds345->getStepAmplitude(waveform, "VR");
        else
            return this->ds345->getStepAmplitude(waveform, "VP");
    }
    return -1;
}

double Generator::getStepAmplitude(const QString &waveform, const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->getStepAmplitude(waveform, getDefaultOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getStepAmplitude(waveform, getDefaultOutputImpedance(), true);
        else
            return -1;
    }
    if (this->generatorModel == "DS345")
        return this->ds345->getStepAmplitude(waveform, unit);
    return -1;
}

double Generator::getStepAmplitudeCurrent(const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->getStepAmplitude(getFunction(), getOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getStepAmplitude(getFunction(), getOutputImpedance(), false);
        else
            return -1;
    }
    if (this->generatorModel == "DS345")
        return this->ds345->getStepAmplitude(getFunction(), unit);
    return -1;
}

double Generator::getDecimalsAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getDecimalsAmplitude(waveform, outputZ, VRMS);
    if (this->generatorModel == "DS345") {
        if (VRMS)
            return this->ds345->getDecimalsAmplitude(waveform, "VR");
        else
            return this->ds345->getDecimalsAmplitude(waveform, "VP");
    }
    return -1;
}

double Generator::getDecimalsAmplitude(const QString &waveform, const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->getDecimalsAmplitude(waveform, getDefaultOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getDecimalsAmplitude(waveform, getDefaultOutputImpedance(), true);
        else
            return -1;
    }
    if (this->generatorModel == "DS345")
        return this->ds345->getDecimalsAmplitude(waveform, unit);
    return -1;
}

double Generator::getDecimalsAmplitudeCurrent(const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->getDecimalsAmplitude(getFunction(), getOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getDecimalsAmplitude(getFunction(), getOutputImpedance(), false);
        else
            return -1;
    }
    if (this->generatorModel == "DS345")
        return this->ds345->getDecimalsAmplitude(getFunction(), unit);
    return -1;
}

bool Generator::isValidAmplitude(const double &amplitude, const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->isValidAmplitude(amplitude, waveform, outputZ, VRMS);
    if (this->generatorModel == "DS345") {
        if (VRMS)
            return this->ds345->isValidAmplitude(amplitude, waveform, "VR");
        else
            return this->ds345->isValidAmplitude(amplitude, waveform, "VP");
    }
    return false;
}

bool Generator::isValidAmplitude(const double &amplitude, const QString &waveform, const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->isValidAmplitude(amplitude, waveform, getDefaultOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->isValidAmplitude(amplitude, waveform, getDefaultOutputImpedance(), true);
        else
            return false;
    }
    if (this->generatorModel == "DS345")
        return this->ds345->isValidAmplitude(amplitude, waveform, unit);
    return false;
}

bool Generator::isValidAmplitudeType(const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR" || unit == "VP")
            return true;
        else
            return false;
    }
    if (this->generatorModel == "DS345")
        return this->ds345->isValidAmplitudeType(unit);
    return false;
}

bool Generator::setAmplitude(const double &amplitude, const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->setAmplitude(amplitude, true);
        return this->ds335->setAmplitude(amplitude, false);
    }
    if (this->generatorModel == "DS345")
        return this->ds345->setAmplitude(amplitude, unit);
    return false;
}

double Generator::getAmplitude(const QString &unit) const
{
    if (this->generatorModel == "DS335") {
        if (unit == "VR")
            return this->ds335->getAmplitude(true);
        return this->ds335->getAmplitude(false);
    }
    if (this->generatorModel == "DS345")
        return this->ds345->getAmplitude(unit);
    return -1;
}

bool Generator::workWithOffset() const
{
    if (this->generatorModel == "DS335")
        return true;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

double Generator::getDefaultOffset() const
{
    if (this->generatorModel == "DS335")
        return 0;
    if (this->generatorModel == "DS345")
        return 0;
    return -1;
}

bool Generator::setDefaultOffset() const
{
    return setOffset(getDefaultOffset());
}

double Generator::getMinOffset() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getMinOffset();
    if (this->generatorModel == "DS345")
        return this->ds345->getMinOffset();
    return -1;
}

double Generator::getMaxOffset() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getMaxOffset();
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxOffset();
    return -1;
}

double Generator::getStepOffset() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getStepOffset();
    if (this->generatorModel == "DS345")
        return this->ds345->getStepOffset();
    return -1;
}

double Generator::getDecimalsOffset() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getDecimalsOffset();
    if (this->generatorModel == "DS345")
        return this->ds345->getDecimalsOffset();
    return -1;
}

bool Generator::isValidOffset(const double &offset) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->isValidOffset(offset);
    if (this->generatorModel == "DS345")
        return this->ds345->isValidOffset(offset);
    return false;
}

bool Generator::setOffset(const double &offset) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->setOffset(offset);
    if (this->generatorModel == "DS345")
        return this->ds345->setOffset(offset);
    return false;
}

double Generator::getOffset() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getOffset();
    if (this->generatorModel == "DS345")
        return this->ds345->getOffset();
    return -1;

}

bool Generator::workWithFrequency() const
{
    if (this->generatorModel == "DS335")
        return true;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

double Generator::getDefaultFrequency() const
{
    if (this->generatorModel == "DS335")
//        return this->ds335->getMinFrequency(getDefaultFunction());
        return 1E4;
    if (this->generatorModel == "DS345")
//        return this->ds345->getMinFrequency(getDefaultFunction());
        return 1E4;
    return -1;
}

bool Generator::setDefaultFrequency() const
{
    return setFrequency(getDefaultFrequency());
}

double Generator::getMinFrequency(const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getMinFrequency(waveform);
    if (this->generatorModel == "DS345")
        return this->ds345->getMinFrequency(waveform);
    return -1;
}

double Generator::getMinFrequencyCurrent() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getMinFrequency(getFunction());
    if (this->generatorModel == "DS345")
        return this->ds345->getMinFrequency(getFunction());
    return -1;
}

double Generator::getMaxFrequency(const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getMaxFrequency(waveform);
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxFrequency(waveform);
    return -1;
}

double Generator::getMaxFrequencyCurrent() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getMaxFrequency(getFunction());
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxFrequency(getFunction());
    return -1;
}

double Generator::getStepFrequency(const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getStepFrequency(waveform);
    if (this->generatorModel == "DS345")
        return this->ds345->getStepFrequency(waveform);
    return -1;
}

double Generator::getStepFrequencyCurrent() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getStepFrequency(getFunction());
    if (this->generatorModel == "DS345")
        return this->ds345->getStepFrequency(getFunction());
    return -1;
}

double Generator::getDecimalsFrequency(const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getDecimalsFrequency(waveform);
    if (this->generatorModel == "DS345")
        return this->ds345->getDecimalsFrequency(waveform);
    return -1;

}

double Generator::getDecimalsFrequencyCurrent() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getDecimalsFrequency(getFunction());
    if (this->generatorModel == "DS345")
        return this->ds345->getDecimalsFrequency(getFunction());
    return -1;
}

bool Generator::isValidFrequency(const double &frequency, const QString &waveform) const
{
    QString current_waveform = waveform;
    if (waveform.isEmpty())
        current_waveform = getFunction();
    return (frequency >= getMinFrequency(current_waveform) &&
            frequency <= getMaxFrequency(current_waveform));
}

bool Generator::setFrequency(const double &frequency) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->setFrequency(frequency);
    if (this->generatorModel == "DS345")
        return this->ds345->setFrequency(frequency);
    return false;
}

double Generator::getFrequency() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getFrequency();
    if (this->generatorModel == "DS345")
        return this->ds345->getFrequency();
    return -1;
}

bool Generator::workWithFunction() const
{
    if (this->generatorModel == "DS335")
        return true;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

QString Generator::getDefaultFunction() const
{
    if (this->generatorModel == "DS335")
        return "SINE";
    if (this->generatorModel == "DS345")
        return "SINE";
    return "";
}

bool Generator::setDefaultFunction() const
{
    return setFunction(getDefaultFunction());
}

QStringList Generator::getFunctionList() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getFunctionList();
    if (this->generatorModel == "DS345")
        return this->ds345->getFunctionList();
    return QStringList();
}

int Generator::functionNumberFromString(const QString &function_string) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->functionNumberFromString(function_string);
    if (this->generatorModel == "DS345")
        return this->ds345->functionNumberFromString(function_string);
    return -1;
}

QString Generator::functionStringFromNumber(const int &function_number) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->functionStringFromNumber(function_number);
    if (this->generatorModel == "DS345")
        return this->ds345->functionStringFromNumber(function_number);
    return "";
}

bool Generator::setFunction(const int &function) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->setFunction(function);
    if (this->generatorModel == "DS345")
        return this->ds345->setFunction(function);
    return false;
}

bool Generator::setFunction(const QString &function) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->setFunction(function);
    if (this->generatorModel == "DS345")
        return this->ds345->setFunction(function);
    return false;
}

QString Generator::getFunction() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getFunction();
    if (this->generatorModel == "DS345")
        return this->ds345->getFunction();
    return "";
}

bool Generator::workWithInverse() const
{
    if (this->generatorModel == "DS335")
        return true;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

bool Generator::getDefaultInverse() const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return false;
    return false;
}

bool Generator::setDefaultInverse() const
{
    return setInverse(getDefaultInverse());
}

bool Generator::setInverse(const bool &inverse) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->setInverse(inverse);
    if (this->generatorModel == "DS345")
        return this->ds345->setInverse(inverse);
    return false;
}

bool Generator::getInverse() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getInverse();
    if (this->generatorModel == "DS345")
        return this->ds345->getInverse();
    return false;
}

bool Generator::workWithSynchronization() const
{
    if (this->generatorModel == "DS335")
        return true;
    if (this->generatorModel == "DS345")
        return false;
    return false;
}

bool Generator::getDefaultSynchronization() const
{
    if (this->generatorModel == "DS335")
        return true;
    if (this->generatorModel == "DS345")
        return false;
    return false;
}

bool Generator::setDefaultSynchronization() const
{
    return setSynchronization(getDefaultSynchronization());
}

bool Generator::setSynchronization(const bool &synchronization) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->setSynchronization(synchronization);
    if (this->generatorModel == "DS345")
        return false;
    return false;
}

bool Generator::getSynchronization() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getSynchronization();
    if (this->generatorModel == "DS345")
        return false;
    return false;
}

bool Generator::workWithOutputImpedance() const
{
    if (this->generatorModel == "DS335")
        return true;
    if (this->generatorModel == "DS345")
        return false;
    return false;
}

QString Generator::getDefaultOutputImpedance() const
{
    if (this->generatorModel == "DS335")
        return "Hi-Z";
    if (this->generatorModel == "DS345")
        return "";
    return "";
}

bool Generator::setDefaultOutputImpedance() const
{
    return setOutputImpedance(getDefaultOutputImpedance());
}

QStringList Generator::getOutputImpedanceList() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getOutputImpedanceList();
    if (this->generatorModel == "DS345")
        return QStringList();
    return QStringList();
}

int Generator::outputImpedanceNumberFromString(const QString &outputImpedance_string) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->outputImpedanceNumberFromString(outputImpedance_string);
    if (this->generatorModel == "DS345")
        return -1;
    return -1;
}

QString Generator::outputImpedanceStringFromNumber(const int &outputImpedance_number) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->outputImpedanceStringFromNumber(outputImpedance_number);
    if (this->generatorModel == "DS345")
        return "";
    return "";
}

bool Generator::setOutputImpedance(const int &outputImpedance) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->setOutputImpedance(outputImpedance);
    if (this->generatorModel == "DS345")
        return false;
    return false;
}

bool Generator::setOutputImpedance(const QString &outputImpedance) const
{
    if (this->generatorModel == "DS335")
        return this->ds335->setOutputImpedance(outputImpedance);
    if (this->generatorModel == "DS345")
        return false;
    return false;
}

QString Generator::getOutputImpedance() const
{
    if (this->generatorModel == "DS335")
        return this->ds335->getOutputImpedance();
    if (this->generatorModel == "DS345")
        return "";
    return "";
}

bool Generator::workWithAMDepth() const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

int Generator::getDefaultAMDepth() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return 0;
    return -1;
}

bool Generator::setDefaultAMDepth() const
{
    return setAMDepth(getDefaultAMDepth());
}

int Generator::getMinAMDepth() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMinAMDepth();
    return -1;
}

int Generator::getMaxAMDepth() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxAMDepth();
    return -1;
}

bool Generator::isValidAMDepth(const int &persentage) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->isValidAMDepth(persentage);
    return false;
}

bool Generator::setAMDepth(const int &persentage) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->setAMdepth(persentage);
    return false;
}

int Generator::getAMDepth() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getAMdepth();
    return -1;
}

bool Generator::workWithFMSpan() const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

double Generator::getDefaultFMSpan() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMinFMSpan(getDefaultFunction());
    return -1;
}

bool Generator::setDefaultFMSpan() const
{
    return setFMSpan(getDefaultFMSpan());
}

double Generator::getMinFMSpan(const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMinFMSpan(waveform);
    return -1;
}

double Generator::getMinFMSpanCurrent() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMinFMSpan(getFunction());
    return -1;
}

double Generator::getMaxFMSpan(const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxFMSpan(waveform);
    return -1;
}

double Generator::getMaxFMSpanCurrent() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxFMSpan(getFunction());
    return -1;
}

bool Generator::isValidFMSpan(const int &span, const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->isValidFMSpan(span, waveform);
    return false;
}

bool Generator::setFMSpan(const double &span) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->setFMSpan(span);
    return false;
}

double Generator::getFMSpan() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getFMSpan();
    return -1;
}

bool Generator::workWithModulationFunction() const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

QString Generator::getDefaultModulationFunction() const
{
    if (this->generatorModel == "DS335")
        return "";
    if (this->generatorModel == "DS345")
        return "SINE";
    return "";
}

bool Generator::setDefaultModulationFunction() const
{
    return setModulationFunction(getDefaultModulationFunction());
}

QStringList Generator::getModulationFunctionList() const
{
    if (this->generatorModel == "DS335")
        return QStringList();
    if (this->generatorModel == "DS345")
        return this->ds345->getModulationFunctionList();
    return QStringList();
}

int Generator::modulationFunctionNumberFromString(const QString &function_string) const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->modulationFunctionNumberFromString(function_string);
    return -1;
}

QString Generator::modulationFunctionStringFromNumber(const int &function_number) const
{
    if (this->generatorModel == "DS335")
        return "";
    if (this->generatorModel == "DS345")
        return this->ds345->modulationFunctionStringFromNumber(function_number);
    return "";
}

bool Generator::setModulationFunction(const int &function) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->setModulationFunction(function);
    return false;
}

bool Generator::setModulationFunction(const QString &function) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->setModulationFunction(function);
    return false;
}

QString Generator::getModulationFunction() const
{
    if (this->generatorModel == "DS335")
        return "";
    if (this->generatorModel == "DS345")
        return this->ds345->getModulationFunction();
    return "";
}

bool Generator::workWithModulationEnabled() const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

bool Generator::getDefaultModulationEnabled() const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return false;
    return false;
}

bool Generator::setDefaultModulationEnabled() const
{
    return setModulationEnabled(getDefaultModulationEnabled());
}

bool Generator::setModulationEnabled(const bool &enable) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->setModulationEnabled(enable);
    return false;
}

bool Generator::getModulationEnabled() const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->getModulationEnabled();
    return false;
}

bool Generator::workWithModulationType() const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

QString Generator::getDefaultModulationType() const
{
    if (this->generatorModel == "DS335")
        return "";
    if (this->generatorModel == "DS345")
        return "INTERNAL AM";
    return "";
}

bool Generator::setDefaultModulationType() const
{
    return setModulationType(getDefaultModulationType());
}

QStringList Generator::getModulationTypeList() const
{
    if (this->generatorModel == "DS335")
        return QStringList();
    if (this->generatorModel == "DS345")
        return this->ds345->getModulationTypeList();
    return QStringList();
}

int Generator::modulationTypeNumberFromString(const QString &type_string) const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->modulationTypeNumberFromString(type_string);
    return -1;
}

QString Generator::modulationTypeStringFromNumber(const int &type_number) const
{
    if (this->generatorModel == "DS335")
        return "";
    if (this->generatorModel == "DS345")
        return this->ds345->modulationTypeStringFromNumber(type_number);
    return "";
}

bool Generator::setModulationType(const int &type) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->setModulationType(type);
    return false;
}

bool Generator::setModulationType(const QString &type) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->setModulationType(type);
    return false;
}

QString Generator::getModulationType() const
{
    if (this->generatorModel == "DS335")
        return "";
    if (this->generatorModel == "DS345")
        return this->ds345->getModulationType();
    return "";
}

bool Generator::workWithModulationRate() const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

double Generator::getDefaultModulationRate() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMinModulationRate();
    return -1;
}

bool Generator::setDefaultModulationRate() const
{
    return setModulationRate(getDefaultModulationRate());
}

double Generator::getMinModulationRate() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMinModulationRate();
    return -1;
}

double Generator::getMaxModulationRate() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxModulationRate();
    return -1;
}

double Generator::getStepModulationRate() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getStepModulationRate();
    return -1;
}

double Generator::getDecimalsModulationRate() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getDecimalsModulationRate();
    return -1;
}

bool Generator::isValidModulationRate(const double &rate) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->isValidModulationRate(rate);
    return false;
}

bool Generator::setModulationRate(const double &rate) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->setModulationRate(rate);
    return false;
}

double Generator::getModulationRate() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getModulationRate();
    return -1;
}

bool Generator::workWithModulationSpan() const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return true;
    return false;
}

double Generator::getDefaultModulationSpan() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMinModulationSpan(getDefaultFunction());
    return -1;
}

bool Generator::setDefaultModulationSpan() const
{
    return setModulationSpan(getDefaultModulationSpan());
}

double Generator::getMinModulationSpan(const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMinModulationSpan(waveform);
    return -1;
}

double Generator::getMinModulationSpanCurrent() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMinModulationSpan(getFunction());
    return -1;
}

double Generator::getMaxModulationSpan(const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxModulationSpan(waveform);
    return -1;
}

double Generator::getMaxModulationSpanCurrent() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getMaxModulationSpan(getFunction());
    return -1;
}

double Generator::getStepModulationSpan(const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getStepModulationSpan(waveform);
    return -1;
}

double Generator::getStepModulationSpanCurrent() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getStepModulationSpan(getFunction());
    return -1;
}

double Generator::getDecimalsModulationSpan(const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getDecimalsModulationSpan(waveform);
    return -1;
}

double Generator::getDecimalsModulationSpanCurrent() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getDecimalsModulationSpan(getFunction());
    return -1;
}

bool Generator::isValidModulationSpan(const double &span, const QString &waveform) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->isValidModulationSpan(span, waveform);
    return false;
}

bool Generator::setModulationSpan(const double &span) const
{
    if (this->generatorModel == "DS335")
        return false;
    if (this->generatorModel == "DS345")
        return this->ds345->setModulationSpan(span);
    return false;
}

double Generator::getModulationSpan() const
{
    if (this->generatorModel == "DS335")
        return -1;
    if (this->generatorModel == "DS345")
        return this->ds345->getModulationSpan();
    return -1;
}

void Generator::catchErrorSignal(const QString &s) const
{
    emit this->errorSignal(s);
}

void Generator::catchResponseSignal(const QString &s) const
{
    emit this->responseSignal(s);
}

void Generator::catchCommandSignal(const QString &s) const
{
    emit this->commandSignal(s);
}

void Generator::catchTimeoutSignal(const QString &s) const
{
    emit this->timeoutSignal(s);
}
