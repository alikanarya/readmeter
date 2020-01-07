#include "netops.h"

netOps::netOps(QObject *parent) : QObject(parent)
{

}

netOps::netOps (QString _url) {
    url.setUrl(_url);
    Q_ASSERT(&manager);
    connect(&manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(downloadFinished(QNetworkReply*)));
}

netOps::~netOps() { cout << " done " << endl; }

void netOps::makeRequest(unsigned int id) {
    url.setUrl(RequestUrl[id]);
    QNetworkRequest request(url);
    request.setRawHeader("RequestID", QString::number(id).toUtf8());
    manager.get(request);
}

void netOps::downloadFinished(QNetworkReply *reply) {

    if (reply->error()) {
        cout << " Error: downloadFinished() " << reply->errorString().constData() << endl;
    } else {

        QByteArray datagram; datagram.clear();
        while (reply->bytesAvailable() > 0) {
            datagram.append(reply->readAll());
            cout << " ok " << endl;
        }
        int datagramSize = datagram.size();
        printf("Datagram size : %u", datagramSize); cout << endl;
        int _requestMode = reply->request().rawHeader(RequestID).toInt();
        cout << reply->request().rawHeader(RequestID).constData() << endl;

        if (_requestMode==0 || _requestMode==1 || _requestMode==5) {
            QImage *temp = new QImage;
            temp->loadFromData(datagram);
            if (temp->save(webDir + "ngmeter.jpeg")) {
                //makeRequest(2);
                //makeRequest(6);
                QDir().mkdir(dirName1);
                QDir().mkdir(dirName2);
                QDir().mkdir(dirNameF);
                temp->save(QString(fileName));
            }

        } else {
            cout << " data: " << QString::fromUtf8(datagram).toUtf8().constData() << endl;

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
