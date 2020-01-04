#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>

using namespace std;


class dataThread: public QThread{

    Q_OBJECT

public:

    dataThread();
    void connectToDB();

    ~dataThread();

    void stop();

    bool dbRecordEnable = false;
    bool dbConnectionOK = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QString tableNames[13] = {"zones", "oto", "sln", "blk", "mut", "eyo", "cyo", "yod", "tempout", "boiler_status", "ot_status", "boiler_temp", "boiler_settemp"};

    bool cmdRecordData = false;


public slots:

    void recordData();


protected:

    void run();


private:

    volatile bool stopped;

signals:
    void dbConnected();

};
#endif // DATATHREAD_H
