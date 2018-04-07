#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <QObject>
#include <QThread>
#include <QtSerialPort/QSerialPort>

class state_machine : public QObject
{
    Q_OBJECT
    Q_ENUMS(Message)
    Q_ENUMS(State_transaction)

public:
    state_machine();
    ~state_machine();


    enum Message
        {
            MESSAGE_NO,
            MESSAGE_ACC_WRITTEN,
            MESSAGE_MAXSPD_WRITTEN,
            MESSAGE_STEPS_WRITTEN,
            MESSAGE_EVERYTHING_WRITTEN,
            MESSAGE_READ

        };

    enum State_transaction
    {
        TRANSACTION_MAX_SPD,
        TRANSACTION_ACC,
        TRANSACTION_STEPS,
        TRANSACTION_DONE
    };


    void store(void);
    int read_param(int acc, int steps, int maxspd);
    State_transaction transaction_state;
    void setTransaction(State_transaction a);
    state_machine::State_transaction getTransaction();
    void setParameters(int a, int b, int c);


signals:
    void controllers_mssg(const QString , bool, state_machine::Message );
    //void read_parameters(int ,int, int);


    //void finished();


    //void test(int i);

private:
    QSerialPort *stepper_serial;
    int acceleration;
    int maxSpeed;
    int steps;

    QString read_serial(int timeout);
    //Q_INVOKABLE
    bool write_serial(QString writedata, int timeout);
    //Q_INVOKABLE
    bool transaction(QString a, QString b, int c);

public slots:

    void process();

    void openPort(QString port_name);
    void closePort(QString port_name);
    //void disconnect_stepper(void);
    ;

};

Q_DECLARE_METATYPE(state_machine::Message)
Q_DECLARE_METATYPE(state_machine::State_transaction)

#endif // STATE_MACHINE_H
