/*
 * jpeg support:
 * compile /qt-everywhere-opensource-src-5.7.0/qtbase/src/plugins/imageformats/jpeg/jpeg.pro with BBB(arm) kit
 * it will put libqjpeg.so to
 * qt-everywhere-opensource-src-5.7.0/qtbase/plugins/imageformats/ in host and
 * /usr/local/qt-armhf/plugins/imageformats/ in BBB
 */

#include <QCoreApplication>
#include <iostream>
#include "netops.h"
#include "datathread.h"

using namespace std;

QString hostName = "localhost";
QString dbName = "homeAutoDB";
QString dbUser = "ali";
QString dbPass = "reyhan";
bool firstRun = true;
QString urlCapWithFlash = "http://192.168.2.49/capture_with_flashlight";
QString urlCapWithOutFlash = "http://192.168.2.49/capture";

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    printf("Hello World !!!");
    cout << endl;

    dataThread *dataX = new dataThread();
    dataX->dbRecordEnable = true;
    dataX->connectToDB();

//    netOps _net("http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/capture.jpg&single");
//    netOps _net("http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/meter0.jpg&single");
    netOps _net("http://192.168.2.10/meter/meter0.jpg");
    _net.makeRequest(0);

    return a.exec();
}
