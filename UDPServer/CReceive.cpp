#include "CReceive.h"
#include <QDebug>

CReceive::CReceive(QObject *parent) : QObject(parent)
{
    //m_thread = new QThread();
    m_udpSocket = new QUdpSocket();
    m_localhost = "127.0.0.1";
    m_port = 8888;


    //this->moveToThread(m_thread);
    //m_udpSocket->moveToThread(m_thread);


    //m_thread->start();
}

CReceive::~CReceive()
{

    if(m_udpSocket != NULL)
    {
        m_udpSocket->close();
        m_udpSocket->deleteLater();
    }

//    m_thread->quit();
//    m_thread->wait();
//    m_thread->deleteLater();
}

void CReceive::Init(QString tmphost, unsigned short tmport)
{
    m_port = tmport;
    m_localhost = tmphost;
    if(m_udpSocket != NULL && m_udpSocket->isValid())
    {
        Stop();
    }
}

void CReceive::Start()
{
    if(m_udpSocket == NULL)
    {
        m_udpSocket = new QUdpSocket(this);
        m_udpSocket->bind(QHostAddress(m_localhost),m_port);
        connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readDatagrams()), Qt::DirectConnection);
    }
    else
    {
        m_udpSocket->bind(QHostAddress(m_localhost),m_port);
        connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readDatagrams()), Qt::DirectConnection);
    }
}

void CReceive::Stop()
{

    //m_thread->deleteLater();
    m_udpSocket->close();

//    m_thread->quit();
//    m_thread->wait();
}

void CReceive::readDatagrams()
{
    QHostAddress client_address; //client ip addr
    m_data.clear();
    while(m_udpSocket->hasPendingDatagrams())
    {
        m_data.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(m_data.data(), m_data.size(), &client_address);
        QString strclient_address = client_address.toString();
        deliverInfo(m_data, strclient_address);
        qDebug() << "receive UDP's size:" << m_data.size();
    }
}
