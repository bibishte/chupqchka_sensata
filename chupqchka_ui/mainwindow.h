#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "state_machine.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    QSerialPort serialPort;
    state_machine *st_machine;
    QThread *thread_state_machine;

private slots:

public slots:
    void scan_serial(void);
    void message_manage(const QString mssg, bool a, state_machine::Message m);
    void connect_stepper(void);
    void disconnect_stepper(void);
    void store_params(void);
    //void read_param_motor(int acc, int steps, int maxspd);

signals:


};

#endif // MAINWINDOW_H
