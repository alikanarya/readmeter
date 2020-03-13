/*
 * jpeg support:
 * compile /qt-everywhere-opensource-src-5.7.0/qtbase/src/plugins/imageformats/jpeg/jpeg.pro with BBB(arm) kit
 * it will put libqjpeg.so to
 * qt-everywhere-opensource-src-5.7.0/qtbase/plugins/imageformats/ in host and
 * /usr/local/qt-armhf/plugins/imageformats/ in BBB
 *
 */

#include <QCoreApplication>
#include <iostream>
#include "netops.h"
#include "datathread.h"

using namespace std;

#define INIFILENAME         "readmeter.ini"

QString hostName = "localhost";
QString dbName = "homeAutoDB";
QString dbUser = "ali";
QString dbPass = "reyhan";
dataThread *dataX;
bool firstRun = true;
const int RequestUrlSize = 9;
QString RequestUrl[9] = {
    /*0*/   "http://192.168.2.49/capture_with_flashlight",
    /*1*/   "http://192.168.2.49/capture",
    /*2*/   "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.48:8080/ngmeter.jpeg&single",
    /*3*/   "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.49/capture_with_flashlight&single",
    /*4*/   "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.49/capture&single",
    /*5*/   "http://192.168.2.10/meter/capture.jpg",
    /*6*/   "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/capture.jpg&single",
    /*7*/   "http://192.168.2.46:8080/ngmeter.jpeg",
    /*8*/   "http://192.168.2.46:8000/analyse.html?url=http://192.168.2.46:8080/ngmeter.jpeg"
    };
QString webDir = "/var/www/html/";
QString webUrl = "http://192.168.2.48:8080/";
int requestMode = 0;

QString ipCamNg = "192.168.2.49";
QString ipOCrEngine = "192.168.2.11";

bool dockerHostLive = false;
bool camNgLive = false;
bool dockerRunning = false;
bool localWebServerRunning = false;
bool analyseActive = false;

bool readSettings(){

    if (QFile::exists(INIFILENAME)){

        ipCamNg = settings->value("ipCamNg", _CLIENT_ADR).toString();
        clientPort = settings->value("clientPort", _CLIENT_PORT).toInt();
        dbName = settings->value("dbName", _DB_NAME).toString();
        dbUser = settings->value("dbUser", _DB_USER).toString();
        dbPass = settings->value("dbPass", _DB_PASS).toString();
        ds18b20_SN1 = settings->value("ds18b20_SN1", _DS18B20_SN1).toString();
        server1Address = settings->value("server1Address", _CLIENT_ADR).toString();
        server1Port = settings->value("server1Port", _SERVER1_PORT).toInt();

        //cout << clientAddress.toUtf8().constData() << endl;
        return true;

    } else {
        cout << "ini file not found" << endl;
        return false;

    }
}
int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);
    //printf("Hello World !!!");cout << endl;

    bool _dbRecordEnable = false;

    if (argc == 1){
        if ( std::string(argv[1]) == "ocr" )
            analyseActive = true;
        else if ( !std::string(argv[1]) == "-qws" ) {
            printf("Wrong argument! Usage: readmeter <ocr> <db>");cout << endl;
            qApp->quit();
        }
    } else if (argc == 2){

        if ( std::string(argv[1]) == "ocr" && std::string(argv[2]) == "db" ) {
            analyseActive = true;
            _dbRecordEnable = true;
        } else if ( std::string(argv[1]) == "-qws" && std::string(argv[2]) == "ocr" )
            analyseActive = true;
        else {
            printf("Wrong argument! Usage: readmeter <ocr> <db>");cout << endl;
            qApp->quit();
        }
        /*else {
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
        }*/
    } else if (argc == 3){
        if ( std::string(argv[1]) == "-qws" && std::string(argv[2]) == "ocr" && std::string(argv[3]) == "db" ) {
            analyseActive = true;
            _dbRecordEnable = true;
        } else {
            printf("Wrong argument! Usage: readmeter <ocr> <db>");cout << endl;
            qApp->quit();
        }
    } else {
        printf("Wrong argument! Usage: readmeter <ocr> <db>");cout << endl;
        qApp->quit();
    };

    readSettings();

    dataX = new dataThread();
    dataX->dbRecordEnable = _dbRecordEnable;
    if (dataX->dbRecordEnable)
        dataX->connectToDB();
    dataX->setNames();

    netOps _net("");
    std::cout << std::boolalpha;
    //dockerHostLive = _net.checkHost(ipOCrEngine);
    //cout << "DockerHost: " << dockerHostLive << "\n";

    // net request is asynchronous, will be visited later
    //_net.makeRequest(7);    //check local web server is runnning
    //cout << "WebServer: " << localWebServerRunning << "\n";

    camNgLive = _net.checkHost(ipCamNg);
    //cout << "CamNg: " << camNgLive << "\n";
    if (!camNgLive)
        dataX->append2Log("Camera Ng is offline ");

    if (camNgLive)
        _net.makeRequest(requestMode);
    else
        return 0;

    return a.exec();
}
