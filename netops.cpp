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
        //QString DataAsString = QTextCodec::codecForMib(106)->toUnicode(content);
        //cout << DataAsString.constData();
        //cout << QString(content).toUtf8().constData();
        cout <<  reply->readAll().toStdString() << reply->isFinished();
        manager.disconnect();
        reply->deleteLater();
    }
}
