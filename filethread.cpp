#include "filethread.h"

FileThread::FileThread(QObject *parent) : QThread(parent)
{

}

FileThread::~FileThread()
{
    qDebug()<<"Thread End";
}

void FileThread::run()
{

}
