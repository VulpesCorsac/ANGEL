#ifndef MDG_23_BY_GEO_STRESS_TEST_SIMPLE_H
#define MDG_23_BY_GEO_STRESS_TEST_SIMPLE_H

#include <QDialog>
#include <QTest>
#include <QTime>

#include "../MDG_23_by_Geo.h"
#include "../../../../Functions/AllFunctions.h"

namespace Ui {
    class MDG_23_by_Geo_Stress_Test_Simple;
}

class MDG_23_by_Geo_Stress_Test_Simple : public QDialog
{
    Q_OBJECT

private:
    MDG_23_by_Geo* monochromator;

private:
    int additionalSteps = 0;
    int speed = 10000;
    int times_pause_seconds = 3;
    int reverse_pause_seconds = 30;
    int repeat_pause_seconds = 900;
    int time_length = 0;
    QTime timer;

public:
    explicit MDG_23_by_Geo_Stress_Test_Simple(QWidget *parent = 0);
    ~MDG_23_by_Geo_Stress_Test_Simple();

private:
    bool isNULL() const;
    void closing();

public:
    void setMonochromator(MDG_23_by_Geo* new_Monochromator);
    MDG_23_by_Geo* getMonochromator() const;

private:
    bool movement();
    bool movement_and_swap();
    bool round();

private slots:
    void on_pushButtonStartingPoint_clicked();
    void on_pushButtonAction_clicked();
    bool on_pushButtonCoarseLeft_clicked();
    bool on_pushButtonCoarseRight_clicked();
    bool on_pushButtonFineLeft_clicked();
    bool on_pushButtonFineRight_clicked();

private:
    Ui::MDG_23_by_Geo_Stress_Test_Simple *ui;
};

#endif // MDG_23_BY_GEO_STRESS_TEST_SIMPLE_H
