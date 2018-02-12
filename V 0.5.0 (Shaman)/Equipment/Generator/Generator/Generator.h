#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QDebug>

#include "../AllGenerators.h"

class Generator : public QObject
{
    Q_OBJECT

private:
    DS335* ds335;
    DS345* ds345;
    QString generatorType;

public:
    explicit Generator(QObject *parent = 0);
    explicit Generator(const QString &portName, const int &new_baudrate, const QString &new_generatorType = "");
    explicit Generator(QSerialPort *serial, const QString &new_generatorType = "");
    explicit Generator(DS335* new_ds335);
    explicit Generator(DS345* new_ds345);
    ~Generator();

    void setDS335(DS335* new_ds335);
    DS335* getDS335() const;
    void setDS345(DS345* new_ds345);
    DS345* getDS345() const;

    bool openSerial(QSerialPort *serial) const;

    void setGeneratorType(const QString &new_generatorType);
    QString getGeneratorType() const;
    bool autoSetGeneratorType(QSerialPort *serial);

    bool setConnection(const QString &portName, const int &new_baudrate, const QString &new_generatorType = "");
    bool disconnect() const;

    bool isValidBaudrate(const int &new_baudrate) const;
    bool changeBaudrate(const int &new_baudrate);

    bool isActive() const;

    bool send(const QString &command, QString &response, const bool &wait_for_response) const;
    bool sendCommand(const QString &command) const;
    bool sendQuery(const QString &command, QString &response) const;

    bool getIDN(QString &idn) const;

    bool setAmplitude(const double &new_amplitude, const QString &type) const;
    double getAmplitude(QString &type) const;

    bool setFrequency(const double &new_frequency) const;
    double getFrequency() const;

    int functionToNumber(const QString &function_string) const;
    QString numberToFunction(const int &function_number) const;

    bool setFunction(const int &function_number) const;
    bool setFunction(const QString &function) const;
    QString getFunction() const;

    bool setInverse(const bool &new_inverse) const;
    bool getInverse() const;

    bool setSynchronization(const bool &new_synchronization) const;
    bool getSynchronization() const;

    int impedanceToNumber(const QString &impedance_string) const;
    QString numberToImpedance(const int &impedance_number) const;

    bool setOutputImpedance(const QString &impedance_string) const;
    bool setOutputImpedance(const int &impedance_number) const;
    QString getOutputImpedance() const;

    bool setAMdepth(const int &new_persentage) const;
    int getAMdepth() const;

    bool setFMspan(const double &new_span) const;
    double getFMspan() const;

    int modulateFunctionToNumber(const QString &function_string) const;
    QString numberToModulateFunction(const int &function_number) const;

    bool setModulationFunction(const int &function_number) const;
    bool setModulationFunction(const QString &function_string) const;
    QString getModulationFunction() const;

    bool setModulationEnabled(const bool &enable) const;
    bool getModulationEnabled() const;

    int modulateTypeToNumber(const QString &type_string) const;
    QString numberToModulateType(const int &type_number) const;

    bool setModulationType(const int &function_number) const;
    bool setModulationType(const QString &function_string) const;
    QString getModulationType() const;

    bool setModulationRate(const double &new_rate) const;
    double getModulationRate() const;

    bool setModulationSpan(const double &new_span) const;
    double getModulationSpan() const;

signals:
    void errorSignal(const QString &s) const;
    void responseSignal(const QString &s) const;
    void commandSignal(const QString &s) const;
    void timeoutSignal(const QString &s) const;

public slots:
};

#endif // GENERATOR_H
