#ifndef DS345_H
#define DS345_H

#include <QObject>
#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class DS345 : public QObject
{
    Q_OBJECT

private:
    const int defaultWriteTimeout    = 50;
    const int defaultReadTimeout     = 200;
    const int defaultReadWaitTimeout = 50;

    QSerialPort* serial;

public:
    explicit DS345(); // TAKEN
    explicit DS345(const QString &portName, const int &new_baudrate); // TAKEN
    ~DS345(); // TAKEN

    QSerialPort* getSerial() const; // TAKEN
    void setSerial(QSerialPort* new_serial); // TAKEN

    bool setConnection(const QString &portName, const int &new_baudrate); // TAKEN
    void disconnect() const; // TAKEN

    bool isValidBaudrate(const int &new_baudrate) const; // TAKEN
    bool changeBaudrate(const int &new_baudrate); // TAKEN

    bool isActive() const; // TAKEN

    bool send(const QString &command, QString &response, const bool &wait_for_response) const; // TAKEN
    bool sendCommand(const QString &command) const; // TAKEN
    bool sendQuery(const QString &command, QString &response) const; // TAKEN

    bool getIDN(QString &idn) const; // TAKEN

    bool correctAmplitudeType(const QString &type) const;
    bool setAmplitude(const double &new_amplitude, const QString &type) const; // MODIFIED
    double getAmplitude(QString &type) const; // MODIFIED
    double getMaxAmplitude() const; // TODO

    bool setFrequency(const double &new_frequency) const; // TAKEN
    double getFrequency() const; // TAKEN
    double getMaxFrequency() const; // TODO

    int functionToNumber(const QString &function_string) const; // MODIFIED
    QString numberToFunction(const int &function_number) const; // MODIFIED

    bool setFunction(const int &function_number) const; // TAKEN
    bool setFunction(const QString &function) const; // TAKEN
    QString getFunction() const; // TAKEN
    QStringList getFunctionList() const; // TODO

    bool setInverse(const bool &new_inverse) const; // TAKEN
    bool getInverse() const; // TAKEN

    bool setSynchronization(const bool &new_synchronization) const; // TAKEN
    bool getSynchronization() const; // TAKEN

    bool setAMdepth(const int &new_persentage) const;
    int getAMdepth() const;

    bool setFMspan(const double &new_span) const;
    double getFMspan() const;

    int modulateFunctionToNumber(const QString &function_string) const;
    QString numberToModulateFunction(const int &function_number) const;
    QStringList getModulateFunctionList() const; // TODO

    bool setModulationFunction(const int &function_number) const;
    bool setModulationFunction(const QString &function_string) const;
    QString getModulationFunction() const;

    bool setModulationEnabled(const bool &enable) const;
    bool getModulationEnabled() const;

    QString numberToModulateType(const int &type_number) const;
    int modulateTypeToNumber(const QString &type_string) const;

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
};

#endif // DS345_H
