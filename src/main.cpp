#include "Control"
#include <QApplication>
#include <QtSerialPort>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Control w;
    QSerialPort *myPort = new QSerialPort;

    w.Open(argc, argv, myPort);
    w.run();

    return a.exec();
}
