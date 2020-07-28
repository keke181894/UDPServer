#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSplitter>
#include <QNetworkAddressEntry>
#include "structdef.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pServer = NULL;
    m_pServer = new CReceive(this);
    QStringList ipList = GetIPAddress();
    ui->comboBox->addItems(ipList);

    QSplitter *pSplitter = new QSplitter;
    pSplitter->setOrientation(Qt::Vertical);
    pSplitter->addWidget(ui->groupBox_4);
    pSplitter->addWidget(ui->groupBox_2);
    pSplitter->addWidget(ui->groupBox_3);

    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(ui->groupBox);
    pMainLayout->addWidget(pSplitter);
    this->centralWidget()->setLayout(pMainLayout);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    static bool bStart = false;
    if(bStart) // to do stop
    {
        bStart = false;
        m_pServer->Stop();
        ui->pushButton->setText("Start");
        ui->comboBox->setEnabled(true);
        ui->spinBox->setEnabled(true);

    }
    else // to do start
    {
        bStart = true;
        ui->pushButton->setText("Stop");
        ui->comboBox->setEnabled(false);
        ui->spinBox->setEnabled(false);
        m_pServer->Init(ui->comboBox->currentText(),ui->spinBox->value());
        m_pServer->Start();
        connect(m_pServer,SIGNAL(deliverInfo(QByteArray,QString)),this, SLOT(ShowMessage(QByteArray,QString)));
    }


}

void MainWindow::ShowMessage(QByteArray info, QString clientip)
{
    Parse(info);
    QString strMsg = QString("IP = %1,Length = %2:").arg(clientip).arg(info.size());
    ui->textEdit_2->append(strMsg);
    if(ui->radioButton->isChecked())
    {
        ui->textEdit_2->append(QString::fromLocal8Bit(info));
    }
    else
    {
        strMsg.clear();
        for(int i = 0; i < info.size(); i++)
        {
            strMsg += QString("%1 ").arg((unsigned char)info.at(i),2, 16,QLatin1Char('0'));
            QApplication::processEvents();
        }
        ui->textEdit_2->append(strMsg);
    }
}

bool MainWindow::Parse(QByteArray info)
{
    stNFMsg stMsg;
    bool result = stMsg.decode(info);
    if(result != true)
    {
        ui->textEdit->append("decode failure.");
    }
    return result;
}

QStringList MainWindow::GetIPAddress()
{
    QStringList result;
    foreach (QHostAddress one, QNetworkInterface::allAddresses())
    {
        if(one.protocol() == QAbstractSocket::IPv4Protocol)
        {
            result.append(one.toString());
        }
    }

//    foreach (QNetworkInterface two, QNetworkInterface::allInterfaces())
//    {
//        qDebug() << two.humanReadableName();
//        qDebug() << two.hardwareAddress();
//        qDebug() << two.name();
//        qDebug() << two.flags();

//        if(two.isValid())
//        {
//            qDebug() << "this IP is valid.";
//        }
//        else
//        {
//            qDebug() << "this IP is invalid.";
//        }

//        if(two.flags() == QNetworkInterface::IsUp)
//        {
//            qDebug() << "this IP is Active.";
//        }
//        else
//        {
//            qDebug() << "this IP is inActive.";
//        }

//        foreach (QHostAddress three, two.allAddresses())
//        {
//            if(three.protocol() == QAbstractSocket::IPv4Protocol)
//            {
//                qDebug() << three.toString();
//            }
//        }
//    }
    return result;
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit_2->clear();
}
