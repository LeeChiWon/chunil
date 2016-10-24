#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    Ui::ConfigDialog *ui;

    QSqlDatabase LocalDB;

    void ConfigDataSave();
    void ConfigDataLoad();

signals:
    void LocalDBInit();
    void AutoStart();
    void LanguageChanged(int LanguageSelect);
    void TableWidgetAdd(QString MachineName,QString Path);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void Retranslator();

private:

};

#endif // CONFIGDIALOG_H
