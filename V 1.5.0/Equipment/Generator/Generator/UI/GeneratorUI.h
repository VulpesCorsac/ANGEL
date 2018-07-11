#ifndef GENERATORUI_H
#define GENERATORUI_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "../Generator.h"

namespace Ui {
    class GeneratorUI;
}

class GeneratorUI : public QMainWindow
{
    Q_OBJECT

private:
    Generator *generator = nullptr;
    bool filling = false;

public:
    explicit GeneratorUI(QWidget *parent = 0);
    explicit GeneratorUI(Generator_State *currentState); // TODO
    ~GeneratorUI();

private slots:
    void on_actionComListRefresh_triggered();
    void on_comboBoxGeneratorModel_currentTextChanged(const QString &arg1);
    void on_pushButtonComAction_clicked(); // TODO
    void connectGenerator(); // TODO

private:
    Ui::GeneratorUI *ui;
};

#endif // GENERATORUI_H
