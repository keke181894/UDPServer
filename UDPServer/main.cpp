#include "mainwindow.h"
#include <QApplication>

//void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //qInstallMessageHandler(outputMessage);

    MainWindow w;
    w.show();

    return a.exec();
}
/*
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("[Debug   ]");
        break;
    case QtWarningMsg:
        text = QString("[Warning ]");
        break;
    case QtCriticalMsg:
        text = QString("[Critical]");
        break;
    case QtFatalMsg:
        text = QString("[Fatal   ]");
    }


    QString context_info = QString("(%1:%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(current_date).arg(text).arg(msg).arg(context_info);

    QFile file("log.txt");
    if(file.exists())
    {
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << message << "\r\n";
        file.flush();
        file.close();
    }

    mutex.unlock();
}
*/
