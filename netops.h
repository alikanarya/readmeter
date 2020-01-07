#ifndef NETOPS_H
#define NETOPS_H

#define RequestID   "RequestID"

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
#include <QDir>

using namespace std;

extern QString urlCapWithFlash;
extern QString urlCapWithOutFlash;
extern QString RequestUrl[];
extern QString webDir;
extern QString webUrl;
extern char dirName[];
extern char dirName1[];
extern char dirName2[];
extern char dirNameF[];
extern char fileName[];

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
