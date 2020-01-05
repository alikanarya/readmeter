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
const int RequestUrlSize = 7;
QString RequestUrl[7] = {
    /*0*/   "http://192.168.2.49/capture_with_flashlight",
    /*1*/   "http://192.168.2.49/capture",
    /*2*/   "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.46:8080/ngmeter.jpeg&single",
    /*3*/   "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.49/capture_with_flashlight&single",
    /*4*/   "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.49/capture&single",
    /*5*/   "http://192.168.2.10/meter/capture.jpg",
    /*6*/   "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/capture.jpg&single"
    };
QString webDir = "/var/www/html/";
QString webUrl = "http://192.168.2.46:8080/";
int requestMode = 0;

int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);
    //printf("Hello World !!!");cout << endl;

    if (argc == 1){

        requestMode = 0;

    } else if (argc == 2){

        if ( std::string(argv[1]) == "-qws" )
            requestMode = 0;
        else {
            QString argument = QString::fromStdString(std::string(argv[1]));
            bool ok = false;
            requestMode = argument.toInt(&ok, 10);

            if (!ok) {
                cout << "wrong argument!" << endl;
                return 0;
            } else if (requestMode < 0 || requestMode > (RequestUrlSize-1)) {
                cout << "wrong request mode!" << endl;
                return 0;
            } else {
                cout << RequestUrl[requestMode].toUtf8().constData() << endl;
            }
        }

    };


    dataThread *dataX = new dataThread();
    dataX->dbRecordEnable = true;
    dataX->connectToDB();

//    netOps _net("http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/capture.jpg&single");
//    netOps _net("http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/meter0.jpg&single");
    netOps _net("http://192.168.2.10/meter/meter0.jpg");
    _net.makeRequest(requestMode);

    return a.exec();
}
