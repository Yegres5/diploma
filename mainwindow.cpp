#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>

#include "initialmodelingframe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    InitialModelingFrame* frame = new InitialModelingFrame(this);
    setCentralWidget(frame);

    m_saveSettingsAction = new QAction(this);
    m_saveSettingsAction->setText(tr("Save"));
    m_saveSettingsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    QMenu* file_menu = new QMenu(tr("File"), this) ;
    file_menu->addAction(m_saveSettingsAction);
    menuBar()->addMenu(file_menu);
}

MainWindow::~MainWindow()
{
}

// saving settings: https://evileg.com/ru/post/61/

void MainWindow::save_settings()
{

}
