#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CReceive.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool Parse(QByteArray info);
private slots:
    void on_pushButton_clicked();
    void ShowMessage(QByteArray info, QString clientip);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    CReceive *m_pServer;

    QStringList GetIPAddress();
};

#endif // MAINWINDOW_H
