#ifndef NETOPS_H
#define NETOPS_H

#define RequestID   "RequestID"
#define RQ_CAM_CAPTURE  0       // capturing image from cam (or a web server)
#define RQ_OCR          1       // ocr analising of the image

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
#include <QProcess>
#include "datathread.h"

using namespace std;

extern dataThread *dataX;
extern QString urlOCREngine;
extern QString urlOCRSource;

extern QString urlCapWithFlash;
extern QString urlCapWithOutFlash;
extern QString webDir;
extern char dirName[];
extern char dirName1[];
extern char dirName2[];
extern char dirNameF[];
extern char fileName[];
extern char fileNameBare[];
extern bool camNgLive;
extern bool ocrEngineRunning;
extern bool localWebServerRunning;
extern bool analyseActive;

class netOps : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;

public:
    QUrl url;
    QString urlOCR = "";
    int requestMode = 0;

    explicit netOps(QObject *parent = 0);
    netOps(QString _url);
    ~netOps();
    void makeRequest(QString _url, unsigned int id);
    bool checkHost(QString ip);

signals:

public slots:
    void downloadFinished(QNetworkReply *reply);

};

#endif // NETOPS_H
