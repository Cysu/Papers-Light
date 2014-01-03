#include "gui/MainWindow.h"
#include "utils/utils.h"
#include <QMenuBar>
#include <QTextCodec>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>

using std::string;
using std::vector;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setCodecs("UTF-8");
    setWindowTitle(tr("Papers Light"));

    createMenus();
    createPanels();
}

MainWindow::~MainWindow()
{

}

void MainWindow::openDatabase()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open a Database File"),
                                                    QDir::homePath());

    if (!filePath.isEmpty()) {
        databaseHelper_.init(filePath);
        refreshAllPanels();
    }
}

void MainWindow::yearSelectedOnly(int index)
{
    searchHelper_.clear();

    SearchHelper::Filter filter;
    filter.first = SearchHelper::Year;
    filter.second = yearStats_[index].getName();

    searchHelper_.addFilter(filter);
    refreshPaperList();
}

void MainWindow::bookTitleSelectedOnly(int index)
{
    searchHelper_.clear();

    SearchHelper::Filter filter;
    filter.first = SearchHelper::BookTitle;
    filter.second = bookTitleStats_[index].getName();

    searchHelper_.addFilter(filter);
    refreshPaperList();
}

void MainWindow::authorSelectedOnly(int index)
{
    searchHelper_.clear();

    SearchHelper::Filter filter;
    filter.first = SearchHelper::Author;
    filter.second = authorStats_[index].getName();

    searchHelper_.addFilter(filter);
    refreshPaperList();
}

void MainWindow::tagSelectedOnly(int index)
{
    searchHelper_.clear();

    SearchHelper::Filter filter;
    filter.first = SearchHelper::Tag;
    filter.second = tagStats_[index].getName();

    searchHelper_.addFilter(filter);
    refreshPaperList();
}

void MainWindow::setCodecs(const char* codec)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(codec));
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
    authorList_ = new CategoryList(tr("Author"));
    tagList_ = new CategoryList(tr("Tag"));
    searchBar_ = new SearchBar;
    paperList_ = new PaperList;
    paperInfoTable_ = new PaperInfoTable;

    connect(yearList_, &CategoryList::itemSelectedOnly, this, &MainWindow::yearSelectedOnly);
    connect(bookTitleList_, &CategoryList::itemSelectedOnly, this, &MainWindow::bookTitleSelectedOnly);
    connect(authorList_, &CategoryList::itemSelectedOnly, this, &MainWindow::authorSelectedOnly);
    connect(tagList_, &CategoryList::itemSelectedOnly, this, &MainWindow::tagSelectedOnly);

    QVBoxLayout* leftPanelLayout = new QVBoxLayout;
    leftPanelLayout->addWidget(yearList_);
    leftPanelLayout->addWidget(bookTitleList_);
    leftPanelLayout->addWidget(authorList_);
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

    showMaximized();
}

void MainWindow::refreshAllPanels()
{
    yearStats_ = databaseHelper_.getYearStats();
    bookTitleStats_ = databaseHelper_.getBookTitleStats();
    authorStats_ = databaseHelper_.getAuthorStats();
    tagStats_ = databaseHelper_.getTagStats();

    yearList_->clear();
    yearList_->addItems(stats2QStringList(yearStats_));

    bookTitleList_->clear();
    bookTitleList_->addItems(stats2QStringList(bookTitleStats_));

    authorList_->clear();
    authorList_->addItems(stats2QStringList(authorStats_));

    tagList_->clear();
    tagList_->addItems(stats2QStringList(tagStats_));

    searchHelper_.clear();

    refreshPaperList();
}

void MainWindow::refreshPaperList()
{
    string queryString = searchHelper_.getSqlQueryString();
    QSqlQuery query = databaseHelper_.exec(queryString.c_str());

    vector<Paper> papers;
    while (query.next()) {
        int paperId = query.value(0).toInt();
        papers.push_back(databaseHelper_.getPaper(paperId));
    }

    paperList_->clear();
    paperList_->addItems(papers2QStringList(papers));
}
