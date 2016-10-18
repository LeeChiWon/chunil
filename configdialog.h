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

    QSqlDatabase LocalDB;

    void ConfigDataSave();
    void ConfigDataLoad();

signals:
    void LocalDBInit();
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
