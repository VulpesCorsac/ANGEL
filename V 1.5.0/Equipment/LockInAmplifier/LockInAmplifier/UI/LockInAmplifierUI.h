#ifndef LOCKINAMPLIFIERUI_H
#define LOCKINAMPLIFIERUI_H

#include <QMainWindow>

namespace Ui {
class LockInAmplifierUI;
}

class LockInAmplifierUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit LockInAmplifierUI(QWidget *parent = 0);
    ~LockInAmplifierUI();

private:
    Ui::LockInAmplifierUI *ui;
};

#endif // LOCKINAMPLIFIERUI_H
