#include "gui/MainWindow.h"
#include <QMenuBar>
#include <QTextCodec>
#include <QFileDialog>

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
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open a Database File"),
                                                    QDir::homePath());

    if (!filePath.isEmpty()) {
        db_.init(filePath);
    }

}

void MainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    openDatabaseAction_ = fileMenu->addAction(tr("Open Database File"));
    connect(openDatabaseAction_, &QAction::triggered, this, &MainWindow::openDatabase);
}

void MainWindow::createPanels()
{

}
