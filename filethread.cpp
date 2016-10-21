#include "filethread.h"

FileThread::FileThread(QThread *parent) : QObject(parent)
{

}

FileThread::~FileThread()
{
    qDebug()<<"Thread End";
}

void FileThread::run()
{

}
