#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Это QT добавил автоматически

    ui->pushButtonAction->setText("Connect"); // Кнопка работает в режиме "Подключить"

    QStringList Bauds; // Заполнение настройки частоты передачи данных
    Bauds.push_back(   "300");
    Bauds.push_back(   "600");
    Bauds.push_back(  "1200");
    Bauds.push_back(  "2400");
    Bauds.push_back(  "4800");
    Bauds.push_back(  "9600");
    Bauds.push_back( "19200");
    Bauds.push_back( "38400");
    Bauds.push_back( "57600");
    Bauds.push_back("115200");
    ui->comboBoxBaud->addItems(Bauds);
    ui->comboBoxBaud->setCurrentText("9600"); // Указание "дефолтного" значения

    QStringList Parity; // Заполнение настройки чётности протокола
    Parity.push_back("NO");
    Parity.push_back("EVEN");
    Parity.push_back("ODD");
    Parity.push_back("MARK");
    Parity.push_back("SPACE");
    ui->comboBoxParity->addItems(Parity);
    ui->comboBoxParity->setCurrentText("NO"); // Указание "дефолтного" значения

    QStringList Data; // Заполнение настройки количества битов данных
    Data.push_back("6");
    Data.push_back("5");
    Data.push_back("7");
    Data.push_back("8");
    ui->comboBoxData->addItems(Data);
    ui->comboBoxData->setCurrentText("8"); // Указание "дефолтного" значения

    QStringList Stop; // Заполнение настройки количества стоп-битов
    Stop.push_back("1");
    Stop.push_back("1.5");
    Stop.push_back("2");
    ui->comboBoxStop->addItems(Stop);
    ui->comboBoxStop->setCurrentText("1"); // Указание "дефолтного" значения

    ui->comboBoxFlowControl->addItem("NO"); // Заполнение настройки окончания строки
    ui->comboBoxFlowControl->addItem("SOFT");
    ui->comboBoxFlowControl->addItem("HARD");
    ui->comboBoxFlowControl->setCurrentText("NO"); // Указание "дефолтного" значения

    ui->comboBoxLineTerminator->addItem("NO"); // Заполнение настройки окончания строки
    ui->comboBoxLineTerminator->addItem("EOL");
    ui->comboBoxLineTerminator->setCurrentText("EOL"); // Указание "дефолтного" значения

    on_actionCOMUPD_triggered(); // Обнновить список доступных портов

    serial = new QSerialPort();  // Создать новый объект класса "SerialPort"

    connect(this, SIGNAL(response(QString)),
            this, SLOT(get_response(QString))); // Подключить сигнал получения ответа к слоту
}

MainWindow::~MainWindow()
{
    delete serial; // Удалить ненужные переменные
    delete ui;
}

void MainWindow::on_actionCOMUPD_triggered()
{
    ui->comboBoxCOM->clear(); // Стереть все имевшиеся старые порты

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) { // Добавление доступных в системе портов
        ui->comboBoxCOM->addItem(info.portName());
    }

    return;
}

void MainWindow::on_pushButtonAction_clicked()
{
    if (ui->pushButtonAction->text() == "Connect") { // Если нам нужно подключиться
        serial->setPortName(ui->comboBoxCOM->currentText()); // Указание имени порта

        serial->setBaudRate(ui->comboBoxBaud->currentText().toInt()); // Указание частоты передачи порта

        if (ui->comboBoxParity->currentText() == "NO") // Указание чётности протокола передачи данных
            serial->setParity(QSerialPort::NoParity);
        if (ui->comboBoxParity->currentText() == "ODD")
            serial->setParity(QSerialPort::OddParity);
        if (ui->comboBoxParity->currentText() == "EVEN")
            serial->setParity(QSerialPort::EvenParity);
        if (ui->comboBoxParity->currentText() == "MARK")
            serial->setParity(QSerialPort::MarkParity);
        if (ui->comboBoxParity->currentText() == "SPACE")
            serial->setParity(QSerialPort::SpaceParity);

        if (ui->comboBoxStop->currentText() == "1") // Указание количества стоповых бит
            serial->setStopBits(QSerialPort::OneStop);
        if (ui->comboBoxStop->currentText() == "1.5")
            serial->setStopBits(QSerialPort::OneAndHalfStop);
        if (ui->comboBoxStop->currentText() == "2")
            serial->setStopBits(QSerialPort::TwoStop);

        if (ui->comboBoxData->currentText() == "5") // Указание количества бит данных
            serial->setDataBits(QSerialPort::Data5);
        if (ui->comboBoxData->currentText() == "6")
            serial->setDataBits(QSerialPort::Data6);
        if (ui->comboBoxData->currentText() == "7")
            serial->setDataBits(QSerialPort::Data7);
        if (ui->comboBoxData->currentText() == "8")
            serial->setDataBits(QSerialPort::Data8);

        if (ui->comboBoxFlowControl->currentText() == "NO") // Указание метода контроля передачи данных
            serial->setFlowControl(QSerialPort::NoFlowControl);
        if (ui->comboBoxFlowControl->currentText() == "SOFT")
            serial->setFlowControl(QSerialPort::SoftwareControl);
        if (ui->comboBoxFlowControl->currentText() == "HARD")
            serial->setFlowControl(QSerialPort::HardwareControl);

        if (!serial->open(QIODevice::ReadWrite)) { // Если попытка открыть порт для ввода\вывода не получилось
            QSerialPort::SerialPortError getError; // Ошибка открытия порта
            serial->error(getError); // Получить номер ошибки
            emit response(tr("Can't open %1, error code %2").arg(ui->comboBoxCOM->currentText()).arg(serial->error())); // Выдать сообщение об ошибке

            return;
        }

        ui->pushButtonAction->setText("Disconnect"); // Перевести кнопку в режим "Отключение"
    } else { // Если нам нужно отключиться
        this->serial->close(); // Закрыть открытый порт

        ui->pushButtonAction->setText("Connect"); // Перевести кнопку в режим "Подключение"
    }

    // Блокировка или разблокировка переключателей настроек порта
    ui->comboBoxCOM->setEnabled(ui->pushButtonAction->text() == "Connect");
    ui->comboBoxBaud->setEnabled(ui->pushButtonAction->text() == "Connect");
    ui->comboBoxData->setEnabled(ui->pushButtonAction->text() == "Connect");
    ui->comboBoxParity->setEnabled(ui->pushButtonAction->text() == "Connect");
    ui->comboBoxStop->setEnabled(ui->pushButtonAction->text() == "Connect");

    // Блокировка или разблокировка кнопок отправки и приёма сообщений
    ui->comboBoxLineTerminator->setEnabled(ui->pushButtonAction->text() == "Disconnect");
    ui->lineEditCommand->setEnabled(ui->pushButtonAction->text() == "Disconnect");
    ui->pushButtonSend->setEnabled(ui->pushButtonAction->text() == "Disconnect");
    ui->pushButtonRecieve->setEnabled(ui->pushButtonAction->text() == "Disconnect");

    return;
}

void MainWindow::on_pushButtonSend_clicked()
{
    QString command = ui->lineEditCommand->text(); // Получить команду из текстового поля

    if (ui->comboBoxLineTerminator->currentText() == "EOL") // При необходимости добавить символ окончания строки
        command += "\n";

    serial->write(command.toLocal8Bit()); // Передать данные по порту в битовом представлении

    on_pushButtonRecieve_clicked(); // Получить ответ

    return;
}

void MainWindow::on_pushButtonRecieve_clicked()
{
    QString temp; // Строка для записи ответа
    QByteArray array; // Массив данных ответа

    this->firstWaitTime = ui->spinBoxTimeout->value(); // Задать выбранный пользователем таймаут

    this->time = QTime(0, 0, 0, 0); // Обнуление времени
    time.start(); // Начала счётчика

    if (serial->waitForReadyRead(this->firstWaitTime)) { // Если за данное число миллисекунд что-то пришло
        array = serial->readAll(); // Прочитать полученные данные
        while (serial->waitForReadyRead(this->additionalWaitTime)) // Если пришло что-то ещё
            array += serial->readAll(); // Дописать новые данные
        temp = QString(array); // Преобразовать полученные данные в строку

        ui->lineEditTTR->setText(QString::number(time.elapsed()-this->additionalWaitTime)); // Записать время отклика
    } else { // Если не пришло ничего
        temp = "Wait write request timeout"; // Сформировать отчёт об ошибке

        ui->lineEditTTR->setText(">" + QString::number(this->firstWaitTime)); // Время отклика не определено точно
    }

    emit response(temp); // Вывести полученный ответ на экран

    return;
}

void MainWindow::get_response(const QString &s)
{
    ui->lineEditResponse->setText(s); // Вывести сообщение в текстовое поле

    return;
}
