#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "filethread.h"
#include "configdialog.h"
#include "define.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;

    FileThread *fileThread;
    QSqlDatabase LocalDB;
    ConfigDialog *configDialog;
    QTranslator Translator;
    QMap<QString,QLabel*> TableWidgetLabelMap;
    QMap<QString,QThread*> FileThreadMap;

    void TableWidgetInit();
    void TableWidgetDelete();
    void TableWidgetDB(int Select);

signals:
    void Retranslator();

private slots:
    void on_actionConfig_triggered();
    void LocalDBInit();
    void on_pushButton_Add_clicked();
    void on_pushButton_Delete_clicked();
    void on_pushButton_Path_clicked(); 
    void LanguageChange(int LanguageSelect);
    void TabelWidgetAdd(QString MachineName,QString Path);
    void on_tableWidget_MachineInfo_itemClicked(QTableWidgetItem *item);
    void on_actionKorean_triggered();
    void on_actionEnglish_triggered();
    void on_actionEspanol_triggered();
    void closeEvent(QCloseEvent *event);
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void UpdateCommunity(QString MachineName,QString Context);

    void on_actionExit_triggered();

private:

};

#endif // MAINWINDOW_H
