#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QObject>

class FileThread : public QObject
{
    Q_OBJECT
public:
    explicit FileThread(QObject *parent = 0);

signals:

public slots:
};

#endif // FILETHREAD_H