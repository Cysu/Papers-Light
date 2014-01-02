#include "gui/MainWindow.h"
#include "utils/utils.h"
#include <QMenuBar>
#include <QTextCodec>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

using std::string;
using std::vector;

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
        refreshAllPanels();
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
    yearList_ = new CategoryList(tr("Year"));
    bookTitleList_ = new CategoryList(tr("Book Title"));
    tagList_ = new CategoryList(tr("Tag"));
    searchBar_ = new SearchBar;
    paperList_ = new PaperList;
    paperInfoTable_ = new PaperInfoTable;

    QVBoxLayout* leftPanelLayout = new QVBoxLayout;
    leftPanelLayout->addWidget(yearList_);
    leftPanelLayout->addWidget(bookTitleList_);
    leftPanelLayout->addWidget(tagList_);

    QVBoxLayout* middlePanelLayout = new QVBoxLayout;
    middlePanelLayout->addWidget(searchBar_);
    middlePanelLayout->addWidget(paperList_);

    QVBoxLayout* rightPanelLayout = new QVBoxLayout;
    rightPanelLayout->addWidget(paperInfoTable_);

    QHBoxLayout* frameLayout = new QHBoxLayout;
    frameLayout->addLayout(leftPanelLayout);
    frameLayout->addLayout(middlePanelLayout);
    frameLayout->addLayout(rightPanelLayout);

    QWidget* frame = new QWidget;
    frame->setLayout(frameLayout);

    setCentralWidget(frame);
}

void MainWindow::refreshAllPanels()
{
    vector<CategoryStats> yearStats = db_.getYearStats();
    vector<CategoryStats> bookTitleStats = db_.getBookTitleStats();

    yearList_->clear();
    yearList_->addItems(stats2QStringList(yearStats));

    bookTitleList_->clear();
    bookTitleList_->addItems(stats2QStringList(bookTitleStats));
}
