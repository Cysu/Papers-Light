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
    QSqlQuery query;
    query.prepare("SELECT year, book_title_id, title, path, comment FROM pl_paper WHERE paper_id = :paper_id");
    query.bindValue(":paper_id", paperId);
    query.exec();

    Paper paper;
    if (!query.last()) return paper;

    paper.setId(paperId);
    paper.setYear(query.value(0).toInt());
    paper.setBookTitle(getBookTitle(query.value(1).toInt()));
    paper.setTitle(query.value(2).toString().toStdString());
    paper.setPath(query.value(3).toString().toStdString());
    paper.setComment(query.value(4).toString().toStdString());

    query.prepare("SELECT author_id FROM pl_paper2author WHERE paper_id = :paper_id ORDER BY rowid ASC");
    query.bindValue(":paper_id", paperId);
    query.exec();

    vector<string> authors;
    while (query.next()) {
        int authorId = query.value(0).toInt();
        authors.push_back(getAuthor(authorId));
    }

    query.prepare("SELECT tag_id FROM pl_paper2tag WHERE paper_id = :paper_id");
    query.bindValue(":paper_id", paperId);
    query.exec();

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
    QSqlQuery query;
    query.prepare("SELECT book_title_name FROM pl_book_title WHERE book_title_id = :book_title_id");
    query.bindValue(":book_title_id", bookTitleId);
    query.exec();

    if (!query.last()) return string();
    return query.value(0).toString().toStdString();
}

string DatabaseHelper::getAuthor(int authorId)
{
    QSqlQuery query;
    query.prepare("SELECT author_name FROM pl_author WHERE author_id = :author_id");
    query.bindValue(":author_id", authorId);
    query.exec();

    if (!query.last()) return string();
    return query.value(0).toString().toStdString();
}

string DatabaseHelper::getTag(int tagId)
{
    QSqlQuery query;
    query.prepare("SELECT tag_name FROM pl_tag WHERE tag_id = :tag_id");
    query.bindValue(":tag_id", tagId);
    query.exec();

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
        query.prepare("SELECT COUNT(*) FROM pl_paper WHERE year = :year");
        query.bindValue(":year", years[i]);
        query.exec();
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
        query.prepare("SELECT COUNT(*) FROM pl_paper WHERE book_title_id = :book_title_id");
        query.bindValue(":book_title_id", bookTitlesId[i]);
        query.exec();
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
        query.prepare("SELECT COUNT(*) FROM pl_paper2author WHERE author_id = :author_id");
        query.bindValue(":author_id", authorsId[i]);
        query.exec();
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
        query.prepare("SELECT COUNT(*) FROM pl_paper2tag WHERE tag_id = :tag_id");
        query.bindValue(":tag_id", tagsId[i]);
        query.exec();
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

    QSqlQuery query;
    if (paper.getId() <= 0) {
        query.prepare("INSERT INTO pl_paper(year, book_title_id, title, path, comment) "
                      "VALUES(:year, :book_title_id, :title, :path, :comment)");
        query.bindValue(":year", paper.getYear());
        query.bindValue(":book_title_id", bookTitleId);
        query.bindValue(":title", paper.getTitle().c_str());
        query.bindValue(":path", paper.getPath().c_str());
        query.bindValue(":comment", paper.getComment().c_str());
    } else {
        query.prepare("INSERT INTO pl_paper(paper_id, year, book_title_id, title, path, comment) "
                      "VALUES(:paper_id, :year, :book_title_id, :title, :path, :comment)");
        query.bindValue(":paper_id", paper.getId());
        query.bindValue(":year", paper.getYear());
        query.bindValue(":book_title_id", bookTitleId);
        query.bindValue(":title", paper.getTitle().c_str());
        query.bindValue(":path", paper.getPath().c_str());
        query.bindValue(":comment", paper.getComment().c_str());
    }
    query.exec();

    int paperId = query.lastInsertId().toInt();
    QVariantList paperIdList, authorIdList;
    for (int i = 0; i < static_cast<int>(authorsId.size()); ++i) {
        paperIdList.append(paperId);
        authorIdList.append(authorsId[i]);
    }

    query.prepare("INSERT INTO pl_paper2author(paper_id, author_id) "
                  "VALUES(:paper_id, :author_id)");
    query.bindValue(":paper_id", paperIdList);
    query.bindValue(":author_id", authorIdList);
    query.execBatch();

    paperIdList.clear();
    QVariantList tagIdList;
    for (int i = 0; i < static_cast<int>(tagsId.size()); ++i) {
        paperIdList.append(paperId);
        tagIdList.append(tagsId[i]);
    }

    query.prepare("INSERT INTO pl_paper2tag(paper_id, tag_id) "
                  "VALUES(:paper_id, :tag_id)");
    query.bindValue(":paper_id", paperIdList);
    query.bindValue(":tag_id", tagIdList);
    query.execBatch();

    return paperId;
}

int DatabaseHelper::addBookTitle(const string& bookTitle)
{
    QSqlQuery query;
    query.prepare("INSERT INTO pl_book_title(book_title_name) "
                  "VALUES(:book_title_name)");
    query.bindValue(":book_title_name", bookTitle.c_str());
    query.exec();

    return query.lastInsertId().toInt();
}

int DatabaseHelper::addAuthor(const string& author)
{
    QSqlQuery query;
    query.prepare("INSERT INTO pl_author(author_name) "
                  "VALUES(:author_name)");
    query.bindValue(":author_name", author.c_str());
    query.exec();

    return query.lastInsertId().toInt();
}

int DatabaseHelper::addTag(const string& tag)
{
    QSqlQuery query;
    query.prepare("INSERT INTO pl_tag(tag_name) "
                  "VALUES(:tag_name)");
    query.bindValue(":tag_name", tag.c_str());
    query.exec();

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
    QSqlQuery query;
    query.prepare("DELETE FROM pl_paper WHERE paper_id = :paper_id");
    query.bindValue(":paper_id", paperId);
    query.exec();

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
    QSqlQuery query;
    query.prepare("DELETE FROM pl_book_title WHERE book_title_id = :book_title_id");
    query.bindValue(":book_title_id", bookTitleId);
    query.exec();

    query.prepare("SELECT paper_id FROM pl_paper WHERE book_title_id = :book_title_id");
    query.bindValue(":book_title_id", bookTitleId);
    query.exec();
    while (query.next()) {
        int paperId = query.value(0).toInt();
        removePaper(paperId);
    }
}

void DatabaseHelper::removeAuthor(int authorId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM pl_author WHERE author_id = :author_id");
    query.bindValue(":author_id", authorId);
    query.exec();
}

void DatabaseHelper::removeTag(int tagId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM pl_tag WHERE tag_id = :tag_id");
    query.bindValue(":tag_id", tagId);
    query.exec();
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
    QSqlQuery query;
    query.prepare("SELECT book_title_id FROM pl_book_title WHERE book_title_name = :book_title_name");
    query.bindValue(":book_title_name", bookTitle.c_str());
    query.exec();

    if (!query.last()) return -1;
    return query.value(0).toInt();
}

int DatabaseHelper::getAuthorId(const string& author)
{
    QSqlQuery query;
    query.prepare("SELECT author_id FROM pl_author WHERE author_name = :author_name");
    query.bindValue(":author_name", author.c_str());
    query.exec();

    if (!query.last()) return -1;
    return query.value(0).toInt();
}

int DatabaseHelper::getTagId(const string& tag)
{
    QSqlQuery query;
    query.prepare("SELECT tag_id FROM pl_tag WHERE tag_name = :tag_name");
    query.bindValue(":tag_name", tag.c_str());
    query.exec();

    if (!query.last()) return -1;
    return query.value(0).toInt(0);
}
