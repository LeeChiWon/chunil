#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QObject>
#include <QThread>
#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class FileThread : public QThread
{
    Q_OBJECT
public:
    explicit FileThread(QObject *parent = 0);
    ~FileThread();

    QTimer *ThreadTimer;
    QString FilePath;
    QString MachineName;
    bool ExistFile;
    QDateTime FileModifyDateTime;
    QSqlDatabase RemoteDB;

    void FileRead(QString FileName);
    void RemoteDBSet();
    void DBSave(QStringList Context,int Select);
    void FileInfoChanaged(QString FileName);

signals:
    void UpdateCommunity(QString MachineName,QString Context);    
public slots:
    void Timeout();
protected:
    void run();
};

#endif // FILETHREAD_H
