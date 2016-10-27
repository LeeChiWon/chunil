#include "filethread.h"
#include "mainwindow.h"

extern MainWindow *pMain;

FileThread::FileThread(QObject *parent) : QThread(parent)
  ,ExistFile(false)
{
    ThreadTimer=new QTimer();
    connect(ThreadTimer,SIGNAL(timeout()),this,SLOT(Timeout()),Qt::DirectConnection);
    ThreadTimer->moveToThread(this);
    RemoteDBSet();
}

FileThread::~FileThread()
{
    ThreadTimer->deleteLater();
}

void FileThread::FileRead(QString FileName)
{
    QString Text;
    QStringList Context;
    QFile File(FilePath+"\\"+FileName);
    int LineCount=0;
    if(!File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream ReadStream(&File);
    while(!ReadStream.atEnd())
    {
        Text=ReadStream.readLine();

        if(FileName=="Cheonil.dat" || FileName=="Cheonilshot.dat")
        {
            if(LineCount==1)
            {
                Context=Text.split(",");
            }
            LineCount++;
        }
        else if(FileName=="Cheonil-Alarms.dat")
        {
            Context.append(Text);
        }
    }

    if(Context.count()>0)
    {
        if(FileName=="Cheonil.dat")
        {
            DBSave(Context,FILENAME_RECIPE_NUMBER);
        }
        else if(FileName=="Cheonil-Alarms.dat")
        {
            DBSave(Context,FILENAME_ALARM_NUMBER);
        }
        else if(FileName=="Cheonilshot.dat")
        {
            DBSave(Context,FILENAME_SHOT_NUMBER);
        }
    }

    emit UpdateCommunity(MachineName,Text);
    File.close();
}

void FileThread::RemoteDBSet()
{
    try
    {
        QSqlDatabase::removeDatabase(QString("RemoteDB-%1").arg(MachineName));
        RemoteDB=QSqlDatabase::addDatabase("QODBC",QString("RemoteDB-%1").arg(MachineName));

        RemoteDB.setDatabaseName(pMain->configDialog->ui->lineEdit_DBName->text());
        RemoteDB.setHostName(pMain->configDialog->ui->lineEdit_DBIP->text());
        RemoteDB.setPort(pMain->configDialog->ui->lineEdit_DBPort->text().toInt());
        RemoteDB.setUserName(pMain->configDialog->ui->lineEdit_UserName->text());
        RemoteDB.setPassword(pMain->configDialog->ui->lineEdit_Password->text());

        if(!RemoteDB.open())
        {
            qDebug()<<RemoteDB.lastError();
            return;
        }
    }
    catch(QException &e)
    {
        qDebug()<<e.what();
    }
}

void FileThread::DBSave(QStringList Context,int Select)
{
    try
    {
        RemoteDB=QSqlDatabase::database(QString("RemoteDB-%1").arg(MachineName));

        if(!RemoteDB.isOpen())
        {
            RemoteDBSet();
            RemoteDB=QSqlDatabase::database(QString("RemoteDB-%1").arg(MachineName));
        }
        QSqlQuery RemoteDBQuery(RemoteDB);

        switch(Select)
        {
        case FILENAME_RECIPE_NUMBER:
            RemoteDBQuery.exec(QString("insert into Shot_data(Machine_Name,Additional_Info_1,Additional_Info_2,TimeStamp) values('%1','%2','%3','%4')")
                               .arg(MachineName,Context.at(0),Context.at(1),QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            break;
        case FILENAME_ALARM_NUMBER:
            RemoteDBQuery.exec(QString("insert into Shot_data(Machine_Name,Additional_Info_1,Additional_Info_2,TimeStamp) values('%1','%2','%3','%4')")
                               .arg(MachineName,Context.at(0),Context.at(1),QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            break;
        case FILENAME_SHOT_NUMBER:
            RemoteDBQuery.exec(QString("insert into Shot_data(Machine_Name,TimeStamp,Shot_Number,Injection_Time,Plasticizing_Time,Cycle_Time,Clamp_Close_Time,"
                                       "Cushion_Position,Switch_Over_Position,Plasticizing_Position,Clamp_Open_Position,Max_Injection_Speed,Max_Injection_Pressure"
                                       ",Max_Switch_Over_Pressure,Max_Back_Pressure) "
                                       "values('%1','%2 %3',%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16)")
                               .arg(MachineName,Context.at(0),Context.at(1))
                               .arg(Context.at(2).toInt())
                               .arg(Context.at(5).toDouble())
                               .arg(Context.at(6).toDouble())
                               .arg(Context.at(7).toDouble())
                               .arg(Context.at(8).toDouble())
                               .arg(Context.at(9).toDouble())
                               .arg(Context.at(10).toDouble())
                               .arg(Context.at(11).toDouble())
                               .arg(Context.at(12).toDouble())
                               .arg(Context.at(13).toDouble())
                               .arg(Context.at(14).toDouble())
                               .arg(Context.at(15).toDouble())
                               .arg(Context.at(16).toDouble()));
            break;
        }
        qDebug()<<RemoteDBQuery.lastError();
    }
    catch(QException &e)
    {
        qDebug()<<e.what();
    }
}

void FileThread::FileInfoChanaged(QString FileName)
{
    QLabel *Label;
    QFileInfo FileInfo(FilePath+"\\"+FileName);

    if(!FileInfo.exists())
    {
        Label=pMain->TableWidgetLabelMap.value(MachineName);
        Label->setText(tr("<p><img src=:/Img/Img/NoFile.png width=30 height=30 align=middle> Not File</p>"));
        ExistFile=false;
        return;
    }

    if(!ExistFile)
    {
        Label=pMain->TableWidgetLabelMap.value(MachineName);
        Label->setText(tr("<p><img src=:/Img/Img/File.png width=30 height=30 align=middle>File exist</p>"));
        ExistFile=true;
    }

    if(FileInfo.lastModified()!=FileModifyDateTime)
    {
        FileRead(FileName);
        FileModifyDateTime=FileInfo.lastModified();
    }
}

void FileThread::Timeout()
{    
    QStringList FileName;
    FileName<<"Cheonil.dat"<<"Cheonil-Alarms.dat"<<"Cheonilshot.dat";

    for(int i=0; i<FileName.count(); i++)
    {
        FileInfoChanaged(FileName.at(i));
    }
}

void FileThread::run()
{
    ThreadTimer->start(1000);
    exec();
}
