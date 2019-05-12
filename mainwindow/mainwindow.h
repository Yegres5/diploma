#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMap>
#include <QVariant>

#define ORGANIZATION_NAME "@EVGENY@"
#define ORGANIZATION_DOMAIN "localhost"
#define APPLICATION_NAME "FindOptimalK program"


#define SETTINGS_FILE_PWD "config.ini" //Full path to the ini file

//Settings group names
#define SETTINGS_MAINWINDOW "Mainwindow parametrs"

//Settings section names
#define SETTINGS_INITIAL_SECTION "Initial parametrs"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;

    QAction* m_saveSettingsAction;
    QAction* m_loadSettingsAction;
    QAction* m_dbConnectionAction;
    QMap<QString, QVariant> rocketMapFromDB;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void save_settings();
    void load_settings();
    void call_dbConnection();
    void start_modeling(QMap<QString, QVariant>* parametrs);

};

#endif // MAINWINDOW_H
