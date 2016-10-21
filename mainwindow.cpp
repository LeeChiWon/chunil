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
    MachineLabel<<tr("MachineName")<<tr("DirectoryPath")<<tr("Status");
    ui->tableWidget_Community->setHorizontalHeaderLabels(CommunityLabel);
    ui->tableWidget_MachineInfo->setHorizontalHeaderLabels(MachineLabel);
    ui->tableWidget_MachineInfo->resizeColumnsToContents();
    ui->tableWidget_Community->resizeColumnsToContents();
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
    if(ui->lineEdit_MachineName->text().isEmpty() || ui->lineEdit_Path->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Warning"),tr("MachineName or Path is empty."),QMessageBox::Ok);
        return;
    }

    for(int i=0; i<ui->tableWidget_MachineInfo->rowCount(); i++)
    {
        if(ui->lineEdit_MachineName->text()==ui->tableWidget_MachineInfo->item(i,0)->text())
        {
            QMessageBox::warning(this,tr("Warning"),tr("MachineName is same."),QMessageBox::Ok);
            return;
        }
    }
    TabelWidgetAdd(ui->lineEdit_MachineName->text(),ui->lineEdit_Path->text());
    TableWidgetDB(DB_ADD);
}

void MainWindow::on_pushButton_Delete_clicked()
{    
    if(ui->lineEdit_MachineName->text().isEmpty() && ui->lineEdit_Path->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Warning"),tr("MachineName or Path is empty."),QMessageBox::Ok);
        return;
    }
    TableWidgetDelete();
    TableWidgetDB(DB_DELETE);
}

void MainWindow::on_pushButton_Path_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_Path->setText(dir);
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

    emit Retranslator();
    ui->retranslateUi(this);

    TableWidgetInit();

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

void MainWindow::TabelWidgetAdd(QString MachineName, QString Path)
{
    QLabel *label=new QLabel();

    // QPixmap Item(":/Img/Img/NoFile.png");
    // label->setPixmap(Item.scaled(30,35,Qt::KeepAspectRatio));
    ui->tableWidget_MachineInfo->setRowCount(ui->tableWidget_MachineInfo->rowCount()+1);

    label->setText(tr("<p><img src=:/Img/Img/NoFile.png width=30 height=30 align=middle> Not File</p>"));

    TableWidgetLabelMap.insert(MachineName,label);

    ui->tableWidget_MachineInfo->setItem(ui->tableWidget_MachineInfo->rowCount()-1,0,new QTableWidgetItem(MachineName));
    ui->tableWidget_MachineInfo->setItem(ui->tableWidget_MachineInfo->rowCount()-1,1,new QTableWidgetItem(Path));
    ui->tableWidget_MachineInfo->setCellWidget(ui->tableWidget_MachineInfo->rowCount()-1,2,label);
    ui->tableWidget_MachineInfo->resizeColumnsToContents();
}

void MainWindow::TableWidgetDelete()
{
    ui->tableWidget_MachineInfo->removeRow(ui->tableWidget_MachineInfo->currentRow());
    ui->tableWidget_MachineInfo->resizeColumnsToContents();
}

void MainWindow::TableWidgetDB(int Select)
{
    try
    {
        LocalDB=QSqlDatabase::database("LocalDB");

        if(!LocalDB.isOpen())
        {
            LocalDBInit();
            LocalDB=QSqlDatabase::database("LocalDB");
        }

        QSqlQuery LocalDBQuery(LocalDB);
        switch(Select)
        {
        case DB_ADD:
            LocalDBQuery.exec(QString("insert into machine_setting(machinename,directorypath) values('%1','%2')").arg(ui->lineEdit_MachineName->text(),ui->lineEdit_Path->text()));
            break;
        case DB_DELETE:
            LocalDBQuery.exec(QString("delete from machine_setting where machinename='%1'").arg(ui->lineEdit_MachineName->text()));
            break;
        }
        qDebug()<<LocalDBQuery.lastError();
    }
    catch(QException &e)
    {
        qDebug()<<e.what();
    }
}

void MainWindow::on_tableWidget_MachineInfo_itemClicked(QTableWidgetItem *item)
{
    ui->lineEdit_MachineName->setText(ui->tableWidget_MachineInfo->item(item->row(),0)->text());
    ui->lineEdit_Path->setText(ui->tableWidget_MachineInfo->item(item->row(),1)->text());
}

void MainWindow::on_actionKorean_triggered()
{
    LanguageChange(KOREAN);
}

void MainWindow::on_actionEnglish_triggered()
{
    LanguageChange(ENGLISH);
}

void MainWindow::on_actionEspanol_triggered()
{
    LanguageChange(ESPANOL);
}

void MainWindow::closeEvent ( QCloseEvent * event )
{
    event->ignore();
    if (QMessageBox::Yes == QMessageBox::question(this, tr("Close Confirmation?"),
                                                  tr("Are you sure you want to exit?"),
                                                  QMessageBox::Yes|QMessageBox::No))
    {
        event->accept();
    }
}
