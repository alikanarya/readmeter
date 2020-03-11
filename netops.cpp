#include "netops.h"

netOps::netOps(QObject *parent) : QObject(parent)
{

}

netOps::netOps (QString _url) {
    url.setUrl(_url);
    Q_ASSERT(&manager);
    connect(&manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(downloadFinished(QNetworkReply*)));
}

netOps::~netOps() { qDebug() << " done " << endl; }

void netOps::makeRequest(unsigned int id) {
    requestMode = id;
    url.setUrl(RequestUrl[id]);
    QNetworkRequest request(url);
    request.setRawHeader("RequestID", QString::number(id).toUtf8());
    manager.get(request);
}

void netOps::downloadFinished(QNetworkReply *reply) {

    if (reply->error()) {
        cout << " Error: downloadFinished() " << reply->errorString().constData() << endl;
        if (requestMode==2 || requestMode==3 || requestMode==4 || requestMode==6) {
            dockerRunning = false;
            cout << "Docker: " << dockerRunning << "\n";
            dataX->closeLogFile();
            qApp->quit();
        }
        if (requestMode==7) {
             localWebServerRunning = false;
        }
    } else {

        QByteArray datagram; datagram.clear();
        while (reply->bytesAvailable() > 0) {
            datagram.append(reply->readAll());
        }
        int datagramSize = datagram.size();
        //printf("Datagram size : %u", datagramSize); cout << endl;
        int _requestMode = reply->request().rawHeader(RequestID).toInt();
        //cout << reply->request().rawHeader(RequestID).constData() << endl;

        if (_requestMode==0 || _requestMode==1 || _requestMode==5) {
            QImage *temp = new QImage;
            temp->loadFromData(datagram);

            QDir().mkdir(dirName1);
            QDir().mkdir(dirName2);
            QDir().mkdir(dirNameF);
            if (temp->save(QString(fileName))){
                dataX->append2Log(QString(fileNameBare) + " saved");
            }

            if (temp->save(webDir + "ngmeter.jpeg")) {
                if (analyseActive) {
                    requestMode = 8;    //6;
                    makeRequest(requestMode);
                } else {
                    dataX->closeLogFile();
                    qApp->quit();
                }
            } else {
                dataX->closeLogFile();
                qApp->quit();
            }

        } else if (_requestMode==7){
            localWebServerRunning = true;
        } else if (_requestMode==8){
            //dataX->insertToDB(QString::fromUtf8(datagram).toUtf8().constData().insert(datagramSize-1,"."));
            dataX->insertToDB(QString::fromUtf8(datagram).toUtf8().insert(datagramSize-1,"."));
            qApp->quit();
        } else {
            cout << " data: " << QString::fromUtf8(datagram).toUtf8().constData() << endl;
            dockerRunning = true;
            cout << "Docker: " << dockerRunning << "\n";
            dataX->closeLogFile();
            qApp->quit();
        }



        /*
        QImageWriter writer("test.jpeg");
        writer.write(*temp);
        if(writer.canWrite())
            qDebug("i can write");
        else
            qDebug("i can't write");
        cout << writer.errorString().toUtf8().constData() << endl;
        //cout << QImageWriter::supportedImageFormats() << endl;
        */

        //manager.disconnect();
        reply->deleteLater();
    }
}

bool netOps::checkHost(QString ip){
    QProcess pingProcess;
    QString exec = "ping";
    QStringList params;
    params << "-c" << "1" << ip;
    pingProcess.start(exec,params,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = pingProcess.readAllStandardOutput();
    //QString p_stderr = pingProcess.readAllStandardError();
    return (!p_stdout.contains("errors", Qt::CaseInsensitive));
}

