#ifndef SMSD_4_2_UI_H
#define SMSD_4_2_UI_H

#include <QDialog>

#include "../SMSD_4_2.h"

namespace Ui {
	class SMSD_4_2_UI;
}

class SMSD_4_2_UI : public QDialog
{
    Q_OBJECT

private:
    SMSD_4_2* stepMotor;
    int defaultAcceleration  = 0;
    int defaultPause         = 0;
    int defaultStartingSpeed = 1;
    int defaultSpeed         = 10000;
    int defaultStep          = 10000;
    int maxCheating          = 10;

public:
    explicit SMSD_4_2_UI(QWidget *parent = 0);
    ~SMSD_4_2_UI();

private:
    bool isNULL() const;
    void closing();

public:
    void setStepMotor(SMSD_4_2* new_stepMotor);
    SMSD_4_2* getStepMotor(void) const;

private:
    bool simpleCheating();
    bool cheating();

private slots:
    void showResponse(const QString &s);
    void showCommand(const QString &s);

    void on_comboBoxMode_currentTextChanged(const QString &arg1);
    bool on_comboBoxMovementDirection_currentTextChanged(const QString &arg1);

    void on_pushButtonExit_clicked();

    bool on_pushButtonEnable_clicked();
    bool on_pushButtonDisable_clicked();

    bool on_pushButtonMoveUntillZero_clicked();
    bool on_pushButtonMove_clicked();

    bool on_pushButtonReverse_clicked();

    void on_spinBoxAcceleration_valueChanged(int arg1);
    void on_spinBoxPause_valueChanged(int arg1);
    void on_spinBoxSpeed_valueChanged(int arg1);
    void on_spinBoxStartingSpeed_valueChanged(int arg1);
    void on_spinBoxStep_valueChanged(int arg1);

    bool on_pushButtonAccelerationAccept_clicked();
    bool on_pushButtonAccelerationDefault_clicked();

    bool on_pushButtonPauseAccept_clicked();
    bool on_pushButtonSpeedAccept_clicked();

    bool on_pushButtonSpeedDefault_clicked();
    bool on_pushButtonStartingSpeedAccept_clicked();

    bool on_pushButtonStartingSpeedDefault_clicked();
    bool on_pushButtonStepAccept_clicked();

    void on_pushButtonRun_clicked();
    bool on_pushButtonAbort_clicked();

    void on_pushButtonComAction_clicked();

private:
    Ui::SMSD_4_2_UI *ui;
};

#endif // SMSD_4_2_UI_H
