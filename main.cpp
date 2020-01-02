#include <QCoreApplication>
#include <iostream>
#include "netops.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    printf("Hello World !!!");
    cout << endl;
    netOps _net("http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/capture.jpg&single");
//    netOps _net("http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/meter0.jpg&single");
    _net.makeRequest(0);

    return a.exec();
}
