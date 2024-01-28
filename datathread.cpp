#include "datathread.h"
#include <time.h>

#define MAX 64

extern QString hostName;
extern QString dbName;
extern QString dbUser;
extern QString dbPass;
extern QString tableName;
extern bool firstRun;

time_t firstTime, prevTime, currentTime;
struct tm *prevTimeInfo, *currentTimeInfo = new tm();
int timeDiff = 0, timeTotal = 0;
char line[MAX];
char dateInfo[MAX];
char timeInfo[MAX];
char dirName[MAX];
char dirName1[MAX];
char dirName2[MAX];
char dirNameF[MAX];
char fileName[MAX];
char fileNameBare[MAX];

void timeString(){
    sprintf (line, "%02d/%02d/%02d, %02d:%02d:%02d, ", 	currentTimeInfo->tm_mday,
                                                (currentTimeInfo->tm_mon+1),
                                                (currentTimeInfo->tm_year-100),
                                                currentTimeInfo->tm_hour,
                                                currentTimeInfo->tm_min,
                                                currentTimeInfo->tm_sec);
    sprintf (dateInfo, "%02d/%02d/%02d", currentTimeInfo->tm_mday, (currentTimeInfo->tm_mon+1), (currentTimeInfo->tm_year-100));
    sprintf (timeInfo, "%02d:%02d:%02d", currentTimeInfo->tm_hour, currentTimeInfo->tm_min, currentTimeInfo->tm_sec);
    sprintf (dirName, "%04d%02d%02d", (currentTimeInfo->tm_year+1900), (currentTimeInfo->tm_mon+1), currentTimeInfo->tm_mday);
//    sprintf (dirNameF, "/ngmeter-data/%04d%02d%02d", (currentTimeInfo->tm_year+1900), (currentTimeInfo->tm_mon+1), currentTimeInfo->tm_mday);
    sprintf (dirName1, "/ngmeter-data");
    sprintf (dirName2, "/ngmeter-data/%04d-%02d", (currentTimeInfo->tm_year+1900), (currentTimeInfo->tm_mon+1));
    sprintf (dirNameF, "/ngmeter-data/%04d-%02d/%02d", (currentTimeInfo->tm_year+1900), (currentTimeInfo->tm_mon+1), currentTimeInfo->tm_mday);
    sprintf (fileName, "%s/%s_%02d%02d%02d.jpeg", dirNameF, dirName, currentTimeInfo->tm_hour, currentTimeInfo->tm_min, currentTimeInfo->tm_sec);
    sprintf (fileNameBare, "%s_%02d%02d%02d.jpeg", dirName, currentTimeInfo->tm_hour, currentTimeInfo->tm_min, currentTimeInfo->tm_sec);
}

dataThread::dataThread(){

    time (&currentTime);
    currentTimeInfo = localtime (&currentTime);
    firstTime = prevTime = currentTime;
    timeString();

    if ( !fileExists( logFileName.toUtf8().constData() ) ){

        logFile.open( logFileName.toUtf8().constData(), ios::out | ios::app );

        if (logFile.is_open()){
            logFile << line << "app started" << endl;
        }
    } else {

        logFile.open( logFileName.toUtf8().constData(), ios::out | ios::app );

        /*if (logFile.is_open()){
            logFile << fileSeperators[i].toUtf8().constData() << endl;
        }*/
    }

}

dataThread::~dataThread(){
}


void dataThread::connectToDB(){

    db.setHostName(hostName);
    db.setDatabaseName(dbName);
    db.setUserName(dbUser);
    db.setPassword(dbPass);

    if (!db.open()) {

        append2Log("can not connected to db");
        qDebug() <<  db.lastError().text();

    } else {

        qDebug() <<  "db connection established";
        dbConnectionOK = true;
        emit dbConnected();
    }
}

void dataThread::append2Log(QString str){
    if (logFile.is_open()){
        logFile << dateInfo << " " << timeInfo << " " << str.toUtf8().constData() << endl;
    }
}

void dataThread::closeLogFile(){
    if (logFile.is_open()){
        logFile.close();
    }

}

void dataThread::run(){

    //if (!stopped){    }
    //stopped = false;

    if (cmdRecordData) {
        setNames();
        cmdRecordData = false;
    }

}

void dataThread::stop(){

    stopped = true;
}

void dataThread::setNames(){

    time (&currentTime);
    currentTimeInfo = localtime (&currentTime);
    //timeDiff = difftime(currentTime, prevTime);
    //timeTotal = difftime(currentTime, firstTime);
    //prevTime = currentTime;
    timeString();

    //cout << line;
    qDebug() << dateInfo << " " << timeInfo;
    qDebug() << dirName << " " << fileName;

    firstRun = false;
}

void dataThread::insertToDB(QString str){

    if (db.open()) {
        bool ok;
        str.toFloat(&ok);
        QString _ocr = str;
        //qDebug() << "float? " << ok;
        if (!ok)
            str = "0";
        QString qryStr = "";
        qryStr = QString( "INSERT INTO %1 (date, time, value, ocr) VALUES ('%2', '%3', %4, '%5')").arg(tableName).arg(dateInfo).arg(timeInfo).arg(str).arg(_ocr);
        //qryStr = QString( "INSERT INTO gas_reading (date, time, value, note) VALUES ('%1', '%2', %3, '%4')").arg(date).arg(time).arg(result).arg(FIXED);

        //qDebug() << qryStr.toUtf8().constData();

        QSqlQuery qry;
        qry.prepare( qryStr );

        if( !qry.exec() ){
            append2Log("can not insterted to db");
            qDebug() << qry.lastError().type();
            qDebug() << qry.lastError().databaseText();
        }
        else {
            qDebug( "Inserted!" );
        }
    } else {
        append2Log("not connected to db");
    }
}

