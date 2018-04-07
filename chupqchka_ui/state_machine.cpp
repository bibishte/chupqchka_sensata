#include "state_machine.h"
#include <QtSerialPort/QSerialPort>
#include <stdio.h>

state_machine::state_machine()
{
    stepper_serial=new QSerialPort();


}

state_machine::~state_machine()
{

}

void state_machine::openPort(QString port_name)
{
//    stepper_serial->baudRate(stepper_serial->AllDirections);
    stepper_serial->setBaudRate(stepper_serial->Baud115200);

    stepper_serial->setDataBits(stepper_serial->Data8);
    stepper_serial->setFlowControl(stepper_serial->NoFlowControl);
    stepper_serial->setParity(stepper_serial->NoParity);
    stepper_serial->setStopBits(stepper_serial->OneStop);
    stepper_serial->setPortName(port_name);


    if (!stepper_serial->open(QIODevice::ReadWrite))
    {
        emit controllers_mssg(tr("Can't open %1, error code %2")
              .arg(port_name).arg(stepper_serial->error()), true, MESSAGE_NO);
    }
    else
    {

        emit controllers_mssg(tr("opened %1")
                  .arg(port_name), false, MESSAGE_NO);
    }
}

void state_machine::closePort(QString port_name)
{
    if (!stepper_serial->isOpen())
    {

        emit controllers_mssg(tr("no open port found")
                   , true, MESSAGE_NO);
    }
    else
    {
        stepper_serial->close();
        emit controllers_mssg(tr("closed %1")
                   .arg(port_name), true, MESSAGE_NO);
    }
}


QString state_machine::read_serial(int timeout)
{
    bool a=true;
    QByteArray responseData;

    if(!stepper_serial->isOpen())
    {
        return "serial not open";
    }

    while(a==true)
    {
        a=stepper_serial->waitForReadyRead(timeout);
    }

    responseData = stepper_serial->readAll();
    if(responseData.contains(0x0d))
    {
        return responseData;

    }
    return NULL;

}

bool state_machine::write_serial(QString writedata, int timeout)
{
    bool a=true;
    QByteArray responseData;
    QByteArray requestData = writedata.toLocal8Bit();
    if(!stepper_serial->isOpen())
    {
        return false;
    }
    stepper_serial->clear();
    stepper_serial->write(requestData);
    while (a==true)
    {
        a=stepper_serial->waitForBytesWritten(timeout);
    }

    a=true;
    while(a==true)
    {
        a=stepper_serial->waitForReadyRead(timeout);
    }

    responseData = stepper_serial->readAll();
    a=stepper_serial->waitForReadyRead(timeout);
    responseData = stepper_serial->readAll();
    a=stepper_serial->waitForReadyRead(timeout);
    responseData = stepper_serial->readAll();

    return true;

}


bool state_machine::transaction(QString a, QString b, int c)
{
    QByteArray requestData = a.toLocal8Bit();
    QByteArray answer = b.toLocal8Bit();
    //answer.append(0x0d);
    if(!stepper_serial->isOpen())
    {
        return false;
    }
    //stepper_serial->clear();
    stepper_serial->write(requestData);
    //stepper_serial->waitForBytesWritten(-1);
    //stepper_serial->waitForReadyRead(c);
    if (stepper_serial->waitForBytesWritten(c))
    {
        //if (stepper_serial->waitForReadyRead(c))
        //{
            QByteArray responseData = stepper_serial->readAll();
            responseData = stepper_serial->readAll();
            responseData = stepper_serial->readAll();
            responseData = stepper_serial->readAll();
            responseData = stepper_serial->readAll();
            if(responseData.contains(0x0d))
            {
                if(answer.contains(responseData))
                {
                    return true;
                }


            }
       // }
    }
    return false;
}

void state_machine::store(void)
{
    QString b;
    QString c;
    QString d;
    //QString e;
    QString f;
    //QString g;

    b.sprintf("acc=%d\r", acceleration);
    c.sprintf("OK\r");
    d.sprintf("maxspd=%d\r", maxSpeed);
    //e.sprintf("OK\r");
    f.sprintf("steps=%d\r", steps);
    //g.sprintf("OK\r");

/*
    if(transaction(b, c, 100))
    {
        emit controllers_mssg(tr("succesfully written acceleration"), true, MESSAGE_ACC_WRITTEN);
        if(transaction(d, e, 100))
        {
             emit controllers_mssg(tr("succesfully written max speed"), true, MESSAGE_MAXSPD_WRITTEN);
             //return true;
             if(transaction(f, g, 100))
             {
                 emit controllers_mssg(tr("succesfully written steps"), true, MESSAGE_STEPS_WRITTEN);
                 return true;
             }
             else
             {
                 emit controllers_mssg(tr("haven't' written steps"), false, MESSAGE_STEPS_WRITTEN);
                 return false;
             }
        }
        else
        {
              emit controllers_mssg(tr("haven't' written max speed"), false, MESSAGE_MAXSPD_WRITTEN);
              return false;
         }
    }
    else
    {
        emit controllers_mssg(tr("haven't' written acceleration"), false, MESSAGE_ACC_WRITTEN);
        return false;
     }
*/


    switch (transaction_state) {
    case TRANSACTION_DONE:

        emit controllers_mssg(tr("succesfully written all parameters"), true, MESSAGE_EVERYTHING_WRITTEN);
        break;

    case TRANSACTION_ACC:
        //if(transaction(b, c, 3000))
        if(write_serial(b, 5))
        {
            read_serial(600);
            read_serial(600);
            read_serial(600);
            read_serial(600);
            if(read_serial(600).contains(c))
            {
                emit controllers_mssg(tr("succesfully written acceleration"), true, MESSAGE_ACC_WRITTEN);
                transaction_state=TRANSACTION_MAX_SPD;
            }
            else
            {
                emit controllers_mssg(tr("wrong answer acc acceleration"), false, MESSAGE_NO);
                transaction_state=TRANSACTION_DONE;
            }
        }
        else
        {
            emit controllers_mssg(tr("haven't' written acceleration"), false, MESSAGE_ACC_WRITTEN);
            transaction_state=TRANSACTION_DONE;
        }

        break;

    case TRANSACTION_MAX_SPD:
        //if(transaction(d, c, 3000))
        if(write_serial(d, -1))
        {
            if(read_serial(600).contains(c))
            {
                emit controllers_mssg(tr("succesfully written max speed"), true, MESSAGE_MAXSPD_WRITTEN);
                transaction_state=TRANSACTION_STEPS;
            }
            else
            {
                emit controllers_mssg(tr("wrong answer max speed"), false, MESSAGE_MAXSPD_WRITTEN);
                transaction_state=TRANSACTION_DONE;
            }
        }
        else
        {
            emit controllers_mssg(tr("haven't' written max speed"), false, MESSAGE_MAXSPD_WRITTEN);
            transaction_state=TRANSACTION_DONE;
        }
        break;

    case TRANSACTION_STEPS:
        //if(transaction(f, c, 3000))
        if(write_serial(f, -1))
        {
            if(read_serial(600).contains(c))
            {
                emit controllers_mssg(tr("succesfully written steps"), true, MESSAGE_STEPS_WRITTEN);
                transaction_state=TRANSACTION_DONE;
            }
            else
            {
                emit controllers_mssg(tr("havent answer steps"), true, MESSAGE_STEPS_WRITTEN);
                transaction_state=TRANSACTION_DONE;
            }
        }
        else
        {
            emit controllers_mssg(tr("haven't' written steps"), false, MESSAGE_STEPS_WRITTEN);
            transaction_state=TRANSACTION_DONE;
        }
        break;
    default:
        break;
    }
}

/*
int state_machine::read_param(int acc, int steps, int maxspd)
{
    QString b;
    QString c;
    QString d;
    QString e;
    QString f;
    QString g;
    b.sprintf("acc?\r");
    c.sprintf("%d", acc);
    d.sprintf("maxspd?\r");
    e.sprintf("%d", max_spd);
    f.sprintf("steps?\r");
    g.sprintf("%d", steps);


    if(transaction(b, c, 100) && transaction(d, e, 100) && transaction(f, g, 100))
    {
        //emit controllers_mssg(tr("succesfully read"), true, MESSAGE_READ);
        emit read_param(acc, steps, maxspd);
        return true;

    else
    {
        emit controllers_mssg(tr("haven't' read"), false, MESSAGE_READ);
        return false;
     }

}*/


void state_machine::setTransaction(State_transaction a)
{
    transaction_state=a;
}

state_machine::State_transaction state_machine::getTransaction()
{
    return transaction_state;
}

void state_machine::process()
{
    while(1)
    {
        QThread::msleep(5);
        store();
    }

}

void state_machine::setParameters(int a, int b, int c)
{
    acceleration=a;
    maxSpeed=b;
    steps=c;
}


