#include "gui/PaperInfoTable.h"
#include "utils/PreferencesManager.h"
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QGridLayout>

using std::string;
using std::vector;

static QString relativePath(const QString& homePath, const QString& filePath)
{
    QStringList homePathFolders = QDir::fromNativeSeparators(homePath).split("/");
    QStringList filePathFolders = QDir::fromNativeSeparators(filePath).split("/");

    int n = homePathFolders.size();
    int m = filePathFolders.size();

    int i = 0;
    while (i < std::min(n, m) && homePathFolders[i] == filePathFolders[i]) ++i;

    QStringList ret;
    for (int j = 0; j < n-i; ++j) ret << "..";
    for (int j = i; j < m; ++j) ret << filePathFolders[j];

    return QDir::toNativeSeparators(ret.join("/"));
}

PaperInfoTable::PaperInfoTable(QWidget* parent)
    : QWidget(parent),
      year_(nullptr),
      bookTitle_(nullptr),
      title_(nullptr),
      authors_(nullptr),
      tags_(nullptr),
      path_(nullptr),
      comment_(nullptr),
      paper_()
{
    createPanels();
}

PaperInfoTable::~PaperInfoTable()
{

}

const Paper& PaperInfoTable::getPaper() const
{
    return paper_;
}

void PaperInfoTable::setPaper(const Paper& paper)
{
    paper_ = paper;
    year_->setText(QString::number(paper.getYear()));
    bookTitle_->setText(paper.getBookTitle().c_str());
    title_->setText(paper.getTitle().c_str());
    authors_->setBubbles(paper.getAuthors());
    tags_->setBubbles(paper.getTags());
    path_->setText(paper.getPath().c_str());
    comment_->setText(paper.getComment().c_str());
}

void PaperInfoTable::clear()
{
    paper_ = Paper();
    year_->clear();
    bookTitle_->clear();
    title_->clear();
    authors_->clear();
    tags_->clear();
    path_->clear();
    comment_->clear();
}

void PaperInfoTable::chooseFilePath()
{
    QString homePath = PreferencesManager::instance().getPaperFolderPath();
    QString filePath = QFileDialog::getOpenFileName(
                this, tr("Choose Database File"), homePath);

    if (!filePath.isEmpty()) {
        path_->setText(relativePath(homePath, filePath));
    }
}

void PaperInfoTable::savePaper()
{
    if (paper_.isNull()) return;

    paper_.setYear(year_->text().toInt());
    paper_.setBookTitle(bookTitle_->text().toStdString());
    paper_.setTitle(title_->text().toStdString());
    paper_.setAuthors(authors_->getBubbles());
    paper_.setTags(tags_->getBubbles());
    paper_.setPath(path_->text().toStdString());
    paper_.setComment(comment_->toPlainText().toStdString());

    emit paperSaved();
}

void PaperInfoTable::removePaper()
{
    if (paper_.isNull()) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
                this, tr("Remove Paper"), tr("Confirm to remove the paper?"));

    if (reply == QMessageBox::Yes) {
        emit paperRemoved(paper_);
        clear();
    }
}

void PaperInfoTable::createPanels()
{
    year_ = new QLineEdit;
    bookTitle_ = new QLineEdit;
    title_ = new QLineEdit;
    authors_ = new BubblesEdit;
    tags_ = new BubblesEdit;
    path_ = new QLineEdit;
    comment_ = new QTextEdit;

    QString paperFolderPath = PreferencesManager::instance().getPaperFolderPath();
    QLabel* paperFolderPathLabel = new QLabel(QDir::toNativeSeparators(paperFolderPath + "/"));

    QIcon folderOpenIcon(":/icons/folder_open.png");
    QPushButton* pathButton = new QPushButton(folderOpenIcon, tr(""));
    connect(pathButton, &QPushButton::clicked, this, &PaperInfoTable::chooseFilePath);

    QPushButton* saveButton = new QPushButton(tr("Save"));
    QPushButton* removeButton = new QPushButton(tr("Remove"));
    connect(saveButton, &QPushButton::clicked, this, &PaperInfoTable::savePaper);
    connect(removeButton, &QPushButton::clicked, this, &PaperInfoTable::removePaper);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Year")), 0, 0);
    layout->addWidget(year_, 0, 1, 1, 1);
    layout->addWidget(new QLabel(tr("Book Title")), 0, 2);
    layout->addWidget(bookTitle_, 0, 3, 1, 3);
    layout->addWidget(new QLabel(tr("Title")), 1, 0);
    layout->addWidget(title_, 1, 1, 1, 5);
    layout->addWidget(new QLabel(tr("Authors")), 2, 0);
    layout->addWidget(authors_, 2, 1, 1, 5);
    layout->addWidget(new QLabel(tr("Tags")), 3, 0);
    layout->addWidget(tags_, 3, 1, 1, 5);
    layout->addWidget(new QLabel(tr("Path")), 4, 0);
    layout->addWidget(paperFolderPathLabel, 4, 1);
    layout->addWidget(path_, 4, 2, 1, 3);
    layout->addWidget(pathButton, 4, 5);
    layout->addWidget(comment_, 5, 0, 1, 6);
    layout->addWidget(saveButton, 6, 0, 1, 3);
    layout->addWidget(removeButton, 6, 3, 1, 3);

    setLayout(layout);
}
