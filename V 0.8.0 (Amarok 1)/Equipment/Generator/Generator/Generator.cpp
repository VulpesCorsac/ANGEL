#include "Generator.h"

Generator::Generator(QObject *parent) : QObject(parent)
{
    toNULL();
}

Generator::Generator(const QString &portName, const int &new_baudrate, const QString &new_generatorType)
{
    toNULL();
    setConnection(portName, new_baudrate, new_generatorType);
}

Generator::Generator(QSerialPort *serial, const QString &new_generatorType)
{
    toNULL();
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
    this->generatorType = "";
    if (this->ds335 != nullptr)
        this->ds335->~DS335();
    if (this->ds345 != nullptr)
        this->ds345->~DS345();
    this->ds335 = nullptr;
    this->ds345 = nullptr;
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

    current_state.model = this->generatorType;

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

bool Generator::setState(const Generator_State &new_state)
{
    bool result = true;

    if (new_state.model != this->generatorType)
        result = false;
    if (workWithAmplitude())
        result &= this->setAmplitude(new_state.amplitude, "VR");
    if (workWithOffset())
        result &= this->setOffset(new_state.offset);
    if (workWithFrequency())
        result &= this->setFrequency(new_state.frequency);
    if (workWithFunction())
        result &= this->setFunction(new_state.function);
    if (workWithInverse())
        result &= this->setInverse(new_state.inverse);
    if (workWithSynchronization())
        result &= this->setSynchronization(new_state.synchronization);
    if (workWithOutputImpedance())
        result &= this->setOutputImpedance(new_state.outputImpedance);
    if (workWithAMDepth())
        result &= this->setAMDepth(new_state.amDepth);
    if (workWithFMSpan())
        result &= this->setFMSpan(new_state.fmSpan);
    if (workWithModulationFunction())
        result &= this->setModulationFunction(new_state.modulationFunction);
    if (workWithModulationEnabled())
        result &= this->setModulationEnabled(new_state.modulationEnabled);
    if (workWithModulationType())
        result &= this->setModulationType(new_state.modulationType);
    if (workWithModulationRate())
        result &= this->setModulationRate(new_state.modulationRate);
    if (workWithModulationSpan())
        result &= this->setModulationSpan(new_state.modulationSpan);

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

QStringList Generator::getSupportedTypes() const
{
    QStringList ans;

    ans.push_back("DS335");
    ans.push_back("DS345");

    ans.push_back("AUTO");

    return ans;
}

void Generator::setGeneratorType(const QString &new_generatorType)
{
    this->generatorType = new_generatorType;

    return;
}

QString Generator::getGeneratorType() const
{
    return this->generatorType;
}

bool Generator::autoSetGeneratorType(QSerialPort *serial)
{
    this->generatorType = "";

    QString type = this->srs->autoDetect(serial);

    if (type.trimmed().isEmpty()) {
        this->srs->disconnect(serial);

        return false;
    }

    this->generatorType = type;
    if (type == "DS335"){
        this->ds335 = new DS335();
        this->ds335->setSerial(serial);
        init();

        return true;
    }
    if (type == "DS345"){
        this->ds345 = new DS345();
        this->ds345->setSerial(serial);
        init();

        return true;
    }

    this->srs->disconnect(serial);

    return false;
}

bool Generator::autoSetGeneratorType(const QString &portName, const int &new_baudrate)
{
    QSerialPort *serial = new QSerialPort();

    serial->setPortName(portName);
    serial->setBaudRate(new_baudrate);

    serial->setParity(QSerialPort::NoParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop); // TOCHANGE

    return autoSetGeneratorType(serial);
}

bool Generator::autoSetGeneratorType(const QString &portName)
{
    QStringList baudrates = this->srs->getBaudrateList();
    for (int baud = 0; baud < baudrates.size(); baud++) {
        int baudrate = baudrates[baud].toInt();

        if (autoSetGeneratorType(portName, baudrate))
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
        init();

        return this->ds335->setConnection(portName, new_baudrate);
    }
    if (new_generatorType.trimmed().toUpper() == "DS345") {
        if (this->ds345 == nullptr)
            this->ds345 = new DS345();
        this->generatorType = "DS345";
        init();

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

QStringList Generator::getBaudrateList() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getBaudrateList();
    if (this->generatorType == "DS345")
        return this->ds345->getBaudrateList();
    return QStringList();
}

bool Generator::isValidBaudrateAll(const int &new_baudrate) const
{
    DS335 temp335;
    DS345 temp345;
    bool answer = temp335.isValidBaudrate(new_baudrate);
    answer &= temp345.isValidBaudrate(new_baudrate);
    return answer;
}

bool Generator::isValidBaudrate(const int &new_baudrate) const
{
    if (this->generatorType == "DS335")
        return this->ds335->isValidBaudrate(new_baudrate);
    if (this->generatorType == "DS345")
        return this->ds345->isValidBaudrate(new_baudrate);
    return false;
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

QString Generator::ask(const QString &command) const
{
    if (this->generatorType == "DS335")
        return this->ds335->ask(command);
    if (this->generatorType == "DS345")
        return this->ds345->ask(command);
    return "";
}

bool Generator::getIDN(QString &idn) const
{
    if (this->generatorType == "DS335")
        return this->ds335->getIDN(idn);
    if (this->generatorType == "DS345")
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
        for (int i = 1; i >= 0; i--) {
            if (i == 0)
                qDebug() << "Generator inverse disbling";
            else
                qDebug() << "Generator inverse enabling";

            if (!setInverse(i)) {
                qDebug() << "Generator inverse comand failed";

                ans = false;
            }

            wait();

            if (getInverse() == i)
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
        for (int i = 0; i <= 1; i++) {
            if (i == 0)
                qDebug() << "Generator synchronization disbling";
            else
                qDebug() << "Generator synchronization enabling";

            if (!setSynchronization(i)) {
                qDebug() << "Generator synchronization comand failed";

                ans = false;
            }

            wait();

            if (getSynchronization() == i)
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
        for (int i = 1; i >= 0; i--) {
            if (i == 0)
                qDebug() << "Generator modulation disbling";
            else
                qDebug() << "Generator modulation enabling";

            if (!setModulationEnabled(i)) {
                qDebug() << "Generator modulation comand failed";

                ans = false;
            }

            wait();

            if (getModulationEnabled() == i)
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
    if (this->generatorType == "DS335")
        return 1;
    if (this->generatorType == "DS345")
        return 1;
    return 1E5;
}

double Generator::getAverageOutputTime() const
{
    if (this->generatorType == "DS335")
        return 30;
    if (this->generatorType == "DS345")
        return 30;
    return 1E5;
}

bool Generator::workWithAmplitude() const
{
    if (this->generatorType == "DS335")
        return true;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

double Generator::getDefaultAmplitude() const
{
    if (this->generatorType == "DS335")
        return 1;
    if (this->generatorType == "DS345")
        return 1;
    return -1;
}

bool Generator::setDefaultAmplitude() const
{
    return setAmplitude(getDefaultAmplitude(), "VR");
}

QStringList Generator::getAmplitudeTypeList() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getAmplitudeTypeList();
    if (this->generatorType == "DS345")
        return this->ds345->getAmplitudeTypeList();
    return QStringList();
}

double Generator::getMinAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (this->generatorType == "DS335")
        return this->ds335->getMinAmplitude(waveform, outputZ, VRMS);
    if (this->generatorType == "DS345") {
        if (VRMS)
            return this->ds345->getMinAmplitude(waveform, "VR");
        else
            return this->ds345->getMinAmplitude(waveform, "VP");
    }
    return -1;
}

double Generator::getMinAmplitude(const QString &waveform, const QString &unit) const
{
    if (this->generatorType == "DS335") {
        if (unit == "VR")
            return this->ds335->getMinAmplitude(waveform, getDefaultOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getMinAmplitude(waveform, getDefaultOutputImpedance(), true);
        else
            return -1;
    }
    if (this->generatorType == "DS345")
        return this->ds345->getMinAmplitude(waveform, unit);
    return -1;
}

double Generator::getMaxAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (this->generatorType == "DS335")
        return this->ds335->getMaxAmplitude(waveform, outputZ, VRMS);
    if (this->generatorType == "DS345") {
        if (VRMS)
            return this->ds345->getMaxAmplitude(waveform, "VR");
        else
            return this->ds345->getMaxAmplitude(waveform, "VP");
    }
    return -1;
}

double Generator::getMaxAmplitude(const QString &waveform, const QString &unit) const
{
    if (this->generatorType == "DS335") {
        if (unit == "VR")
            return this->ds335->getMaxAmplitude(waveform, getDefaultOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getMaxAmplitude(waveform, getDefaultOutputImpedance(), true);
        else
            return -1;
    }
    if (this->generatorType == "DS345")
        return this->ds345->getMaxAmplitude(waveform, unit);
    return -1;
}

double Generator::getStepAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (this->generatorType == "DS335")
        return this->ds335->getStepAmplitude(waveform, outputZ, VRMS);
    if (this->generatorType == "DS345") {
        if (VRMS)
            return this->ds345->getStepAmplitude(waveform, "VR");
        else
            return this->ds345->getStepAmplitude(waveform, "VP");
    }
    return -1;
}

double Generator::getStepAmplitude(const QString &waveform, const QString &unit) const
{
    if (this->generatorType == "DS335") {
        if (unit == "VR")
            return this->ds335->getStepAmplitude(waveform, getDefaultOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getStepAmplitude(waveform, getDefaultOutputImpedance(), true);
        else
            return -1;
    }
    if (this->generatorType == "DS345")
        return this->ds345->getStepAmplitude(waveform, unit);
    return -1;
}

double Generator::getDecimalsAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (this->generatorType == "DS335")
        return this->ds335->getDecimalsAmplitude(waveform, outputZ, VRMS);
    if (this->generatorType == "DS345") {
        if (VRMS)
            return this->ds345->getDecimalsAmplitude(waveform, "VR");
        else
            return this->ds345->getDecimalsAmplitude(waveform, "VP");
    }
    return -1;
}

double Generator::getDecimalsAmplitude(const QString &waveform, const QString &unit) const
{
    if (this->generatorType == "DS335") {
        if (unit == "VR")
            return this->ds335->getDecimalsAmplitude(waveform, getDefaultOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->getDecimalsAmplitude(waveform, getDefaultOutputImpedance(), true);
        else
            return -1;
    }
    if (this->generatorType == "DS345")
        return this->ds345->getDecimalsAmplitude(waveform, unit);
    return -1;
}

bool Generator::isValidAmplitude(const double &new_amplitude, const QString &waveform, const QString &outputZ, const bool &VRMS) const
{
    if (this->generatorType == "DS335")
        return this->ds335->isValidAmplitude(new_amplitude, waveform, outputZ, VRMS);
    if (this->generatorType == "DS345") {
        if (VRMS)
            return this->ds345->isValidAmplitude(new_amplitude, waveform, "VR");
        else
            return this->ds345->isValidAmplitude(new_amplitude, waveform, "VP");
    }
    return false;
}

bool Generator::isValidAmplitude(const double &new_amplitude, const QString &waveform, const QString &unit) const
{
    if (this->generatorType == "DS335") {
        if (unit == "VR")
            return this->ds335->isValidAmplitude(new_amplitude, waveform, getDefaultOutputImpedance(), true);
        if (unit == "VP")
            return this->ds335->isValidAmplitude(new_amplitude, waveform, getDefaultOutputImpedance(), true);
        else
            return false;
    }
    if (this->generatorType == "DS345")
        return this->ds345->isValidAmplitude(new_amplitude, waveform, unit);
    return false;
}

bool Generator::isValidAmplitudeType(const QString &unit) const
{
    if (this->generatorType == "DS335") {
        if (unit == "VR" || unit == "VP")
            return true;
        else
            return false;
    }
    if (this->generatorType == "DS345")
        return this->ds345->isValidAmplitudeType(unit);
    return false;
}

bool Generator::setAmplitude(const double &new_amplitude, const QString &unit) const
{
    if (this->generatorType == "DS335") {
        if (unit == "VR")
            return this->ds335->setAmplitude(new_amplitude, true);
        return this->ds335->setAmplitude(new_amplitude, false);
    }
    if (this->generatorType == "DS345")
        return this->ds345->setAmplitude(new_amplitude, unit);
    return false;
}

double Generator::getAmplitude(const QString &unit) const
{
    if (this->generatorType == "DS335") {
        if (unit == "VR")
            return this->ds335->getAmplitude(true);
        return this->ds335->getAmplitude(false);
    }
    if (this->generatorType == "DS345")
        return this->ds345->getAmplitude(unit);
    return -1;
}

bool Generator::workWithOffset() const
{
    if (this->generatorType == "DS335")
        return true;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

double Generator::getDefaultOffset() const
{
    if (this->generatorType == "DS335")
        return 0;
    if (this->generatorType == "DS345")
        return 0;
    return -1;
}

bool Generator::setDefaultOffset() const
{
    return setOffset(getDefaultOffset());
}

double Generator::getMinOffset() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getMinOffset();
    if (this->generatorType == "DS345")
        return this->ds345->getMinOffset();
    return -1;
}

double Generator::getMaxOffset() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getMaxOffset();
    if (this->generatorType == "DS345")
        return this->ds345->getMaxOffset();
    return -1;
}

double Generator::getStepOffset() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getStepOffset();
    if (this->generatorType == "DS345")
        return this->ds345->getStepOffset();
    return -1;
}

double Generator::getDecimalsOffset() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getDecimalsOffset();
    if (this->generatorType == "DS345")
        return this->ds345->getDecimalsOffset();
    return -1;
}

bool Generator::isValidOffset(const double &offset) const
{
    if (this->generatorType == "DS335")
        return this->ds335->isValidOffset(offset);
    if (this->generatorType == "DS345")
        return this->ds345->isValidOffset(offset);
    return false;
}

bool Generator::setOffset(const double &new_offset) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setOffset(new_offset);
    if (this->generatorType == "DS345")
        return this->ds345->setOffset(new_offset);
    return false;
}

bool Generator::getOffset() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getOffset();
    if (this->generatorType == "DS345")
        return this->ds345->getOffset();
    return false;

}

bool Generator::workWithFrequency() const
{
    if (this->generatorType == "DS335")
        return true;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

double Generator::getDefaultFrequency() const
{
    if (this->generatorType == "DS335")
//        return this->ds335->getMinFrequency(getDefaultFunction());
        return 1E4;
    if (this->generatorType == "DS345")
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
    if (this->generatorType == "DS335")
        return this->ds335->getMinFrequency(waveform);
    if (this->generatorType == "DS345")
        return this->ds345->getMinFrequency(waveform);
    return -1;
}

double Generator::getMaxFrequency(const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return this->ds335->getMaxFrequency(waveform);
    if (this->generatorType == "DS345")
        return this->ds345->getMaxFrequency(waveform);
    return -1;
}

double Generator::getStepFrequency(const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return this->ds335->getStepFrequency(waveform);
    if (this->generatorType == "DS345")
        return this->ds345->getStepFrequency(waveform);
    return -1;
}

double Generator::getDecimalsFrequency(const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return this->ds335->getDecimalsFrequency(waveform);
    if (this->generatorType == "DS345")
        return this->ds345->getDecimalsFrequency(waveform);
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

bool Generator::workWithFunction() const
{
    if (this->generatorType == "DS335")
        return true;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

QString Generator::getDefaultFunction() const
{
    if (this->generatorType == "DS335")
        return "SINE";
    if (this->generatorType == "DS345")
        return "SINE";
    return "";
}

bool Generator::setDefaultFunction() const
{
    return setFunction(getDefaultFunction());
}

QStringList Generator::getFunctionList() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getFunctionList();
    if (this->generatorType == "DS345")
        return this->ds345->getFunctionList();
    return QStringList();
}

int Generator::functionNumberFromString(const QString &function_string) const
{
    if (this->generatorType == "DS335")
        return this->ds335->functionNumberFromString(function_string);
    if (this->generatorType == "DS345")
        return this->ds345->functionNumberFromString(function_string);
    return -1;
}

QString Generator::functionStringFromNumber(const int &function_number) const
{
    if (this->generatorType == "DS335")
        return this->ds335->functionStringFromNumber(function_number);
    if (this->generatorType == "DS345")
        return this->ds345->functionStringFromNumber(function_number);
    return "";
}

bool Generator::setFunction(const int &new_function) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setFunction(new_function);
    if (this->generatorType == "DS345")
        return this->ds345->setFunction(new_function);
    return false;
}

bool Generator::setFunction(const QString &new_function) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setFunction(new_function);
    if (this->generatorType == "DS345")
        return this->ds345->setFunction(new_function);
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

bool Generator::workWithInverse() const
{
    if (this->generatorType == "DS335")
        return true;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

bool Generator::getDefaultInverse() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return false;
    return false;
}

bool Generator::setDefaultInverse() const
{
    return setInverse(getDefaultInverse());
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

bool Generator::workWithSynchronization() const
{
    if (this->generatorType == "DS335")
        return true;
    if (this->generatorType == "DS345")
        return false;
    return false;
}

bool Generator::getDefaultSynchronization() const
{
    if (this->generatorType == "DS335")
        return true;
    if (this->generatorType == "DS345")
        return false;
    return false;
}

bool Generator::setDefaultSynchronization() const
{
    return setSynchronization(getDefaultSynchronization());
}

bool Generator::setSynchronization(const bool &new_synchronization) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setSynchronization(new_synchronization);
    if (this->generatorType == "DS345")
        return false;
    return false;
}

bool Generator::getSynchronization() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getSynchronization();
    if (this->generatorType == "DS345")
        return false;
    return false;
}

bool Generator::workWithOutputImpedance() const
{
    if (this->generatorType == "DS335")
        return true;
    if (this->generatorType == "DS345")
        return false;
    return false;
}

QString Generator::getDefaultOutputImpedance() const
{
    if (this->generatorType == "DS335")
        return "Hi-Z";
    if (this->generatorType == "DS345")
        return "";
    return "";
}

bool Generator::setDefaultOutputImpedance() const
{
    return setOutputImpedance(getDefaultOutputImpedance());
}

QStringList Generator::getOutputImpedanceList() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getOutputImpedanceList();
    if (this->generatorType == "DS345")
        return QStringList();
    return QStringList();
}

int Generator::outputImpedanceNumberFromString(const QString &outputImpedance_string) const
{
    if (this->generatorType == "DS335")
        return this->ds335->outputImpedanceNumberFromString(outputImpedance_string);
    if (this->generatorType == "DS345")
        return -1;
    return -1;
}

QString Generator::outputImpedanceStringFromNumber(const int &outputImpedance_number) const
{
    if (this->generatorType == "DS335")
        return this->ds335->outputImpedanceStringFromNumber(outputImpedance_number);
    if (this->generatorType == "DS345")
        return "";
    return "";
}

bool Generator::setOutputImpedance(const int &new_outputImpedance) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setOutputImpedance(new_outputImpedance);
    if (this->generatorType == "DS345")
        return false;
    return false;
}

bool Generator::setOutputImpedance(const QString &new_outputImpedance) const
{
    if (this->generatorType == "DS335")
        return this->ds335->setOutputImpedance(new_outputImpedance);
    if (this->generatorType == "DS345")
        return false;
    return false;
}

QString Generator::getOutputImpedance() const
{
    if (this->generatorType == "DS335")
        return this->ds335->getOutputImpedance();
    if (this->generatorType == "DS345")
        return "";
    return "";
}

bool Generator::workWithAMDepth() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

int Generator::getDefaultAMDepth() const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return 0;
    return -1;
}

bool Generator::setDefaultAMDepth() const
{
    return setAMDepth(getDefaultAMDepth());
}

int Generator::getMinAMDepth() const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMinAMDepth();
    return -1;
}

int Generator::getMaxAMDepth() const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMaxAMDepth();
    return -1;
}

bool Generator::isValidAMDepth(const int &persentage) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->isValidAMDepth(persentage);
    return false;
}

bool Generator::setAMDepth(const int &new_persentage) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->setAMdepth(new_persentage);
    return false;
}

int Generator::getAMDepth() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->getAMdepth();
    return false;
}

bool Generator::workWithFMSpan() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

double Generator::getDefaultFMSpan() const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMinFMSpan(getDefaultFunction());
    return -1;
}

bool Generator::setDefaultFMSpan() const
{
    return setFMSpan(getDefaultFMSpan());
}

double Generator::getMinFMSpan(const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMinFMSpan(waveform);
    return -1;
}

double Generator::getMaxFMSpan(const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMaxFMSpan(waveform);
    return -1;
}

bool Generator::isValidFMSpan(const int &span, const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->isValidFMSpan(span, waveform);
    return false;
}

bool Generator::setFMSpan(const double &new_span) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->setFMSpan(new_span);
    return false;
}

double Generator::getFMSpan() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->getFMSpan();
    return false;
}

bool Generator::workWithModulationFunction() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

QString Generator::getDefaultModulationFunction() const
{
    if (this->generatorType == "DS335")
        return "";
    if (this->generatorType == "DS345")
        return "SINE";
    return "";
}

bool Generator::setDefaultModulationFunction() const
{
    return setModulationFunction(getDefaultModulationFunction());
}

QStringList Generator::getModulationFunctionList() const
{
    if (this->generatorType == "DS335")
        return QStringList();
    if (this->generatorType == "DS345")
        return this->ds345->getModulationFunctionList();
    return QStringList();
}

int Generator::modulationFunctionNumberFromString(const QString &function_string) const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->modulationFunctionNumberFromString(function_string);
    return -1;
}

QString Generator::modulationFunctionStringFromNumber(const int &function_number) const
{
    if (this->generatorType == "DS335")
        return "";
    if (this->generatorType == "DS345")
        return this->ds345->modulationFunctionStringFromNumber(function_number);
    return "";
}

bool Generator::setModulationFunction(const int &new_function) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->setModulationFunction(new_function);
    return false;
}

bool Generator::setModulationFunction(const QString &new_function) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->setModulationFunction(new_function);
    return false;
}

QString Generator::getModulationFunction() const
{
    if (this->generatorType == "DS335")
        return "";
    if (this->generatorType == "DS345")
        return this->ds345->getModulationFunction();
    return "";
}

bool Generator::workWithModulationEnabled() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

bool Generator::getDefaultModulationEnabled() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return false;
    return false;
}

bool Generator::setDefaultModulationEnabled() const
{
    return setModulationEnabled(getDefaultModulationEnabled());
}

bool Generator::setModulationEnabled(const bool &enable) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->setModulationEnabled(enable);
    return false;
}

bool Generator::getModulationEnabled() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->getModulationEnabled();
    return false;
}

bool Generator::workWithModulationType() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

QString Generator::getDefaultModulationType() const
{
    if (this->generatorType == "DS335")
        return "";
    if (this->generatorType == "DS345")
        return "INTERNAL AM";
    return "";
}

bool Generator::setDefaultModulationType() const
{
    return setModulationType(getDefaultModulationType());
}

QStringList Generator::getModulationTypeList() const
{
    if (this->generatorType == "DS335")
        return QStringList();
    if (this->generatorType == "DS345")
        return this->ds345->getModulationTypeList();
    return QStringList();
}

int Generator::modulationTypeNumberFromString(const QString &type_string) const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->modulationTypeNumberFromString(type_string);
    return -1;
}

QString Generator::modulationTypeStringFromNumber(const int &type_number) const
{
    if (this->generatorType == "DS335")
        return "";
    if (this->generatorType == "DS345")
        return this->ds345->modulationTypeStringFromNumber(type_number);
    return "";
}

bool Generator::setModulationType(const int &new_type) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->setModulationType(new_type);
    return false;
}

bool Generator::setModulationType(const QString &new_type) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->setModulationType(new_type);
    return false;
}

QString Generator::getModulationType() const
{
    if (this->generatorType == "DS335")
        return "";
    if (this->generatorType == "DS345")
        return this->ds345->getModulationType();
    return "";
}

bool Generator::workWithModulationRate() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

double Generator::getDefaultModulationRate() const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMinModulationRate();
    return -1;
}

bool Generator::setDefaultModulationRate() const
{
    return setModulationRate(getDefaultModulationRate());
}

double Generator::getMinModulationRate() const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMinModulationRate();
    return -1;
}

double Generator::getMaxModulationRate() const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMaxModulationRate();
    return -1;
}

double Generator::getStepModulationRate() const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getStepModulationRate();
    return -1;
}

double Generator::getDecimalsModulationRate() const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getDecimalsModulationRate();
    return -1;
}

bool Generator::isValidModulationRate(const double &rate) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->isValidModulationRate(rate);
    return false;
}

bool Generator::setModulationRate(const double &new_rate) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->setModulationRate(new_rate);
    return false;
}

double Generator::getModulationRate() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->getModulationRate();
    return false;
}

bool Generator::workWithModulationSpan() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return true;
    return false;
}

double Generator::getDefaultModulationSpan() const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMinModulationSpan(getDefaultFunction());
    return -1;
}

bool Generator::setDefaultModulationSpan() const
{
    return setModulationSpan(getDefaultModulationSpan());
}

double Generator::getMinModulationSpan(const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMinModulationSpan(waveform);
    return -1;
}

double Generator::getMaxModulationSpan(const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getMaxModulationSpan(waveform);
    return -1;
}

double Generator::getStepModulationSpan(const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getStepModulationSpan(waveform);
    return -1;
}

double Generator::getDecimalsModulationSpan(const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return -1;
    if (this->generatorType == "DS345")
        return this->ds345->getDecimalsModulationSpan(waveform);
    return -1;
}

bool Generator::isValidModulationSpan(const double &span, const QString &waveform) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->isValidModulationSpan(span, waveform);
    return false;
}

bool Generator::setModulationSpan(const double &new_span) const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->setModulationSpan(new_span);
    return false;
}

double Generator::getModulationSpan() const
{
    if (this->generatorType == "DS335")
        return false;
    if (this->generatorType == "DS345")
        return this->ds345->getModulationSpan();
    return false;
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
