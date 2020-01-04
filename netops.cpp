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

void netOps::captureImage(bool wFlash) {

   if (wFlash) url.setUrl(urlCapWithFlash);
   else url.setUrl(urlCapWithOutFlash);

   QNetworkRequest request(url);
   manager.get(request);

}

void netOps::downloadFinished(QNetworkReply *reply) {

    if (reply->error()) {
        cout << " downloadFinished() " << reply->errorString().constData() << endl;
    } else {


        QByteArray datagram; datagram.clear();
        //while (client->waitForReadyRead(300)) {
        //cout << reply->bytesAvailable() << "--";
        while (reply->bytesAvailable() > 0) {
            datagram.append(reply->readAll());
            cout << " ok " << endl;
        }
        //cout << " data: " << QString::fromUtf8(datagram).toUtf8().constData() << endl;
        printf(" size : %u", datagram.size());
        cout << endl;
        //manager.disconnect();
        //reply->deleteLater();

        QImage *temp = new QImage;
        temp->loadFromData(datagram);
/*
        while (reply->bytesAvailable() > 0) {
            temp->loadFromData(reply->readAll());
            cout << " ok " << endl;
        }
        */
        //temp->save("test.jpg");
        //cout << endl;
        //QImage image;
        /*
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        temp->save(&buffer, "JPG"); // writes image into ba in PNG format
        */

        QImageWriter writer("test.jpeg");
        writer.write(*temp);
        if(writer.canWrite())
            qDebug("i can write");
        else
            qDebug("i can't write");
        cout << writer.errorString().toUtf8().constData() << endl;
        //cout << QImageWriter::supportedImageFormats() << endl;
    }
}
