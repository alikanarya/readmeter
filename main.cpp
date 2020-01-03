#include <QCoreApplication>
#include <iostream>
#include "netops.h"
#include "datathread.h"

using namespace std;

QString hostName = "1ocalhost";
QString dbName = "homeAutoDB";
QString dbUser = "root";
QString dbPass = "reyhan";
bool firstRun = true;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    printf("Hello World !!!");
    cout << endl;

    dataThread *dataX = new dataThread();
    dataX->dbRecordEnable = true;
    dataX->connectToDB();

//    netOps _net("http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/capture.jpg&single");
    netOps _net("http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/meter0.jpg&single");
    _net.makeRequest(0);

    return a.exec();
}
