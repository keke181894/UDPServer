#ifndef CRECEIVE_H
#define CRECEIVE_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>

class CReceive : public QObject
{
    Q_OBJECT
public:
    explicit CReceive(QObject *parent = nullptr);
    ~CReceive();
    void Init(QString tmphost, unsigned short tmport);
    void Stop();
    void Start();
signals:
    void deliverInfo(QByteArray info, QString clientip);
public slots:
    void readDatagrams(); //listen UDP data
private:
    QUdpSocket *m_udpSocket;
    QString m_localhost;
    unsigned short m_port;
    QByteArray m_data;
    QThread *m_thread;
    bool m_bStop;
};

#endif // CRECEIVE_H
