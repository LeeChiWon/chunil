#include "configdialog.h"
#include "ui_configdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *pMain;

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    connect(this,SIGNAL(LocalDBInit()),pMain,SLOT(LocalDBInit()));
    ConfigDataLoad();
}

ConfigDialog::~ConfigDialog()
{
    delete ui; 
}

void ConfigDialog::ConfigDataSave()
{
    if(ui->lineEdit_DBIP->text().isEmpty() ||ui->lineEdit_DBPort->text().isEmpty() || ui->lineEdit_DBName->text().isEmpty() ||
            ui->lineEdit_TableName->text().isEmpty() || ui->lineEdit_UserName->text().isEmpty() || ui->lineEdit_Password->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Warning"),tr("Setting is empty."),QMessageBox::Ok);
        return;
    }

    try
    {
        if(!LocalDB.isOpen())
        {
            QSqlDatabase::removeDatabase("LocalDB");
            emit LocalDBInit();
            LocalDB=QSqlDatabase::database("LocalDB");
        }

        QSqlQuery LocalDBQuery(LocalDB);
        LocalDBQuery.exec(QString("update general_setting set dbip='%1',dbport='%2',dbname='%3',tablename='%4',username='%5',password='%6',autostart=%7")
                          .arg(ui->lineEdit_DBIP->text(),ui->lineEdit_DBPort->text(),ui->lineEdit_DBName->text(),ui->lineEdit_TableName->text(),ui->lineEdit_UserName->text(),ui->lineEdit_Password->text())
                          .arg(ui->checkBox_AutoStart->isChecked()));
        QMessageBox::information(this,tr("Info"),tr("Setting Saved."),QMessageBox::Ok);
    }

    catch(QException &e)
    {
        qDebug()<<e.what();
    }
}

void ConfigDialog::ConfigDataLoad()
{
    try
    {
        LocalDB=QSqlDatabase::database("LocalDB");

        if(!LocalDB.isOpen())
        {
            QSqlDatabase::removeDatabase("LocalDB");
            emit LocalDBInit();
            LocalDB=QSqlDatabase::database("LocalDB");
        }


        QSqlQuery LocalDBQuery(LocalDB);
        LocalDBQuery.exec("select * from general_setting");
        if(LocalDBQuery.next())
        {
            ui->lineEdit_DBIP->setText(LocalDBQuery.value("dbip").toString());
            ui->lineEdit_DBPort->setText(LocalDBQuery.value("dbport").toString());
            ui->lineEdit_DBName->setText(LocalDBQuery.value("dbname").toString());
            ui->lineEdit_TableName->setText(LocalDBQuery.value("tablename").toString());
            ui->lineEdit_UserName->setText(LocalDBQuery.value("username").toString());
            ui->lineEdit_Password->setText(LocalDBQuery.value("password").toString());
            ui->checkBox_AutoStart->setChecked(LocalDBQuery.value("autostart").toInt());
        }

        LocalDBQuery.exec("select * from machine_setting");
        while(LocalDBQuery.next())
        {
           QTableWidgetItem *TableWidgetItem=new QTableWidgetItem();
           TableWidgetItem->setIcon(QIcon(":/Img/NoFile.png"));
           TableWidgetItem->setTextAlignment(Qt::AlignCenter);
           //TableWidgetItem->setText(tr(""));
        }
    }

    catch(QException &e)
    {
        qDebug()<<e.what();
    }
}

void ConfigDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    switch(ui->buttonBox->standardButton(button))
    {
    case QDialogButtonBox::Ok:
        ConfigDataSave();
        break;
    case QDialogButtonBox::Cancel:
        this->close();
        break;
    }
}
