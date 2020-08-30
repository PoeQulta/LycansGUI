#include "commthread.h"
#include <QtSerialPort/QSerialPort>

#include <QTime>
#include <QDebug>
#include <QTextCodec>
QT_USE_NAMESPACE

CommThread::CommThread(QObject *parent)
    : QThread(parent), waitTimeout(0), quit(false)
{
}

CommThread::~CommThread()
{
    mutex.lock();
    quit = true;
    mutex.unlock();
    wait();
}
//intiates connection to port
void CommThread::transaction(const QString &portName, int waitTimeout, const QString &request)
{
    QMutexLocker locker(&mutex);
          this->portName = portName;
          this->waitTimeout = waitTimeout;
          this->request = request;
          if (!isRunning())
              start();
}
//main thread function
void CommThread::run()
{
    bool currentPortNameChanged = false;

    mutex.lock();
    QString currentPortName;
    if (currentPortName != portName) {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTimeout;
    QString currentRequest = request;
    mutex.unlock();
    QSerialPort serial;
    serial.setBaudRate(QSerialPort::Baud9600);
//error handling
    if (currentPortName.isEmpty()) {
        emit error(tr("No port name specified"));
        return;
    }

    while (!quit) {
        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);
            serial.setBaudRate(QSerialPort::Baud9600);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(portName).arg(serial.error()));
                return;
            }
        }
        // write request
        //checks if any of the release buttons have been clicked and if so sends a request to Arduino
        if (this->release_Payload)

        {
            QString test("A");
            QByteArray requestData = test.toLocal8Bit();
            serial.write(requestData);
            this->release_Payload = false;
        }
        if (this->release_CDA)
        {
            QString test("B");
            QByteArray requestData = test.toLocal8Bit();
            serial.write(requestData);
            this->release_CDA = false;
        }
        //reads incoming data and passes it to GUI thread
        if (serial.waitForReadyRead(currentWaitTimeout)) {
                      // read request
                      QByteArray requestData = serial.readAll();
                      while (serial.waitForReadyRead(10))
                          requestData += serial.readAll();
                      QString request(requestData);
                      emit this->response(request);

                  } else {
                      emit timeout(tr("Wait read request timeout %1")
                                   .arg(QTime::currentTime().toString()));

                  }
        mutex.lock();
                 if (currentPortName != portName) {
                     currentPortName = portName;
                     currentPortNameChanged = true;
                 } else {
                     currentPortNameChanged = false;
                 }
                 currentWaitTimeout = waitTimeout;
                 currentRequest = request;
                 mutex.unlock();
    }
}
