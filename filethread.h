#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QObject>
#include <QThread>
#include <QtWidgets>

class FileThread : public QThread
{
    Q_OBJECT
public:
    explicit FileThread(QObject *parent = 0);
    ~FileThread();
signals:

public slots:
protected:
    void run();
};

#endif // FILETHREAD_H
