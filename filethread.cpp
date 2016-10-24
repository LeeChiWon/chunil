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

void FileThread::FileRead()
{
    QString Text;
    QStringList Context;
    QFile File(FilePath+"\\chunil.txt");
    if(!File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream ReadStream(&File);

    while(!ReadStream.atEnd())
    {
        Text=ReadStream.readLine();
        Context=Text.split(",");
    }

    if(Context.count()>0)
    {
         DBSave(Context);
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

void FileThread::DBSave(QStringList Context)
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

        RemoteDBQuery.exec(QString("insert into Shot_data(Machine_Name,Additional_Info_1,Additional_Info_2,TimeStamp) values('%1','%2','%3','%4')")
                           .arg(MachineName,Context.at(0),Context.at(1),QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        qDebug()<<RemoteDBQuery.lastError();
    }
    catch(QException &e)
    {
        qDebug()<<e.what();
    }
}

void FileThread::Timeout()
{
    QLabel *Label;
    QFileInfo FileInfo(FilePath+"\\chunil.txt");

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
        FileRead();
        FileModifyDateTime=FileInfo.lastModified();
    }
}

void FileThread::run()
{
    ThreadTimer->start(1000);
    exec();
}
