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
#include <QImageWriter>

using namespace std;

extern QString urlCapWithFlash;
extern QString urlCapWithOutFlash;

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
    void captureImage(bool wFlash = true);

signals:

public slots:
    void downloadFinished(QNetworkReply *reply);

};

#endif // NETOPS_H
