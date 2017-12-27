#ifndef CONTROL_H
#define CONTROL_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QDebug>
#include <QTime>

#include <sstream>
#include <vector>
#include <iostream>
#include "ui_control.h"

namespace Ui {
class Control;
}

class Control : public QWidget
{
    Q_OBJECT

public:
    explicit Control(QWidget *parent = nullptr);
    void Open(int argc, char *argv[], QSerialPort *port);
    void run();
    ~Control() override;

private slots:

    void on_btnOn_clicked();
    void on_btnOff_clicked();
    void on_btnBlink_clicked();

    //beta
    void activateConsole();
    void desactivateConsole();


private:
    Ui::Control *ui;
    int baudios;
    QSerialPort *port;
    QString namePort;
    int argc;
    char **argv;

    //Test
    bool stop;
    bool stopBlink;
    bool guiActivate;

    //Methods
    void searchBaude();
    void searchPort();
    int searchGui();
    void setConnection();
    bool isAvailable();
    void delay(int millisecondsToWait );
    void runConsole();
    int mainConsole();
    bool searchHelp();
    void helpMain();

    //Enun Baude
    enum BAUDE{
     B_300 = 300,
     B_600 = 600,
     B_1200 = 1200,
     B_2400 = 2400,
     B_4800 = 4800,
     B_9600 = 9600,
     B_14400 = 14400,
     B_19200 = 19200,
     B_28800 = 28800,
     B_38400 = 38400,
     B_57600 = 57600,
     B_115200 = 115200,
     BAUDE_END
   };

};

#endif // CONTROL_H
