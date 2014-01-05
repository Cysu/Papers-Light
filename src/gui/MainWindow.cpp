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

void MainWindow::newPaper()
{
    Paper paper;
    paper.setId(0);
    paper.setTitle("Untitled");

    papers_.push_back(paper);
    paperList_->addItem(paper.getTitle().c_str());

    paperInfoTable_->setPaper(paper);
}

void MainWindow::editPreference()
{

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

void MainWindow::paperSelectedOnly(int index)
{
    paperInfoTable_->setPaper(papers_[index]);
}

void MainWindow::savePaperInfo()
{
    paperInfoTable_->saveChanges();
    databaseHelper_.updatePaper(paperInfoTable_->getPaper());

    refreshAllPanels();
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

    fileMenu->addSeparator();
    newPaperAction_ = fileMenu->addAction(tr("New Paper"));
    newPaperAction_->setShortcut(QKeySequence::New);
    connect(newPaperAction_, &QAction::triggered, this, &MainWindow::newPaper);

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editPreferenceAction_ = editMenu->addAction(tr("Edit Preference"));
    connect(editPreferenceAction_, &QAction::triggered, this, &MainWindow::editPreference);
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

    QPushButton* paperInfoSaveButton = new QPushButton(tr("Save"));

    connect(yearList_, &CategoryList::itemSelectedOnly, this, &MainWindow::yearSelectedOnly);
    connect(bookTitleList_, &CategoryList::itemSelectedOnly, this, &MainWindow::bookTitleSelectedOnly);
    connect(authorList_, &CategoryList::itemSelectedOnly, this, &MainWindow::authorSelectedOnly);
    connect(tagList_, &CategoryList::itemSelectedOnly, this, &MainWindow::tagSelectedOnly);
    connect(paperList_, &PaperList::itemSelectedOnly, this, &MainWindow::paperSelectedOnly);
    connect(paperInfoSaveButton, &QPushButton::clicked, this, &MainWindow::savePaperInfo);

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
    rightPanelLayout->addWidget(paperInfoSaveButton);

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

    papers_.clear();
    while (query.next()) {
        int paperId = query.value(0).toInt();
        papers_.push_back(databaseHelper_.getPaper(paperId));
    }

    paperList_->clear();
    paperList_->addItems(papers2QStringList(papers_));
}
