#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

#define ORGANIZATION_NAME "@EVGENY@"
#define ORGANIZATION_DOMAIN "localhost"
#define APPLICATION_NAME "FindOptimalK program"

#define SETTINGS_MAINWINDOW "Mainwindow parametrs"

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

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void save_settings();
    void load_settings();
};

#endif // MAINWINDOW_H
