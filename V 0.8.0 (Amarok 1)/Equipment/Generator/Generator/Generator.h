#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QDebug>

#include <QTime>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QTest>

#include "../../../Functions/AllFunctions.h"
#include "../../CommonFunctionsForSRS/SRS.h"
#include "../AllGenerators.h"

struct Generator_State
{
    QString model;
    double amplitude; // In Vrms
    double offset;
    double frequency;
    QString function;
    bool inverse;
    bool synchronization;
    QString outputImpedance;
    int amDepth;
    double fmSpan;
    QString modulationFunction;
    bool modulationEnabled;
    QString modulationType;
    double modulationRate;
    double modulationSpan;
};

class Generator : public QObject
{
    Q_OBJECT

private:
    SRS* srs;

    DS335* ds335;
    DS345* ds345;

    QString generatorType; // TODO: rename to model

    const double Err = 1E-10;

    const int waitTime = 700;

public:
    explicit Generator(QObject *parent = 0);
    explicit Generator(const QString &portName, const int &new_baudrate, const QString &new_generatorType = "");
    explicit Generator(QSerialPort *serial, const QString &new_generatorType = "");
    explicit Generator(DS335* new_ds335);
    explicit Generator(DS345* new_ds345);
    ~Generator();

    void toNULL();
    void init();

    Generator_State getState();
    bool setState(const Generator_State &new_state);

    void wait(const double &x = 1) const;

    void setDS335(DS335* new_ds335);
    DS335* getDS335() const;
    void setDS345(DS345* new_ds345);
    DS345* getDS345() const;

    bool openSerial(QSerialPort *serial) const;

    QStringList getSupportedTypes() const;
    void setGeneratorType(const QString &new_generatorType);
    QString getGeneratorType() const;
    bool autoSetGeneratorType(QSerialPort *serial);
    bool autoSetGeneratorType(const QString &portName, const int &new_baudrate);
    bool autoSetGeneratorType(const QString &portName);

    bool setConnection(const QString &portName, const int &new_baudrate, const QString &new_generatorType = "");
    bool disconnect() const;

    QStringList getBaudrateList() const;
    bool isValidBaudrateAll(const int &new_baudrate) const;
    bool isValidBaudrate(const int &new_baudrate) const;
    bool changeBaudrate(const int &new_baudrate);

    bool isActive() const;

    bool send(const QString &command, QString &response, const bool &wait_for_response) const;
    bool sendCommand(const QString &command) const;
    bool sendQuery(const QString &command, QString &response) const;
    QString ask(const QString &command) const;

    bool getIDN(QString &idn) const;

    bool test() const;

    bool setDefaultSettings() const;

    double getAverageInputTime() const;
    double getAverageOutputTime() const;

    bool workWithAmplitude() const;
    double getDefaultAmplitude() const;
    bool setDefaultAmplitude() const;
    QStringList getAmplitudeTypeList() const;
    double getMinAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const;
    double getMinAmplitude(const QString &waveform, const QString &unit) const;
    double getMaxAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const;
    double getMaxAmplitude(const QString &waveform, const QString &unit) const;
    double getStepAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const;
    double getStepAmplitude(const QString &waveform, const QString &unit) const;
    double getDecimalsAmplitude(const QString &waveform, const QString &outputZ, const bool &VRMS) const;
    double getDecimalsAmplitude(const QString &waveform, const QString &unit) const;
    bool isValidAmplitude(const double &new_amplitude, const QString &waveform, const QString &outputZ, const bool &VRMS) const;
    bool isValidAmplitude(const double &new_amplitude, const QString &waveform, const QString &unit) const;
    bool isValidAmplitudeType(const QString &unit) const;
    bool setAmplitude(const double &new_amplitude, const QString &unit) const;
    double getAmplitude(const QString &unit) const;

    bool workWithOffset() const;
    double getDefaultOffset() const;
    bool setDefaultOffset() const;
    double getMinOffset() const;
    double getMaxOffset() const;
    double getStepOffset() const;
    double getDecimalsOffset() const;
    bool isValidOffset(const double &offset) const;
    bool setOffset(const double &new_offset) const;
    bool getOffset() const;

    bool workWithFrequency() const;
    double getDefaultFrequency() const;
    bool setDefaultFrequency() const;
    double getMinFrequency(const QString &waveform) const;
    double getMaxFrequency(const QString &waveform) const;
    double getStepFrequency(const QString &waveform) const;
    double getDecimalsFrequency(const QString &waveform) const;
    bool setFrequency(const double &new_frequency) const;
    double getFrequency() const;

    bool workWithFunction() const;
    QString getDefaultFunction() const;
    bool setDefaultFunction() const;
    QStringList getFunctionList() const;
    int functionNumberFromString(const QString &function_string) const;
    QString functionStringFromNumber(const int &function_number) const;
    bool setFunction(const int &new_function) const;
    bool setFunction(const QString &new_function) const;
    QString getFunction() const;

    bool workWithInverse() const;
    bool getDefaultInverse() const;
    bool setDefaultInverse() const;
    bool setInverse(const bool &new_inverse) const;
    bool getInverse() const;

    bool workWithSynchronization() const;
    bool getDefaultSynchronization() const;
    bool setDefaultSynchronization() const;
    bool setSynchronization(const bool &new_synchronization) const;
    bool getSynchronization() const;

    bool workWithOutputImpedance() const;
    QString getDefaultOutputImpedance() const;
    bool setDefaultOutputImpedance() const;
    QStringList getOutputImpedanceList() const;
    int outputImpedanceNumberFromString(const QString &outputImpedance_string) const;
    QString outputImpedanceStringFromNumber(const int &outputImpedance_number) const;
    bool setOutputImpedance(const int &new_outputImpedance) const;
    bool setOutputImpedance(const QString &new_outputImpedance) const;
    QString getOutputImpedance() const;

    bool workWithAMDepth() const;
    int getDefaultAMDepth() const;
    bool setDefaultAMDepth() const;
    int getMinAMDepth() const;
    int getMaxAMDepth() const;
    bool isValidAMDepth(const int &persentage) const;
    bool setAMDepth(const int &new_persentage) const;
    int getAMDepth() const;

    bool workWithFMSpan() const;
    double getDefaultFMSpan() const;
    bool setDefaultFMSpan() const;
    double getMinFMSpan(const QString &waveform) const;
    double getMaxFMSpan(const QString &waveform) const;
    bool isValidFMSpan(const int &span, const QString &waveform) const;
    bool setFMSpan(const double &new_span) const;
    double getFMSpan() const;

    bool workWithModulationFunction() const;
    QString getDefaultModulationFunction() const;
    bool setDefaultModulationFunction() const;
    QStringList getModulationFunctionList() const;
    int modulationFunctionNumberFromString(const QString &function_string) const;
    QString modulationFunctionStringFromNumber(const int &function_number) const;
    bool setModulationFunction(const int &new_function) const;
    bool setModulationFunction(const QString &new_function) const;
    QString getModulationFunction() const;

    bool workWithModulationEnabled() const;
    bool getDefaultModulationEnabled() const;
    bool setDefaultModulationEnabled() const;
    bool setModulationEnabled(const bool &enable) const;
    bool getModulationEnabled() const;

    bool workWithModulationType() const;
    QString getDefaultModulationType() const;
    bool setDefaultModulationType() const;
    QStringList getModulationTypeList() const;
    int modulationTypeNumberFromString(const QString &type_string) const;
    QString modulationTypeStringFromNumber(const int &type_number) const;
    bool setModulationType(const int &new_type) const;
    bool setModulationType(const QString &new_type) const;
    QString getModulationType() const;

    bool workWithModulationRate() const;
    double getDefaultModulationRate() const;
    bool setDefaultModulationRate() const;
    double getMinModulationRate() const;
    double getMaxModulationRate() const;
    double getStepModulationRate() const;
    double getDecimalsModulationRate() const;
    bool isValidModulationRate(const double &rate) const;
    bool setModulationRate(const double &new_rate) const;
    double getModulationRate() const;

    bool workWithModulationSpan() const;
    double getDefaultModulationSpan() const;
    bool setDefaultModulationSpan() const;
    double getMinModulationSpan(const QString &waveform) const;
    double getMaxModulationSpan(const QString &waveform) const;
    double getStepModulationSpan(const QString &waveform) const;
    double getDecimalsModulationSpan(const QString &waveform) const;
    bool isValidModulationSpan(const double &span, const QString &waveform) const;
    bool setModulationSpan(const double &new_span) const;
    double getModulationSpan() const;

public slots:
    void catchErrorSignal(const QString &s) const;
    void catchResponseSignal(const QString &s) const;
    void catchCommandSignal(const QString &s) const;
    void catchTimeoutSignal(const QString &s) const;

signals:
    void errorSignal(const QString &s) const;
    void responseSignal(const QString &s) const;
    void commandSignal(const QString &s) const;
    void timeoutSignal(const QString &s) const;
};

#endif // GENERATOR_H
