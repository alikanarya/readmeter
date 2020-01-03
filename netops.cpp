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
    QNetworkRequest request(url);
    manager.get(request);
}

void netOps::downloadFinished(QNetworkReply *reply) {

    if (reply->error()) {
        cout << " downloadFinished() " << reply->errorString().constData() << endl;
    } else {
        //cout << " ok " << endl;
        //QByteArray content= reply->readAll();
        //cout << QString(content).toUtf8().constData();
        //cout << reply->bytesAvailable();
        //cout << reply->readAll().toStdString() << reply->isFinished();

        QByteArray datagram; datagram.clear();
        //while (client->waitForReadyRead(300)) {
        //cout << reply->bytesAvailable() << "--";
        while (reply->bytesAvailable() > 0) {
            datagram.append(reply->readAll());
            //cout << " ok " << endl;
        }
        cout << " data: " << QString::fromUtf8(datagram).toUtf8().constData() << endl;
        printf(" size : %u", datagram.size());
        cout << endl;
        //cout << " size: " << QString::number(datagram.size()).toUtf8().constData();
        //manager.disconnect();
        //reply->deleteLater();
    }
}
