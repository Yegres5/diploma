#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

#define ORGANIZATION_NAME "@EVGENY@"
#define ORGANIZATION_DOMAIN "localhost"
#define APPLICATION_NAME "FindOptimalK program"

#define SETTINGS_LA "LA/"
#define SETTINGS_ROCKET "Rocket/"
#define SETTINGS_MODEL "Model/"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;

    QAction* m_saveSettingsAction;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void save_settings();
};

#endif // MAINWINDOW_H
