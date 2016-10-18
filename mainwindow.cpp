#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow *pMain;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    configDialog(0)
{
    ui->setupUi(this);
    pMain=this;
    LocalDBInit();
    configDialog=new ConfigDialog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConfig_triggered()
{
    if(configDialog==NULL)
    {
        configDialog=new ConfigDialog();
    }
    configDialog->show();
}

void MainWindow::LocalDBInit()
{
    try
    {
        QSqlDatabase::removeDatabase("LocalDB");
        LocalDB=QSqlDatabase::addDatabase("QSQLITE","LocalDB");
        LocalDB.setDatabaseName("Config.db");

        if(!LocalDB.open())
        {
            qDebug()<<"DB is not open.";
            return;
        }

        QSqlQuery LocalDBQuery(LocalDB);

        LocalDBQuery.exec("create table if not exists general_setting (dbip text,dbport text,dbname text,tablename text,username text,password text,language int,autostart int);");
        LocalDBQuery.exec("insert into general_setting(dbip,dbport,dbname,tablename,username,password,language,autostart) select '127.0.0.1','1433','chunil','shot_data','cms','cms',0,0 where not exists (select * from general_setting);");
        qDebug()<<LocalDBQuery.lastError();
        LocalDBQuery.exec("create table if not exists machine_setting (machinename text,directorypath text);");
    }
    catch(QException &e)
    {
        qDebug()<<e.what();
    }
}
