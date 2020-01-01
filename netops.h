#ifndef NETOPS_H
#define NETOPS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QImage>
#include <iostream>
#include <QString>
#include <QByteArray>
#include <QTextCodec>

using namespace std;

class netOps : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;

public:
    QUrl url;
    explicit netOps(QObject *parent = 0);
    netOps(QString _url);
    ~netOps();
    void makeRequest(unsigned int id);

signals:

public slots:
    void downloadFinished(QNetworkReply *reply);

};

#endif // NETOPS_H
