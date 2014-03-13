#include "db/DatabaseHelper.h"
#include "common/const.h"
#include <algorithm>
#include <QSqlQuery>
#include <QVariant>

using std::string;
using std::vector;
using std::to_string;

bool sortByNameFunc(const CategoryStats& a, const CategoryStats& b)
{
    return a.getName().compare(b.getName()) < 0;
}

bool sortByCountFunc(const CategoryStats &a, const CategoryStats &b)
{
    return (a.getCount() > b.getCount()) ||
            (a.getCount() == b.getCount() && a.getName().compare(b.getName()) < 0);
}

static void sortStats(vector<CategoryStats>& stats,
                      DatabaseHelper::SortOrder order)
{
    if (order == DatabaseHelper::SortByNone) return;

    if (order == DatabaseHelper::SortByName) {
        std::sort(stats.begin(), stats.end(), sortByNameFunc);
    } else if (order == DatabaseHelper::SortByCount) {
        std::sort(stats.begin(), stats.end(), sortByCountFunc);
    }
}

DatabaseHelper::DatabaseHelper()
    : db_(QSqlDatabase::addDatabase("QSQLITE"))
{

}

DatabaseHelper::~DatabaseHelper()
{
    db_.close();
}

void DatabaseHelper::init(const QString& filePath)
{
    db_.close();
    db_.setDatabaseName(filePath);
    db_.open();

    createTables();
}

QSqlQuery DatabaseHelper::exec(const QString& query)
{
    QSqlQuery ret;
    ret.exec(query);
    return ret;
}

Paper DatabaseHelper::getPaper(int paperId)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "SELECT year, book_title_id, title, path, comment FROM pl_paper WHERE paper_id = %d",
            paperId);

    QSqlQuery query;
    query.exec(qBuf);

    Paper paper;
    if (!query.last()) return paper;

    paper.setId(paperId);
    paper.setYear(query.value(0).toInt());
    paper.setBookTitle(getBookTitle(query.value(1).toInt()));
    paper.setTitle(query.value(2).toString().toStdString());
    paper.setPath(query.value(3).toString().toStdString());
    paper.setComment(query.value(4).toString().toStdString());

    sprintf(qBuf,
            "SELECT author_id FROM pl_paper2author WHERE paper_id = %d ORDER BY rowid ASC",
            paperId);

    query.exec(qBuf);

    vector<string> authors;
    while (query.next()) {
        int authorId = query.value(0).toInt();
        authors.push_back(getAuthor(authorId));
    }

    sprintf(qBuf,
            "SELECT tag_id FROM pl_paper2tag WHERE paper_id = %d",
            paperId);

    query.exec(qBuf);

    vector<string> tags;
    while (query.next()) {
        int tagId = query.value(0).toInt();
        tags.push_back(getTag(tagId));
    }

    paper.setAuthors(authors);
    paper.setTags(tags);

    return paper;
}

string DatabaseHelper::getBookTitle(int bookTitleId)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "SELECT book_title_name FROM pl_book_title WHERE book_title_id = %d",
            bookTitleId);

    QSqlQuery query;
    query.exec(qBuf);

    if (!query.last()) return string();
    return query.value(0).toString().toStdString();
}

string DatabaseHelper::getAuthor(int authorId)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "SELECT author_name FROM pl_author WHERE author_id = %d",
            authorId);

    QSqlQuery query;
    query.exec(qBuf);

    if (!query.last()) return string();
    return query.value(0).toString().toStdString();
}

string DatabaseHelper::getTag(int tagId)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "SELECT tag_name FROM pl_tag WHERE tag_id = %d",
            tagId);

    QSqlQuery query;
    query.exec(qBuf);

    if (!query.last()) return string();
    return query.value(0).toString().toStdString();
}

vector<CategoryStats> DatabaseHelper::getYearStats(SortOrder sortOrder)
{
    QSqlQuery query;
    query.exec("SELECT DISTINCT year FROM pl_paper");

    vector<int> years;
    while (query.next()) {
        int year = query.value(0).toInt();
        years.push_back(year);
    }

    vector<CategoryStats> stats(years.size());
    for (vector<CategoryStats>::size_type i = 0; i < stats.size(); ++i) {
        char qBuf[BUFSIZE];
        sprintf(qBuf,
                "SELECT COUNT(*) FROM pl_paper WHERE year = %d",
                years[i]);

        query.exec(qBuf);
        query.last();

        stats[i].setName(to_string(static_cast<long long>(years[i])));
        stats[i].setCount(query.value(0).toInt());
    }

    sortStats(stats, sortOrder);

    return stats;
}

vector<CategoryStats> DatabaseHelper::getBookTitleStats(SortOrder sortOrder)
{
    QSqlQuery query;
    query.exec("SELECT * FROM pl_book_title");

    vector<int> bookTitlesId;
    vector<string> bookTitlesName;
    while (query.next()) {
        int bookTitleId = query.value(0).toInt();
        string bookTitleName = query.value(1).toString().toStdString();
        bookTitlesId.push_back(bookTitleId);
        bookTitlesName.push_back(bookTitleName);
    }

    vector<CategoryStats> stats(bookTitlesId.size());
    for (vector<CategoryStats>::size_type i = 0; i < stats.size(); ++i) {
        char qBuf[BUFSIZE];
        sprintf(qBuf,
                "SELECT COUNT(*) FROM pl_paper WHERE book_title_id = %d",
                bookTitlesId[i]);

        query.exec(qBuf);
        query.last();

        stats[i].setName(bookTitlesName[i]);
        stats[i].setCount(query.value(0).toInt());
    }

    sortStats(stats, sortOrder);

    return stats;
}

vector<CategoryStats> DatabaseHelper::getAuthorStats(SortOrder sortOrder)
{
    QSqlQuery query;
    query.exec("SELECT * FROM pl_author");

    vector<int> authorsId;
    vector<string> authorsName;
    while (query.next()) {
        int authorId = query.value(0).toInt();
        string authorName = query.value(1).toString().toStdString();
        authorsId.push_back(authorId);
        authorsName.push_back(authorName);
    }

    vector<CategoryStats> stats(authorsId.size());
    for (vector<CategoryStats>::size_type i = 0; i < stats.size(); ++i) {
        char qBuf[BUFSIZE];
        sprintf(qBuf,
                "SELECT COUNT(*) FROM pl_paper2author WHERE author_id = %d",
                authorsId[i]);

        query.exec(qBuf);
        query.last();

        stats[i].setName(authorsName[i]);
        stats[i].setCount(query.value(0).toInt());
    }

    sortStats(stats, sortOrder);

    return stats;
}

vector<CategoryStats> DatabaseHelper::getTagStats(SortOrder sortOrder)
{
    QSqlQuery query;
    query.exec("SELECT * FROM pl_tag");

    vector<int> tagsId;
    vector<string> tagsName;
    while (query.next()) {
        int tagId = query.value(0).toInt();
        string tagName = query.value(1).toString().toStdString();
        tagsId.push_back(tagId);
        tagsName.push_back(tagName);
    }

    vector<CategoryStats> stats(tagsId.size());
    for (vector<CategoryStats>::size_type i = 0; i < stats.size(); ++i) {
        char qBuf[BUFSIZE];
        sprintf(qBuf,
                "SELECT COUNT(*) FROM pl_paper2tag WHERE tag_id = %d",
                tagsId[i]);

        query.exec(qBuf);
        query.last();

        stats[i].setName(tagsName[i]);
        stats[i].setCount(query.value(0).toInt());
    }

    sortStats(stats, sortOrder);

    return stats;
}

int DatabaseHelper::addPaper(const Paper& paper)
{
    int bookTitleId = getBookTitleId(paper.getBookTitle());
    if (bookTitleId == -1) {
        bookTitleId = addBookTitle(paper.getBookTitle());
    }

    vector<int> authorsId(paper.getAuthors().size());
    for (vector<int>::size_type i = 0; i < paper.getAuthors().size(); ++i) {
        const string& author = paper.getAuthors().at(i);
        int authorId = getAuthorId(author);
        if (authorId == -1) {
            authorId = addAuthor(author);
        }
        authorsId[i] = authorId;
    }

    vector<int> tagsId(paper.getTags().size());
    for (vector<int>::size_type i = 0; i < paper.getTags().size(); ++i) {
        const string& tag = paper.getTags().at(i);
        int tagId = getTagId(tag);
        if (tagId == -1) {
            tagId = addTag(tag);
        }
        tagsId[i] = tagId;
    }

    char qBuf[BUFSIZE];
    if (paper.getId() <= 0) {
        sprintf(qBuf,
                "INSERT INTO pl_paper(year, book_title_id, title, path, comment) "
                "VALUES(%d, %d, \"%s\", \"%s\", \"%s\")",
                paper.getYear(), bookTitleId,
                paper.getTitle().c_str(),
                paper.getPath().c_str(),
                paper.getComment().c_str());
    } else {
        sprintf(qBuf,
                "INSERT INTO pl_paper(paper_id, year, book_title_id, title, path, comment) "
                "VALUES(%d, %d, %d, \"%s\", \"%s\", \"%s\")",
                paper.getId(), paper.getYear(), bookTitleId,
                paper.getTitle().c_str(),
                paper.getPath().c_str(),
                paper.getComment().c_str());
    }

    QSqlQuery query;
    query.exec(qBuf);

    int paperId = query.lastInsertId().toInt();

    for (int i = 0; i < static_cast<int>(authorsId.size()); ++i) {
        sprintf(qBuf,
                "INSERT INTO pl_paper2author(paper_id, author_id) "
                "VALUES(%d, %d)",
                paperId, authorsId[i]);

        query.exec(qBuf);
    }

    for (int i = 0; i < static_cast<int>(tagsId.size()); ++i) {
        sprintf(qBuf,
                "INSERT INTO pl_paper2tag(paper_id, tag_id) "
                "VALUES(%d, %d)",
                paperId, tagsId[i]);

        query.exec(qBuf);
    }

    return paperId;
}

int DatabaseHelper::addBookTitle(const string& bookTitle)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "INSERT INTO pl_book_title(book_title_name) "
            "VALUES(\"%s\")",
            bookTitle.c_str());

    QSqlQuery query;
    query.exec(qBuf);

    return query.lastInsertId().toInt();
}

int DatabaseHelper::addAuthor(const string& author)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "INSERT INTO pl_author(author_name) "
            "VALUES(\"%s\")",
            author.c_str());

    QSqlQuery query;
    query.exec(qBuf);

    return query.lastInsertId().toInt();
}

int DatabaseHelper::addTag(const string& tag)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "INSERT INTO pl_tag(tag_name) "
            "VALUES(\"%s\")",
            tag.c_str());

    QSqlQuery query;
    query.exec(qBuf);

    return query.lastInsertId().toInt();
}

int DatabaseHelper::updatePaper(const Paper& paper)
{
    removePaper(paper.getId());
    return addPaper(paper);
}

void DatabaseHelper::removePaper(const Paper& paper)
{
    removePaper(paper.getId());
}

void DatabaseHelper::removePaper(int paperId)
{
    // Remove the paper from database
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "DELETE FROM pl_paper WHERE paper_id = %d",
            paperId);

    QSqlQuery query;
    query.exec(qBuf);

    // Remove empty book titles
    query.exec("SELECT book_title_id FROM pl_book_title WHERE book_title_id NOT IN "
               "(SELECT DISTINCT book_title_id FROM pl_paper)");

    while(query.next()) {
        int bookTitleId = query.value(0).toInt();
        removeBookTitle(bookTitleId);
    }

    // Remove empty authors
    query.exec("SELECT author_id FROM pl_author WHERE author_id NOT IN "
               "(SELECT DISTINCT author_id FROM pl_paper2author)");

    while (query.next()) {
        int authorId = query.value(0).toInt();
        removeAuthor(authorId);
    }

    // Remove empty tags
    query.exec("SELECT tag_id FROM pl_tag WHERE tag_id NOT IN "
               "(SELECT DISTINCT tag_id FROM pl_paper2tag)");

    while (query.next()) {
        int tagId = query.value(0).toInt();
        removeTag(tagId);
    }
}

void DatabaseHelper::removeBookTitle(int bookTitleId)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "DELETE FROM pl_book_title WHERE book_title_id = %d",
            bookTitleId);

    QSqlQuery query;
    query.exec(qBuf);

    sprintf(qBuf,
            "SELECT paper_id FROM pl_paper WHERE book_title_id = %d",
            bookTitleId);

    query.exec(qBuf);
    while (query.next()) {
        int paperId = query.value(0).toInt();
        removePaper(paperId);
    }
}

void DatabaseHelper::removeAuthor(int authorId)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "DELETE FROM pl_author WHERE author_id = %d",
            authorId);

    QSqlQuery query;
    query.exec(qBuf);
}

void DatabaseHelper::removeTag(int tagId)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "DELETE FROM pl_tag WHERE tag_id = %d",
            tagId);

    QSqlQuery query;
    query.exec(qBuf);
}

void DatabaseHelper::createTables()
{
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON");
    query.exec("CREATE TABLE IF NOT EXISTS pl_paper("
               "    paper_id INTEGER PRIMARY KEY,"
               "    year SMALLINT,"
               "    book_title_id INT,"
               "    title TEXT NOT NULL,"
               "    path TEXT,"
               "    comment TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS pl_author("
               "    author_id INTEGER PRIMARY KEY,"
               "    author_name VARCHAR(128) NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS pl_book_title("
               "    book_title_id INTEGER PRIMARY KEY,"
               "    book_title_name VARCHAR(128) NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS pl_tag("
               "    tag_id INTEGER PRIMARY KEY,"
               "    tag_name VARCHAR(128) NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS pl_paper2tag("
               "    paper_id INTEGER NOT NULL,"
               "    tag_id INTEGER NOT NULL,"
               "    FOREIGN KEY(paper_id) REFERENCES pl_paper(paper_id) ON DELETE CASCADE,"
               "    FOREIGN KEY(tag_id) REFERENCES pl_tag(tag_id) ON DELETE CASCADE"
               "    UNIQUE (paper_id, tag_id))");

    query.exec("CREATE TABLE IF NOT EXISTS pl_paper2author("
               "    paper_id INTEGER NOT NULL,"
               "    author_id INTEGER NOT NULL,"
               "    FOREIGN KEY(paper_id) REFERENCES pl_paper(paper_id) ON DELETE CASCADE,"
               "    FOREIGN KEY(author_id) REFERENCES pl_author(author_id) ON DELETE CASCADE,"
               "    UNIQUE (paper_id, author_id))");
}

int DatabaseHelper::getBookTitleId(const string& bookTitle)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "SELECT book_title_id FROM pl_book_title WHERE book_title_name = \"%s\"",
            bookTitle.c_str());

    QSqlQuery query;
    query.exec(qBuf);

    if (!query.last()) return -1;
    return query.value(0).toInt();
}

int DatabaseHelper::getAuthorId(const string& author)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "SELECT author_id FROM pl_author WHERE author_name = \"%s\"",
            author.c_str());

    QSqlQuery query;
    query.exec(qBuf);

    if (!query.last()) return -1;
    return query.value(0).toInt();
}

int DatabaseHelper::getTagId(const string& tag)
{
    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "SELECT tag_id FROM pl_tag WHERE tag_name = \"%s\"",
            tag.c_str());

    QSqlQuery query;
    query.exec(qBuf);

    if (!query.last()) return -1;
    return query.value(0).toInt(0);
}
