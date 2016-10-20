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

    void TableWidgetInit();

signals:
    void Retranslator();

private slots:
    void on_actionConfig_triggered();
    void LocalDBInit();

    void on_pushButton_Add_clicked();

    void on_pushButton_Delete_clicked();

    void on_pushButton_Path_clicked();

    void on_actionStart_triggered();

    void on_actionStop_triggered();
    void LanguageChange(int LanguageSelect);
private:

};

#endif // MAINWINDOW_H
