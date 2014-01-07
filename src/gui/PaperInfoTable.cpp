#include "gui/PaperInfoTable.h"
#include "utils/PreferencesManager.h"
#include <QLabel>
#include <QFileDialog>
#include <QPushButton>
#include <QGridLayout>

using std::string;
using std::vector;

static QString list2QString(const vector<string>& list)
{
    QString ret;
    for (vector<string>::size_type i = 0; i < list.size(); ++i) {
        ret += list[i].c_str();
        if (i != list.size()-1) ret += ",";
    }
    return ret;
}

static vector<string> QString2list(const QString& qstring)
{
    QStringList qstringlist = qstring.split(",");
    vector<string> ret(qstringlist.size());
    for (vector<string>::size_type i = 0; i < ret.size(); ++i) {
        ret[i] = qstringlist[i].toStdString();
    }
    return ret;
}

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
      comment_(nullptr)
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
    authors_->setText(list2QString(paper.getAuthors()));
    tags_->setText(list2QString(paper.getTags()));
    path_->setText(paper.getPath().c_str());
    comment_->setText(paper.getComment().c_str());
}

void PaperInfoTable::saveChanges()
{
    paper_.setYear(year_->text().toInt());
    paper_.setBookTitle(bookTitle_->text().toStdString());
    paper_.setTitle(title_->text().toStdString());
    paper_.setAuthors(QString2list(authors_->text()));
    paper_.setTags(QString2list(tags_->text()));
    paper_.setPath(path_->text().toStdString());
    paper_.setComment(comment_->toPlainText().toStdString());
}

void PaperInfoTable::chooseFilePath()
{
    QString filePath = QFileDialog::getOpenFileName(
                this, tr("Choose Database File"), QDir::homePath());

    if (!filePath.isEmpty()) {
        QString homePath = PreferencesManager::instance().getPaperFolderPath();
        path_->setText(relativePath(homePath, filePath));
    }
}

void PaperInfoTable::createPanels()
{
    year_ = new QLineEdit;
    bookTitle_ = new QLineEdit;
    title_ = new QLineEdit;
    authors_ = new QLineEdit;
    tags_ = new QLineEdit;
    path_ = new QLineEdit;
    comment_ = new QTextEdit;

    QString paperFolderPath = PreferencesManager::instance().getPaperFolderPath();
    QLabel* paperFolderPathLabel = new QLabel(QDir::toNativeSeparators(paperFolderPath + "/"));

    QIcon folderOpenIcon(":/icons/folder_open.png");
    QPushButton* pathButton = new QPushButton(folderOpenIcon, tr(""));
    connect(pathButton, &QPushButton::clicked, this, &PaperInfoTable::chooseFilePath);

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

    setLayout(layout);
}
