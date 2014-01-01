#include "gui/MainWindow.h"
#include <QMenuBar>
#include <QTextCodec>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setCodecs("UTF-8");
    setWindowTitle(tr("Papers Light"));
    showMaximized();

    createMenus();
    createPanels();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setCodecs(const char* codec)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(codec));
}

void MainWindow::openDatabase()
{

}

void MainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    openDatabaseAction_ = fileMenu->addAction(tr("Open Database File"));
    connect(openDatabaseAction_, SIGNAL(triggered()), this, SLOT(openDatabase()));
}

void MainWindow::createPanels()
{

}
