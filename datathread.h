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
#include <fstream>

using namespace std;


class dataThread: public QThread{

    Q_OBJECT

public:

    inline bool fileExists (const std::string& name) {
        if (FILE *file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
        } else {
            return false;
        }
    }

    dataThread();
    void connectToDB();
    void append2Log(QString str);
    void closeLogFile();
    void insertToDB(QString str);

    ~dataThread();

    void stop();

    ofstream logFile;
    QString logFileName = "./readmeter.log";
    bool dbRecordEnable = false;
    bool dbConnectionOK = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QString tableNames[13] = {"zones", "oto", "sln", "blk", "mut", "eyo", "cyo", "yod", "tempout", "boiler_status", "ot_status", "boiler_temp", "boiler_settemp"};

    bool cmdRecordData = false;


public slots:

    void setNames();


protected:

    void run();


private:

    volatile bool stopped;

signals:
    void dbConnected();

};
#endif // DATATHREAD_H
