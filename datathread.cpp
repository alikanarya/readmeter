#include "datathread.h"
#include <time.h>

#define MAX 64

extern QString hostName;
extern QString dbName;
extern QString dbUser;
extern QString dbPass;
extern bool firstRun;

time_t firstTime, prevTime, currentTime;
struct tm *prevTimeInfo, *currentTimeInfo = new tm();
int timeDiff = 0, timeTotal = 0;
char line[MAX];
char dateInfo[MAX];
char timeInfo[MAX];

void timeString(){
    sprintf (line, "%02d/%02d/%02d, %02d:%02d:%02d, ", 	currentTimeInfo->tm_mday,
                                                (currentTimeInfo->tm_mon+1),
                                                (currentTimeInfo->tm_year-100),
                                                currentTimeInfo->tm_hour,
                                                currentTimeInfo->tm_min,
                                                currentTimeInfo->tm_sec);
    sprintf (dateInfo, "%02d/%02d/%02d", currentTimeInfo->tm_mday, (currentTimeInfo->tm_mon+1), (currentTimeInfo->tm_year-100));
    sprintf (timeInfo, "%02d:%02d:%02d", currentTimeInfo->tm_hour, currentTimeInfo->tm_min, currentTimeInfo->tm_sec);
}

dataThread::dataThread(){

    time (&currentTime);
    currentTimeInfo = localtime (&currentTime);
    firstTime = prevTime = currentTime;
    timeString();

}

dataThread::~dataThread(){
}


void dataThread::connectToDB(){

    db.setHostName(hostName);
    db.setDatabaseName(dbName);
    db.setUserName(dbUser);
    db.setPassword(dbPass);

    if (!db.open()) {

        qDebug() <<  db.lastError().text();

    } else {

        qDebug() <<  "db connection established";
        dbConnectionOK = true;
        emit dbConnected();
/*
        QSqlQuery qry;
        QString cmd;

        cmd = QString( "INSERT INTO %1 (date, time, oto, sln, blk, mut, eyo, cyo, yod) VALUES ('%2', '%3', '*', '*', '*', '*', '*', '*', '*')").arg(tableNames[0]).arg(dateInfo).arg(timeInfo);

        qry.prepare( cmd );

        if( !qry.exec() )   qDebug() << qry.lastError();
*/
/*
        for (int i = 0; i < gpioX->dInpNum; i++) {

            cmd = QString( "INSERT INTO %1 (date, time, state) VALUES ('%2', '%3', '*')" ).arg(tableNames[i+1]).arg(dateInfo).arg(timeInfo);

            qry.prepare( cmd );

            if( !qry.exec() )   qDebug() << qry.lastError();

        }
*/
/*
        cmd = QString( "INSERT INTO %1 (date, time, temp) VALUES ('%2', '%3', -99)" ).arg(tableNames[8]).arg(dateInfo).arg(timeInfo);
        //qDebug() << cmd.toUtf8().constData();

        qry.prepare( cmd );

        if( !qry.exec() )   qDebug() << qry.lastError();
*/
    }
}

void dataThread::run(){

    //if (!stopped){    }
    //stopped = false;

    if (cmdRecordData) {
        recordData();
        cmdRecordData = false;
    }

}

void dataThread::stop(){

    stopped = true;
}

void dataThread::recordData(){

    time (&currentTime);
    currentTimeInfo = localtime (&currentTime);
    //timeDiff = difftime(currentTime, prevTime);
    //timeTotal = difftime(currentTime, firstTime);
    //prevTime = currentTime;
    timeString();

    //cout << line;
    cout << dateInfo << " " << timeInfo << "  ";

    //cout << gpioX->aInpArr[0] << " " << gpioX->aInpArr[1] << " " << gpioDS18B20X->sensor1val;

    cout << endl;
/*
    if (dbRecordEnable && db.open()) {

        QSqlQuery qry;
        QString cmd;


//        cmd = QString( "INSERT INTO %1 (date, time, oto, sln, blk, mut, eyo, cyo, yod) VALUES ('%2', '%3', %4, %5, %6, %7, %8, %9, %10)").arg(tableNames[0]).arg(dateInfo).arg(timeInfo).arg(gpioX->dInpArr[0]).arg(gpioX->dInpArr[1]).arg(gpioX->dInpArr[2]).arg(gpioX->dInpArr[3]).arg(gpioX->dInpArr[4]).arg(gpioX->dInpArr[5]).arg(gpioX->dInpArr[6]);
        //qDebug() << cmd.toUtf8().constData();

        qry.prepare( cmd );

        if( !qry.exec() )
          qDebug() << qry.lastError();


        for (int i = 0; i < gpioX->dInpNum; i++) {

            if ( (gpioX->dInpArr[i] != gpioX->dInpArrPrev[i]) || firstRun ) {

                cmd = QString( "INSERT INTO %1 (date, time, state) VALUES ('%2', '%3', %4)").arg(tableNames[i+1]).arg(dateInfo).arg(timeInfo).arg(gpioX->dInpArr[i]);
                //qDebug() << cmd.toUtf8().constData();

                qry.prepare( cmd );

                if( !qry.exec() )
                  qDebug() << qry.lastError();
            }
        }


    }
*/
    firstRun = false;
}

