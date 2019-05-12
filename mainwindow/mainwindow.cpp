#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QMap>
#include <QVariant>
#include <QThread>

#include "initialmodelingframe.h"
#include "AcceptableKDialog/acceptablek.h"
#include "dbconnector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    InitialModelingFrame* frame = new InitialModelingFrame(this);
    setCentralWidget(frame);

    QMenu* file_menu = new QMenu(tr("File"), this);
    QMenu* db_menu = new QMenu(tr("Data base"), this);
    menuBar()->addMenu(file_menu);
    menuBar()->addMenu(db_menu);

    m_saveSettingsAction = new QAction(this);
    m_saveSettingsAction->setText(tr("Save"));
    m_saveSettingsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    file_menu->addAction(m_saveSettingsAction);

    m_loadSettingsAction = new QAction(this);
    m_loadSettingsAction->setText(tr("Load"));
    m_loadSettingsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    file_menu->addAction(m_loadSettingsAction);

    m_dbConnectionAction = new QAction(this);
    m_dbConnectionAction->setText(tr("Connect to data base"));
    m_dbConnectionAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    db_menu->addAction(m_dbConnectionAction);

    connect(m_saveSettingsAction, SIGNAL(triggered()),
            this, SLOT(save_settings()));
    connect(m_loadSettingsAction, SIGNAL(triggered()),
            this, SLOT(load_settings()));
    connect(m_dbConnectionAction, SIGNAL(triggered()),
            this, SLOT(call_dbConnection()));

    connect(frame,SIGNAL(startModelingPressed(QMap<QString, QVariant>*)),
            this, SLOT(start_modeling(QMap<QString, QVariant>*)));
    load_settings();
}

MainWindow::~MainWindow()
{
    save_settings();
}

void MainWindow::save_settings()
{
    InitialModelingFrame* frame = dynamic_cast<InitialModelingFrame*>(centralWidget());
    QMap<QString, QVariant> iniParametrs = frame->getInitialParametrs();
    QSettings settings(SETTINGS_FILE_PWD, QSettings::IniFormat);
    settings.beginGroup(SETTINGS_MAINWINDOW);
    settings.setValue(SETTINGS_INITIAL_SECTION, iniParametrs);
    settings.endGroup();
}

void MainWindow::load_settings()
{
    QSettings settings(SETTINGS_FILE_PWD, QSettings::IniFormat);
    QMap<QString, QVariant> iniMap = settings.value(SETTINGS_MAINWINDOW "/" SETTINGS_INITIAL_SECTION).toMap();
    InitialModelingFrame* frame = dynamic_cast<InitialModelingFrame*>(centralWidget());
    frame->setInitialParametrs(iniMap);
}

void MainWindow::call_dbConnection()
{
    dbConnector* connector = new dbConnector();
    connect(connector, SIGNAL(send_dbParameters(QList<QVariant>*)),
            this->centralWidget(), SIGNAL(moveToRocketDBParameters(QList<QVariant>*)));
    connector->show();
}
#include <QDialog>
void MainWindow::start_modeling(QMap<QString, QVariant>* parametrs)
{
    parametrs->insert("Rock x",0);
    parametrs->insert("Rock y",0);
    parametrs->insert("Rock z",0);
    AcceptableK* window = new AcceptableK(parametrs, this, Qt::Window);
    window->show();
    window->

}
