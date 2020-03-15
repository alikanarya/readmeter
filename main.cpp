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
#include <QSettings>
#include <QFile>
#include "netops.h"
#include "datathread.h"

using namespace std;

#define INIFILENAME     "readmeter.ini"
#define _ipCamNg        "192.168.2.49"
#define _urlCam         "http://192.168.2.49/capture_with_flashlight"
#define _urlOCREngine   "http://192.168.2.48:8000/analyse.html?url="
#define _urlOCRSource   "http://192.168.2.48:8080/ngmeter.jpeg"
#define _DB_NAME        "homeAutoDB"
#define _DB_USER        "ali"
#define _DB_PASS        "reyhan"
#define _tableName      "gas_reading"

QSettings *settings;    // settings: to read/write ini file

dataThread *dataX;
bool firstRun = true;
QString webDir = "/var/www/html/";

QString ipCamNg = "192.168.2.10";
QString urlCam = "http://192.168.2.10/meter/ngmeter.jpeg";
QString urlOCREngine = "http://192.168.2.46:8000/analyse.html?url=";
QString urlOCRSource = "http://192.168.2.46:8080/ngmeter.jpeg";
QString hostName = "localhost";
QString dbName = "homeAutoDB";
QString dbUser = "ali";
QString dbPass = "reyhan";
QString tableName = "gas_reading2";

bool camNgLive = false;
bool ocrEngineRunning = false;
bool analyseActive = false;


bool readSettings(){

    if (QFile::exists(INIFILENAME)){

        ipCamNg = settings->value("ipCamNg", _ipCamNg).toString();
        urlCam = settings->value("urlCam", _urlCam).toString();
        urlOCREngine = settings->value("urlOCREngine", _urlOCREngine).toString();
        urlOCRSource = settings->value("urlOCRSource", _urlOCRSource).toString();
        dbName = settings->value("dbName", _DB_NAME).toString();
        dbUser = settings->value("dbUser", _DB_USER).toString();
        dbPass = settings->value("dbPass", _DB_PASS).toString();
        tableName = settings->value("tableName", _tableName).toString();

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
    bool cont = true;

    if (argc == 2){

        if ( std::string(argv[1]) == "ocr" )
            analyseActive = true;
        else if ( !(std::string(argv[1]) == "-qws") ) {
            cont = false;
            printf("Wrong argument! Usage: readmeter <ocr> <db>");cout << endl;
            qApp->quit();
        }
    } else if (argc == 3){

        if ( std::string(argv[1]) == "ocr" && std::string(argv[2]) == "db" ) {
            analyseActive = true;
            _dbRecordEnable = true;
        } else if ( std::string(argv[1]) == "-qws" && std::string(argv[2]) == "ocr" )
            analyseActive = true;
        else {
            cont = false;
            printf("Wrong argument! Usage: readmeter <ocr> <db>");cout << endl;
            qApp->quit();
        }
    } else if (argc == 4){

        if ( std::string(argv[1]) == "-qws" && std::string(argv[2]) == "ocr" && std::string(argv[3]) == "db" ) {
            analyseActive = true;
            _dbRecordEnable = true;
        } else {
            cont = false;
            printf("Wrong argument! Usage: readmeter <ocr> <db>");cout << endl;
            qApp->quit();
        }
    } else if (argc > 4) {

        cont = false;
        printf("Wrong argument number! Usage: readmeter <ocr> <db>");cout << endl;
        qApp->quit();
    };

    netOps _net("");
    std::cout << std::boolalpha;

    if (cont) {

        settings = new QSettings(INIFILENAME, QSettings::IniFormat);
        readSettings();
        _net.urlOCR = urlOCREngine + urlOCRSource;

        dataX = new dataThread();
        dataX->dbRecordEnable = _dbRecordEnable;
        if (dataX->dbRecordEnable)
            dataX->connectToDB();
        dataX->setNames();


        // check ocr engine host pc is live if it is on other machine, will be visited later
        //dockerHostLive = _net.checkHost(ipOCrEngine);
        //cout << "DockerHost: " << dockerHostLive << "\n";

        // net request is asynchronous, will be visited later
        //_net.makeRequest(7);    //check local web server is runnning
        //cout << "WebServer: " << localWebServerRunning << "\n";

        camNgLive = _net.checkHost(ipCamNg);
        //cout << "CamNg: " << camNgLive << "\n";
        if (!camNgLive)
            dataX->append2Log("Camera Ng is offline ");
    }

    if (camNgLive && cont)
        _net.makeRequest(urlCam, RQ_CAM_CAPTURE);
    else
        return 0;

    return a.exec();
}


/* OLD codes

const int RequestUrlSize = 9;
QString RequestUrl[9] = {
       "http://192.168.2.49/capture_with_flashlight",
       "http://192.168.2.49/capture",
       "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.48:8080/ngmeter.jpeg&single",
       "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.49/capture_with_flashlight&single",
       "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.49/capture&single",
       "http://192.168.2.10/meter/capture.jpg",
       "http://192.168.2.11:3000/wasserzaehler.html?url=http://192.168.2.10/meter/capture.jpg&single",
       "http://192.168.2.46:8080/ngmeter.jpeg",
       "http://192.168.2.46:8000/analyse.html?url=http://192.168.2.46:8080/ngmeter.jpeg"
    };
int requestMode = 0;

//QString ipOCrEngine = "192.168.2.46";
//bool localWebServerRunning = false;
//QString webUrl = "http://192.168.2.48:8080/";

            // argument checking for number input
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

*/
