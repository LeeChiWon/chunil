#include "filethread.h"
#include "mainwindow.h"

extern MainWindow *pMain;

FileThread::FileThread(QObject *parent) : QThread(parent)
  ,ExistFile(false)
{
    ThreadTimer=new QTimer();
    connect(ThreadTimer,SIGNAL(timeout()),this,SLOT(Timeout()),Qt::DirectConnection);
    ThreadTimer->moveToThread(this);
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

    emit UpdateCommunity(MachineName,Text);
    File.close();
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
