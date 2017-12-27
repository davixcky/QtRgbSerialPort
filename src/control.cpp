#include "control.h"

Control::Control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Control)
{
    ui->setupUi(this);
    //connect(m_pButton, SIGNAL (released()),this, SLOT (handleButton()));
    //connect(ui->btnOn, SIGNAL(clicked(bool)), this, SLOT(changeName()));
    connect(ui->btnStopConsole, SIGNAL(clicked(bool)), this, SLOT(desactivateConsole()));
    connect(ui->btnConsole, SIGNAL(clicked(bool)), this, SLOT(activateConsole()));

    this->baudios = 9600;
    this->stop = false;
    this->guiActivate = false;

}

void Control::Open(int argc, char *_argv[], QSerialPort *port){
    this->port = port;
    this->argc = argc;
    argv = _argv;

    auto aux = searchHelp();

    if(!aux){
        searchBaude();
        searchPort();
        setConnection();
    }

    ui->btnOn->setEnabled(true);
    show();

}

void Control::run(){
    if(isAvailable()){
        qDebug() << "Port " << port->portName() << " is available ";
        qDebug() << "Baude " << port->baudRate();
    }

    auto aux = searchGui();

    if(aux == 1){
        ui->btnStopConsole->setEnabled(false);
        ui->btnStopConsole->setVisible(false);
        show();
        guiActivate = true;
    }else if(aux == 0 || !guiActivate){
        hide();
        guiActivate = false;
        runConsole();
    }

}

Control::~Control()
{
    delete ui;
}

void Control::searchBaude(){
    int argAux;
    BAUDE arBaude[] = { B_300,B_600,B_1200,B_2400,B_4800,B_9600,B_14400,B_19200,B_28800,B_38400,B_57600,B_115200 };
    auto itBaude;

    for(int i=0; i<argc; i++){
        if( !strcmp(argv[i],"-b") ){
            for (auto &j : arBaude) {
                itBaude = j;
                std::istringstream(argv[i+1]) >> argAux;
                if( itBaude == argAux){
                    Control::baudios = itBaude;
                }
            }
        }
    }
}

void Control::searchPort(){
    std::vector<QString> v;
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        v.push_back(serialPortInfo.portName());
    }

    for(auto i=0; i<argc; i++){
        if(!strcmp(argv[i],"-p")){
            auto aux = argv[i+1];
            for (auto &j : v) {
                if(j == aux){
                    Control::namePort = aux;
                    break;
                }
            }
        }
    }
}

int Control::searchGui(){
    for(int i=0; i<argc; i++){
        if(!strcmp(argv[i],"-g")){
            if( (!strcmp(argv[i+1],"1")) || (!strcmp(argv[i+1]," ")) ){
                return 1;
                break;
            }else if(!strcmp(argv[i+1],"0")){
                return 0;
                break;
            }
        }
    }

    return -1;
}

bool Control::searchHelp(){
    for(int i=0; i<argc; i++){
        if( !strcmp(argv[i],"-h") ){
            helpMain();
            return true;
            break;
        }
    }
}

void Control::helpMain(){
    /*usage:  pacman <operation> [...]
operations:
    pacman {-h --help}
    pacman {-V --version}
    pacman {-D --database} <options> <package(s)>
    pacman {-F --files}    [options] [package(s)]
    pacman {-Q --query}    [options] [package(s)]
    pacman {-R --remove}   [options] <package(s)>
    pacman {-S --sync}     [options] [package(s)]
    pacman {-T --deptest}  [options] [package(s)]
    pacman {-U --upgrade}  [options] <file(s)>

use 'pacman {-h --help}' with an operation for available options
*/

    std::cout<<"usage: control <operation> [...]\n";
    std::cout<<"operations: \n";
    std::cout<<"  control {-h --help}\n";
    std::cout<<"  control {-p --port}     [options] \n";
    std::cout<<"  control {-b --baude}    [options] \n";
    std::cout<<"  control {-g --gui}      [options] \n";
}

void Control::setConnection(){
    port->setBaudRate(Control::baudios);
    port->setPortName(Control::namePort);
    port->open(QSerialPort::ReadWrite);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setStopBits(QSerialPort::OneStop);
}

bool Control::isAvailable(){
    return port->isOpen() && port->isWritable();
}

void Control::delay(int millisecondsToWait ){

    auto dieTime = QTime::currentTime().addMSecs(millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void Control::runConsole(){
    while(true){
        auto aux = mainConsole();
        if(aux == 1){
            port->write("1");
        }else if(aux == 2){
            port->write("0");
        }else if(aux == 0){

        }

        delay(5000);

        if(guiActivate){
            if(stop){
                break;
            }
        }
    }

}

int Control::mainConsole(){
    int opt;
    do{
        std::cout << "\nMain: "
                  << "\n1. On "
                  << "\n2. Off"
                  << "\n0. Exit"
                  << "\n> ";
        std::cin  >> opt;
    }while((opt != 1) && (opt != 2) && (opt != 0));
    return opt;
}

void Control::on_btnOn_clicked(){
    if(isAvailable()){
        port->write("1");
        stopBlink = false;
    }

}

void Control::on_btnOff_clicked(){
    if(port->isWritable()){
        port->write("0");
        stopBlink = false;
    }else{
        qDebug() << "Port " << port->portName() << " isnt available ";
    }

}

void Control::activateConsole(){

    ui->btnOff->setVisible(false);
    ui->btnOn->setVisible(false);
    ui->btnBlink->setVisible(false);
    ui->btnConsole->setVisible(false);
    ui->btnStopConsole->setVisible(true);
    ui->btnStopConsole->setEnabled(true);

    stop = false;

    runConsole();
}

void Control::desactivateConsole(){         //stop activate

    ui->btnOff->setVisible(true);
    ui->btnOn->setVisible(true);
    ui->btnBlink->setVisible(true);
    ui->btnConsole->setVisible(true);
    ui->btnConsole->setEnabled(true);
    ui->btnStopConsole->setVisible(false);
    ui->btnStopConsole->setEnabled(false);

    stop = true;

}

void Control::on_btnBlink_clicked(){
    stopBlink = true;

    while(stopBlink){
        port->write("1");
        delay(1000);
        port->write("0");
        delay(1000);
    }
}



