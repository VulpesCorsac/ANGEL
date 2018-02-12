#include "DS345.h"

DS345::DS345()
{
    this->serial = nullptr;
    this->srs = nullptr;

    init();
}

DS345::DS345(const QString &portName, const int &new_baudrate)
{
    this->serial = nullptr;
    this->srs = nullptr;

    init();

    setConnection(portName, new_baudrate);
}

DS345::~DS345()
{
    if (this->serial->isOpen())
        this->serial->close();
    this->serial->~QSerialPort();

    this->srs->~SRS();


}

void DS345::init()
{
    if (this->srs == nullptr)
        this->srs = new SRS();

    initBaudrateList();
    initAmplitudeTypeList();
    initFunctionList();
    initModulationFunctionList();
    initModulationTypeList();

    connect(this->srs, SIGNAL(errorSignal(QString)),    this, SLOT(catchErrorSignal(QString))   , Qt::DirectConnection);
    connect(this->srs, SIGNAL(responseSignal(QString)), this, SLOT(catchResponseSignal(QString)), Qt::DirectConnection);
    connect(this->srs, SIGNAL(commandSignal(QString)),  this, SLOT(catchCommandSignal(QString)) , Qt::DirectConnection);
    connect(this->srs, SIGNAL(timeoutSignal(QString)),  this, SLOT(catchTimeoutSignal(QString)) , Qt::DirectConnection);

    return;
}

void DS345::setSerial(QSerialPort *new_serial)
{
    return this->srs->setSerial(&this->serial, new_serial);
}

QSerialPort* DS345::getSerial() const
{
    return this->srs->getSerial(this->serial);
}

bool DS345::setConnection(const QString &portName, const int &new_baudrate)
{
    return this->srs->setConnection(&this->serial, portName, new_baudrate);
}

void DS345::disconnect() const
{
    return this->srs->disconnect(this->serial);
}

void DS345::initBaudrateList()
{
    this->baudrateList.clear();

    this->baudrateList.push_back("1200");
    this->baudrateList.push_back("2400");
    this->baudrateList.push_back("4800");
    this->baudrateList.push_back("9600");

    return;
}

QStringList DS345::getBaudrateList() const
{
    return this->baudrateList;
}

bool DS345::isValidBaudrate(const int &new_baudrate) const
{
    return isValidString(this->baudrateList, QString::number(new_baudrate));
}

bool DS345::changeBaudrate(const int &new_baudrate)
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

bool DS345::isActive() const
{
    return this->srs->isActive(this->serial);
}

bool DS345::send(const QString &command, QString &response, const bool &wait_for_response) const
{
    return this->srs->send(this->serial, command, response, wait_for_response);
}

bool DS345::sendCommand(const QString &command) const
{
    return this->srs->sendCommand(this->serial, command);
}

bool DS345::sendQuery(const QString &command, QString &response) const
{
    return this->srs->sendQuery(this->serial, command, response);
}

QString DS345::ask(const QString &command) const
{
    return this->srs->ask(this->serial, command);
}

bool DS345::getIDN(QString &idn) const
{
    return this->srs->getIDN(this->serial, idn);
}

int DS345::numberFromString(const QStringList &list, const QString &string) const
{
    return this->srs->numberFromString(list, string);
}

QString DS345::stringFromNumber(const QStringList &list, const int &number) const
{
    return this->srs->stringFromNumber(list, number);
}

bool DS345::isValidString(const QStringList &list, const QString &string) const
{
    return this->srs->isValidString(list, string);
}

bool DS345::isValidNumber(const QStringList &list, const int &number) const
{
    return this->srs->isValidNumber(list, number);
}

void DS345::initAmplitudeTypeList()
{
    this->amplitudeTypeList.clear();

    this->amplitudeTypeList.push_back("VP");
    this->amplitudeTypeList.push_back("VR");
    this->amplitudeTypeList.push_back("DB");

    return;
}

QStringList DS345::getAmplitudeTypeList() const
{
   return this->amplitudeTypeList;
}

double DS345::getMinAmplitude(const QString &waveform, const QString &unit) const
{
    if (waveform == "SINE") {
        if (unit == "VP")
            return this->minAmplitudeSineVpp;
        if (unit == "VR")
            return this->minAmplitudeSineVrms;
        if (unit == "DB")
            return this->minAmplitudeSinedBm;
    }
    if (waveform == "SQUARE") {
        if (unit == "VP")
            return this->minAmplitudeSquareVpp;
        if (unit == "VR")
            return this->minAmplitudeSquareVrms;
        if (unit == "DB")
            return this->minAmplitudeSquaredBm;
    }
    if (waveform == "TRIANGLE") {
        if (unit == "VP")
            return this->minAmplitudeTriangleVpp;
        if (unit == "VR")
            return this->minAmplitudeTriangleVrms;
        if (unit == "DB")
            return this->minAmplitudeTriangledBm;
    }
    if (waveform == "RAMP") {
        if (unit == "VP")
            return this->minAmplitudeRampVpp;
        if (unit == "VR")
            return this->minAmplitudeRampVrms;
        if (unit == "DB")
            return this->minAmplitudeRampdBm;
    }
    if (waveform == "NOISE") {
        if (unit == "VP")
            return this->minAmplitudeNoiseVpp;
        if (unit == "VR")
            return this->minAmplitudeNoiseVrms;
        if (unit == "DB")
            return this->minAmplitudeNoisedBm;
    }
    if (waveform == "ARBITRARY") {
        if (unit == "VP")
            return this->minAmplitudeArbitraryVpp;
        if (unit == "VR")
            return -1;
        if (unit == "DB")
            return -1;
    }

    return -1;
}

double DS345::getMaxAmplitude(const QString &waveform, const QString &unit) const
{
    if (waveform == "SINE") {
        if (unit == "VP")
            return this->maxAmplitudeSineVpp;
        if (unit == "VR")
            return this->maxAmplitudeSineVrms;
        if (unit == "DB")
            return this->maxAmplitudeSinedBm;
    }
    if (waveform == "SQUARE") {
        if (unit == "VP")
            return this->maxAmplitudeSquareVpp;
        if (unit == "VR")
            return this->maxAmplitudeSquareVrms;
        if (unit == "DB")
            return this->maxAmplitudeSquaredBm;
    }
    if (waveform == "TRIANGLE") {
        if (unit == "VP")
            return this->maxAmplitudeTriangleVpp;
        if (unit == "VR")
            return this->maxAmplitudeTriangleVrms;
        if (unit == "DB")
            return this->maxAmplitudeTriangledBm;
    }
    if (waveform == "RAMP") {
        if (unit == "VP")
            return this->maxAmplitudeRampVpp;
        if (unit == "VR")
            return this->maxAmplitudeRampVrms;
        if (unit == "DB")
            return this->maxAmplitudeRampdBm;
    }
    if (waveform == "NOISE") {
        if (unit == "VP")
            return this->maxAmplitudeNoiseVpp;
        if (unit == "VR")
            return this->maxAmplitudeNoiseVrms;
        if (unit == "DB")
            return this->maxAmplitudeNoisedBm;
    }
    if (waveform == "ARBITRARY") {
        if (unit == "VP")
            return this->maxAmplitudeArbitraryVpp;
        if (unit == "VR")
            return -1;
        if (unit == "DB")
            return -1;
    }

    return -1;
}

double DS345::getStepAmplitude(const QString &waveform, const QString &unit) const
{
    if (waveform == "SINE") {
        if (unit == "VP")
            return this->stepAmplitudeSineVpp;
        if (unit == "VR")
            return this->stepAmplitudeSineVrms;
        if (unit == "DB")
            return this->stepAmplitudeSinedBm;
    }
    if (waveform == "SQUARE") {
        if (unit == "VP")
            return this->stepAmplitudeSquareVpp;
        if (unit == "VR")
            return this->stepAmplitudeSquareVrms;
        if (unit == "DB")
            return this->stepAmplitudeSquaredBm;
    }
    if (waveform == "TRIANGLE") {
        if (unit == "VP")
            return this->stepAmplitudeTriangleVpp;
        if (unit == "VR")
            return this->stepAmplitudeTriangleVrms;
        if (unit == "DB")
            return this->stepAmplitudeTriangledBm;
    }
    if (waveform == "RAMP") {
        if (unit == "VP")
            return this->stepAmplitudeRampVpp;
        if (unit == "VR")
            return this->stepAmplitudeRampVrms;
        if (unit == "DB")
            return this->stepAmplitudeRampdBm;
    }
    if (waveform == "NOISE") {
        if (unit == "VP")
            return this->stepAmplitudeNoiseVpp;
        if (unit == "VR")
            return this->stepAmplitudeNoiseVrms;
        if (unit == "DB")
            return this->stepAmplitudeNoisedBm;
    }
    if (waveform == "ARBITRARY") {
        if (unit == "VP")
            return this->stepAmplitudeArbitraryVpp;
        if (unit == "VR")
            return -1;
        if (unit == "DB")
            return -1;
    }

    return -1;
}

double DS345::getDecimalsAmplitude(const QString &waveform, const QString &unit) const
{
    if (waveform == "SINE") {
        if (unit == "VP")
            return this->decimalsAmplitudeSineVpp;
        if (unit == "VR")
            return this->decimalsAmplitudeSineVrms;
        if (unit == "DB")
            return this->decimalsAmplitudeSinedBm;
    }
    if (waveform == "SQUARE") {
        if (unit == "VP")
            return this->decimalsAmplitudeSquareVpp;
        if (unit == "VR")
            return this->decimalsAmplitudeSquareVrms;
        if (unit == "DB")
            return this->decimalsAmplitudeSquaredBm;
    }
    if (waveform == "TRIANGLE") {
        if (unit == "VP")
            return this->decimalsAmplitudeTriangleVpp;
        if (unit == "VR")
            return this->decimalsAmplitudeTriangleVrms;
        if (unit == "DB")
            return this->decimalsAmplitudeTriangledBm;
    }
    if (waveform == "RAMP") {
        if (unit == "VP")
            return this->decimalsAmplitudeRampVpp;
        if (unit == "VR")
            return this->decimalsAmplitudeRampVrms;
        if (unit == "DB")
            return this->decimalsAmplitudeRampdBm;
    }
    if (waveform == "NOISE") {
        if (unit == "VP")
            return this->decimalsAmplitudeNoiseVpp;
        if (unit == "VR")
            return this->decimalsAmplitudeNoiseVrms;
        if (unit == "DB")
            return this->decimalsAmplitudeNoisedBm;
    }
    if (waveform == "ARBITRARY") {
        if (unit == "VP")
            return this->decimalsAmplitudeArbitraryVpp;
        if (unit == "VR")
            return -1;
        if (unit == "DB")
            return -1;
    }

    return -1;
}

bool DS345::isValidAmplitude(const double &new_amplitude, const QString &waveform, const QString &unit) const
{
    if (!isValidAmplitudeType(unit))
        return false;
    return (getMinAmplitude(waveform, unit) <= new_amplitude &&
            new_amplitude <= getMaxAmplitude(waveform, unit));
}

bool DS345::isValidAmplitudeType(const QString &unit) const
{
    return isValidString(this->amplitudeTypeList, unit);
}

bool DS345::setAmplitude(const double &new_amplitude, const QString &unit) const
{
    if (!isValidAmplitudeType(unit))
        return false;

    QString command = "AMPL " + QString::number(new_amplitude) + "" + unit;
    return sendCommand(command);
}

double DS345::getAmplitude(const QString &unit) const
{
    if (!isValidAmplitudeType(unit))
        return -1;

    QString response;
    QString command = "AMPL? " + unit;
    if (!sendQuery(command, response))
        return -1;
    else
        return response.trimmed().left(response.length() - 2).toDouble();
}

double DS345::getMinOffset() const
{
    return this->minOffsetV;
}

double DS345::getMaxOffset() const
{
    return this->maxOffsetV;
}

double DS345::getStepOffset() const
{
    return this->stepOffsetV;
}

double DS345::getDecimalsOffset() const
{
    return this->decimalsOffsetV;
}

bool DS345::isValidOffset(const double &offset) const
{
    return (this->minOffsetV <= offset && offset <= this->maxOffsetV);
}

bool DS345::setOffset(const double &new_offset) const
{
    if (!isValidOffset(new_offset))
        return false;

    QString command = "OFFS " + QString::number(new_offset);
    return sendCommand(command);
}

bool DS345::getOffset() const
{
    return ask("OFFS?").toDouble();
}

double DS345::getMinFrequency(const QString &waveform) const
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
    if (waveform == "ARBITRARY")
        return this->minFrequencyArbitrary;

    return -1;
}

double DS345::getMaxFrequency(const QString &waveform) const
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
    if (waveform == "ARBITRARY")
        return this->maxFrequencyArbitrary;

    return -1;
}

double DS345::getStepFrequency(const QString &waveform) const
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
    if (waveform == "ARBITRARY")
        return this->stepFrequencyArbitrary;

    return -1;
}

double DS345::getDecimalsFrequency(const QString &waveform) const
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
    if (waveform == "ARBITRARY")
        return this->decimalsFrequencyArbitrary;

    return -1;
}

bool DS345::setFrequency(const double &new_frequency) const
{
    QString command = "FREQ " + QString::number(new_frequency,'f',6);
    return sendCommand(command);
}

double DS345::getFrequency() const
{
    return ask("FREQ?").toDouble();
}

void DS345::initFunctionList()
{
    this->functionList.clear();

    this->functionList.push_back("SINE");
    this->functionList.push_back("SQUARE");
    this->functionList.push_back("TRIANGLE");
    this->functionList.push_back("RAMP");
    this->functionList.push_back("NOISE");
    this->functionList.push_back("ARBITRARY");

    return;
}

QStringList DS345::getFunctionList() const
{
    return this->functionList;
}

int DS345::functionNumberFromString(const QString &function_string) const
{
    return numberFromString(this->functionList, function_string);
}

QString DS345::functionStringFromNumber(const int &function_number) const
{
    return stringFromNumber(this->functionList, function_number);
}

bool DS345::setFunction(const int &new_function) const
{
    if (!isValidNumber(this->functionList, new_function))
        return false;

    QString command = "FUNC " + QString::number(new_function);
    return sendCommand(command);
}

bool DS345::setFunction(const QString &new_function) const
{
    return setFunction(functionNumberFromString(new_function));
}

QString DS345::getFunction() const
{
    return functionStringFromNumber(ask("FUNC?").toInt());
}

bool DS345::setInverse(const bool &new_inverse) const
{
    QString command = "INVT ";
    if (new_inverse)
        command += "1";
    else
        command += "0";
    return sendCommand(command);
}

bool DS345::getInverse() const
{
    return (ask("INVT?").toInt() == 1);
}

int DS345::getMinAMDepth() const
{
    return this->minAMDepth;
}

int DS345::getMaxAMDepth() const
{
    return this->maxAMDepth;
}

bool DS345::isValidAMDepth(const int &persentage) const
{
    return (this->minAMDepth <= persentage && persentage <= this->maxAMDepth);
}

bool DS345::setAMdepth(const int &new_persentage) const
{
    if (!isValidAMDepth(new_persentage))
        return false;

    QString command = "DPTH " + QString::number(new_persentage);
    return sendCommand(command);
}

int DS345::getAMdepth() const
{
    return ask("DPTH?").toInt();
}

double DS345::getMinFMSpan(const QString &waveform) const
{
    return getMinFrequency(waveform);
}

double DS345::getMaxFMSpan(const QString &waveform) const
{
    return getMaxFrequency(waveform);
}

bool DS345::isValidFMSpan(const int &span, const QString &waveform) const
{
    return (getMinFMSpan(waveform) <= span && span <= getMaxFMSpan(waveform));
}

bool DS345::setFMSpan(const double &new_span) const
{
    QString command = "FDEV " + QString::number(new_span);
    return sendCommand(command);
}

double DS345::getFMSpan() const
{
    return ask("FDEV?").toDouble();
}

void DS345::initModulationFunctionList()
{
    this->modulationFunctionList.clear();

    this->modulationFunctionList.push_back("SINGLE SWEEP");
    this->modulationFunctionList.push_back("RAMP");
    this->modulationFunctionList.push_back("TRIANGLE");
    this->modulationFunctionList.push_back("SINE");
    this->modulationFunctionList.push_back("SQUARE");
    this->modulationFunctionList.push_back("ARB");
    this->modulationFunctionList.push_back("NONE");

    return;
}

QStringList DS345::getModulationFunctionList() const
{
    return this->modulationFunctionList;
}

int DS345::modulationFunctionNumberFromString(const QString &function_string) const
{
    return numberFromString(this->modulationFunctionList, function_string);
}

QString DS345::modulationFunctionStringFromNumber(const int &function_number) const
{
    return stringFromNumber(this->modulationFunctionList, function_number);
}

bool DS345::setModulationFunction(const int &new_function) const
{
    if (!isValidNumber(this->modulationFunctionList, new_function))
        return false;

    QString command = "MDWF " + QString::number(new_function);
    return sendCommand(command);
}

bool DS345::setModulationFunction(const QString &new_function) const
{
    return setModulationFunction(modulationFunctionNumberFromString(new_function));
}

QString DS345::getModulationFunction() const
{
    return modulationFunctionStringFromNumber(ask("MDWF?").toInt());
}

bool DS345::setModulationEnabled(const bool &enable) const
{
    QString command = "MENA ";
    if (enable)
        command += "1";
    else
        command += "0";
    return sendCommand(command);
}

bool DS345::getModulationEnabled() const
{
    return (ask("MENA?").toInt() == 1);
}

void DS345::initModulationTypeList()
{
    this->modulationTypeList.clear();

    this->modulationTypeList.push_back("LIN SWEEP");
    this->modulationTypeList.push_back("LOG SWEEP");
    this->modulationTypeList.push_back("INTERNAL AM");
    this->modulationTypeList.push_back("FM");
    this->modulationTypeList.push_back("FiM");
    this->modulationTypeList.push_back("BURST");

    return;
}

QStringList DS345::getModulationTypeList() const
{
    return this->modulationTypeList;
}

int DS345::modulationTypeNumberFromString(const QString &type_string) const
{
    return numberFromString(this->modulationTypeList, type_string);
}

QString DS345::modulationTypeStringFromNumber(const int &type_number) const
{
    return stringFromNumber(this->modulationTypeList, type_number);
}

bool DS345::setModulationType(const int &new_type) const
{
    if (!isValidNumber(this->modulationTypeList, new_type))
        return false;

    QString command = "MTYP " + QString::number(new_type);
    return sendCommand(command);
}

bool DS345::setModulationType(const QString &new_type) const
{
    return setModulationType(modulationTypeNumberFromString(new_type));
}

QString DS345::getModulationType() const
{
    return modulationTypeStringFromNumber(ask("MTYP?").toInt());
}

double DS345::getMinModulationRate() const
{
    return this->minModulationRate;
}

double DS345::getMaxModulationRate() const
{
    return this->maxModulationRate;
}

double DS345::getStepModulationRate() const
{
    return this->stepModulationRate;
}

double DS345::getDecimalsModulationRate() const
{
    return this->decimalsModulationRate;
}

bool DS345::isValidModulationRate(const double &rate) const
{
    return (this->minModulationRate <= rate && rate <= this->maxModulationRate);
}

bool DS345::setModulationRate(const double &new_rate) const
{
    if (!isValidModulationRate(new_rate))
        return false;

    QString command = "RATE " + QString::number(new_rate);
    return sendCommand(command);

}

double DS345::getModulationRate() const
{
    return ask("RATE?").toDouble();
}

double DS345::getMinModulationSpan(const QString &waveform) const
{
    return getMinFrequency(waveform);
}

double DS345::getMaxModulationSpan(const QString &waveform) const
{
    return getMaxFrequency(waveform);
}

double DS345::getStepModulationSpan(const QString &waveform) const
{
    return getStepFrequency(waveform);
}

double DS345::getDecimalsModulationSpan(const QString &waveform) const
{
    return getDecimalsFrequency(waveform);
}

bool DS345::isValidModulationSpan(const double &span, const QString &waveform) const
{
    return (getMinModulationSpan(waveform) <= span && span <= getMaxModulationSpan(waveform));
}

bool DS345::setModulationSpan(const double &new_span) const
{
    QString command = "SPAN " + QString::number(new_span);
    return sendCommand(command);
}

double DS345::getModulationSpan() const
{
    return ask("SPAN?").toDouble();
}

void DS345::catchErrorSignal(const QString &s) const
{
    emit this->errorSignal(s);
}

void DS345::catchResponseSignal(const QString &s) const
{
    emit this->responseSignal(s);
}

void DS345::catchCommandSignal(const QString &s) const
{
    emit this->commandSignal(s);
}

void DS345::catchTimeoutSignal(const QString &s) const
{
    emit this->timeoutSignal(s);
}
