#ifndef COMMTHREAD_H
#define COMMTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
class CommThread : public QThread
{
    Q_OBJECT
public:
    explicit CommThread(QObject *parent = nullptr);
    ~CommThread();
    void transaction(const QString &portName, int waitTimeout, const QString &request);
    void run() Q_DECL_OVERRIDE;
    bool release_Payload = false;
    bool release_CDA = false;
signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);
private:
    QString portName;
    QString request;
    int waitTimeout = 0;
    QMutex mutex;
    QWaitCondition cond;
    bool quit = false;

public slots:
};

#endif // COMMTHREAD_H
