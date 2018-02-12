#ifndef GENERATORUI_H
#define GENERATORUI_H

#include <QMainWindow>

namespace Ui {
class GeneratorUI;
}

class GeneratorUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit GeneratorUI(QWidget *parent = 0);
    ~GeneratorUI();

private:
    Ui::GeneratorUI *ui;
};

#endif // GENERATORUI_H
