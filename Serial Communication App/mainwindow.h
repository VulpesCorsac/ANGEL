#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTime>
#include <QDebug>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    int firstWaitTime = 500; // Первичное время ожидания новых данных, мс
    int additionalWaitTime = 5; // Дополнительное время ожидания новых данных, мс
    QSerialPort *serial; // SerialPort
    QTime time; // Для измерения времени отклика

public:
    explicit MainWindow(QWidget *parent = 0); // Конструктор класса
    ~MainWindow(); // Деструктор класса

private slots:
    void on_actionCOMUPD_triggered(); // Обновление списка портов

    void on_pushButtonAction_clicked(); // Подключение и отключение

    void on_pushButtonSend_clicked(); // Отправка команды
    void on_pushButtonRecieve_clicked(); // Получение ответа

    void get_response(const QString &s); // Выведение ответа или отчёта об ошибки

signals:
    void response(const QString &s); // Отправка ответа в слот

private:
    Ui::MainWindow *ui; // Пользовательский интерфейс
};

#endif // MAINWINDOW_H
