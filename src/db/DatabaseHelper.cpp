#include "db/DatabaseHelper.h"
#include "common/const.h"
#include <QSqlQuery>
#include <QVariant>

using std::string;
using std::vector;

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

    char qBuf[BUFSIZE];
    sprintf(qBuf,
            "INSERT INTO pl_paper(year, book_title_id, title) "
            "VALUES(%d, %d, \"%s\")",
            paper.getYear(), bookTitleId, paper.getTitle().c_str());

    QSqlQuery query;
    query.exec(qBuf);

    int paperId = query.lastInsertId().toInt();

    for (int i = 0; i < static_cast<int>(authorsId.size()); ++i) {
        sprintf(qBuf,
                "INSERT INTO pl_paper2author(paper_id, author_id, author_order) "
                "VALUES(%d, %d, %d)",
                paperId, authorsId[i], i);

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

void DatabaseHelper::createTables()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS pl_paper("
               "    paper_id INTEGER PRIMARY KEY,"
               "    year SMALLINT,"
               "    book_title_id INT,"
               "    title TEXT NOT NULL,"
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
               "    author_order TINYINT NOT NULL,"
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

    if (query.size() == -1) return -1;
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

    if (query.size() == -1) return -1;
    return query.value(0).toInt();
}
