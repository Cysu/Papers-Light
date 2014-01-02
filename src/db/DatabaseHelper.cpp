#include "db/DatabaseHelper.h"
#include <QSqlQuery>
#include <QVariant>

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

void DatabaseHelper::createTables()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS pl_paper("
               "    paper_id INTEGER PRIMARY KEY,"
               "    year SMALLINT,"
               "    book_title INT,"
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

long long DatabaseHelper::getLastInsertRowId()
{
    QSqlQuery query;
    query.exec("SELECT LAST_INSERT_ROWID()");
    return query.value(0).toLongLong();
}
