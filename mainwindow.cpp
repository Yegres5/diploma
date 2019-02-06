#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QSettings>

#include "initialmodelingframe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    InitialModelingFrame* frame = new InitialModelingFrame(this);
    setCentralWidget(frame);

    QMenu* file_menu = new QMenu(tr("File"), this);
    menuBar()->addMenu(file_menu);

    m_saveSettingsAction = new QAction(this);
    m_saveSettingsAction->setText(tr("Save"));
    m_saveSettingsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    file_menu->addAction(m_saveSettingsAction);

    m_loadSettingsAction = new QAction(this);
    m_loadSettingsAction->setText(tr("Load"));
    m_loadSettingsAction->setShortcut(QKeySequence(Qt::CTRL+ Qt::Key_L));
    file_menu->addAction(m_loadSettingsAction);

    connect(m_saveSettingsAction, SIGNAL(triggered()),
            this, SLOT(save_settings()));
    connect(m_loadSettingsAction, SIGNAL(triggered()),
            this, SLOT(load_settings()));

    load_settings();
}

MainWindow::~MainWindow()
{
}

// saving settings: https://evileg.com/ru/post/61/
#include <QDebug>
void MainWindow::save_settings()
{
    InitialModelingFrame* frame = dynamic_cast<InitialModelingFrame*>(centralWidget());
    QMap<QString, QVariant> iniParametrs = frame->getInitialParametrs();
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup(SETTINGS_MAINWINDOW);
    settings.setValue("initial parametrs", iniParametrs);
    settings.endGroup();
}

void MainWindow::load_settings()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    QMap<QString, QVariant> iniMap = settings.value(SETTINGS_MAINWINDOW"/initial parametrs").toMap();
    InitialModelingFrame* frame = dynamic_cast<InitialModelingFrame*>(centralWidget());
    frame->setInitialParametrs(iniMap);
}
