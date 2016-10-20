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

void MainWindow::TableWidgetInit()
{
    QStringList CommunityLabel,MachineLabel;
    CommunityLabel<<tr("DateTime")<<tr("MachineName")<<tr("Context");
    MachineLabel<<tr("MachineName")<<tr("DirectoryPath");
    ui->tableWidget_Community->setHorizontalHeaderLabels(CommunityLabel);
    ui->tableWidget_MachineInfo->setHorizontalHeaderLabels(MachineLabel);
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
        LocalDBQuery.exec("create table if not exists machine_setting (machinename text,directorypath text);");
    }
    catch(QException &e)
    {
        qDebug()<<e.what();
    }
}

void MainWindow::on_pushButton_Add_clicked()
{

}

void MainWindow::on_pushButton_Delete_clicked()
{

}

void MainWindow::on_pushButton_Path_clicked()
{

}

void MainWindow::on_actionStart_triggered()
{
    ui->actionStart->setEnabled(false);
    ui->actionStop->setEnabled(true);
    ui->pushButton_Add->setEnabled(false);
    ui->pushButton_Delete->setEnabled(false);
    ui->pushButton_Path->setEnabled(false);
    ui->lineEdit_MachineName->setEnabled(false);
    ui->actionConfig->setEnabled(false);
}

void MainWindow::on_actionStop_triggered()
{
    ui->actionStart->setEnabled(true);
    ui->actionStop->setEnabled(false);
    ui->pushButton_Add->setEnabled(true);
    ui->pushButton_Delete->setEnabled(true);
    ui->pushButton_Path->setEnabled(true);
    ui->lineEdit_MachineName->setEnabled(true);
    ui->actionConfig->setEnabled(true);
}


void MainWindow::LanguageChange(int LanguageSelect)
{
    QStringList LanguageList;
    LanguageList<<":/Language/Lang_ko_KR.qm"<<":/Language/Lang_en_US.qm"<<":/Language/Lang_es_MX.qm";

    QApplication::removeTranslator(&Translator);
    Translator.load(LanguageList.at(LanguageSelect));

    QApplication::installTranslator(&Translator);
    ui->retranslateUi(this);
    emit Retranslator();

    try
    {
        LocalDB=QSqlDatabase::database("LocalDB");

        if(!LocalDB.isOpen())
        {
            LocalDBInit();
            LocalDB=QSqlDatabase::database("LocalDB");
        }

        QSqlQuery LocalDBQuery(LocalDB);
        LocalDBQuery.exec(QString("update general_setting set language=%1").arg(LanguageSelect));
    }
    catch(QException &e)
    {
        qDebug()<<e.what();
    }
}
