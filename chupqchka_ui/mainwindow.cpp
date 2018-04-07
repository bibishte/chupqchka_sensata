
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    //serialPort=new QSerialPort();

    st_machine=new state_machine();
    st_machine->setTransaction(state_machine::TRANSACTION_DONE);
        thread_state_machine=new QThread();
        st_machine->moveToThread(thread_state_machine);

    connect(thread_state_machine, SIGNAL(started()), st_machine, SLOT(process()));
    connect(st_machine, SIGNAL(finished()), thread_state_machine, SLOT(quit()));
    connect(st_machine, SIGNAL(finished()), st_machine, SLOT(deleteLater()));
    connect(thread_state_machine, SIGNAL(finished()), thread_state_machine, SLOT(deleteLater()));

    connect(st_machine, SIGNAL(test(int)), this, SLOT(testslot(int)));

    thread_state_machine->start();


    scan_serial();

    connect(ui->rescan, SIGNAL(clicked(bool)), this, SLOT(scan_serial()));
    connect(st_machine, SIGNAL(controllers_mssg(QString,bool,state_machine::Message)),
                this,SLOT(message_manage(const QString,bool, state_machine::Message )));
    //connect(st_machine, SIGNAL(read_parameters(state_machine::Motors,int,int)),
    //            this, SLOT(read_param_motor(state_machine::Motors,int,int)));
    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(connect_stepper()));
    connect(ui->disconnect, SIGNAL(clicked(bool)), this, SLOT(disconnect_stepper()));




    connect(ui->spinBoxSteps, SIGNAL(valueChanged(int)),ui->sliderSteps,SLOT(setValue(int)) );
    connect(ui->sliderSteps,SIGNAL(valueChanged(int)),ui->spinBoxSteps,SLOT(setValue(int)) );
    connect(ui->spinBoxAcc, SIGNAL(valueChanged(int)),ui->sliderAcc,SLOT(setValue(int)) );
    connect(ui->sliderAcc,SIGNAL(valueChanged(int)),ui->spinBoxAcc,SLOT(setValue(int)) );
    connect(ui->spinBoxMaxSpd, SIGNAL(valueChanged(int)),ui->sliderMaxSpd,SLOT(setValue(int)) );
    connect(ui->sliderMaxSpd,SIGNAL(valueChanged(int)),ui->spinBoxMaxSpd,SLOT(setValue(int)) );


    connect(ui->storeButton, SIGNAL(clicked(bool)), this, SLOT(store_params()));
    connect(ui->readButton, SIGNAL(clicked(bool)), this,SLOT(read_param_motor(int,int,int)));


}

MainWindow::~MainWindow()
{
    delete ui;


}


void MainWindow::scan_serial(void)
{
    ui->serialPortComboBox->clear();
        const auto infos = QSerialPortInfo::availablePorts();
            for (const QSerialPortInfo &info : infos)
            {
                ui->serialPortComboBox->addItem(info.portName());

            }

            if(ui->serialPortComboBox->count()==0)
            {
                ui->serialPortComboBox->setDisabled(true);
                ui->connectButton->setDisabled(true);
                ui->statusLabel->setText(tr("have not found any ports"));
            }
            else
            {
                ui->serialPortComboBox->setDisabled(false);
                ui->connectButton->setDisabled(false);
                ui->statusLabel->setText(tr("rescan complete"));
            }

}


void MainWindow::message_manage(const QString mssg, bool a, state_machine::Message m)
{
    ui->statusLabel->setText(mssg);
        //ui->status->appendPlainText(mssg);
    switch (m)
    {
        case state_machine::MESSAGE_NO:
            if(a)
            {
                ui->connectButton->setDisabled(false);
                ui->rescan->setDisabled(false);
                ui->serialPortComboBox->setDisabled(false);
                ui->disconnect->setDisabled(true);

            }
            else
            {
                ui->connectButton->setDisabled(true);
                ui->rescan->setDisabled(true);
                ui->serialPortComboBox->setDisabled(true);
                ui->disconnect->setDisabled(false);

            }
            break;

    case state_machine::MESSAGE_ACC_WRITTEN:

        break;

    case state_machine::MESSAGE_MAXSPD_WRITTEN:

        break;

    case state_machine::MESSAGE_STEPS_WRITTEN:

        break;
    }
}

void MainWindow::connect_stepper(void)
{
    st_machine->openPort(ui->serialPortComboBox->currentText());
}

void MainWindow::disconnect_stepper(void)
{
    st_machine->closePort(ui->serialPortComboBox->currentText());
}

void MainWindow::store_params(void)
{
    //st_machine->store(ui->sliderSteps->value(), ui->sliderAcc->value(), ui->sliderMaxSpd->value());
    st_machine->setParameters(ui->sliderAcc->value(), ui->sliderMaxSpd->value(), ui->sliderSteps->value());
    st_machine->setTransaction(state_machine::TRANSACTION_ACC);
}


/*
void MainWindow::read_param_motor(int acc, int steps, int maxspd)
{
    st_machine->read_param( ui->spinBoxAcc->setValue(acc),ui->spinBoxSteps->setValue(steps),
                           ui->spinBoxMaxSpd->setValue(maxspd));

}*/
