#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include "common/Paper.h"
#include "common/CategoryStats.h"
#include <string>
#include <vector>
#include <QSqlDatabase>

/**
 * @brief The DatabaseHelper class provides basic operations of papers
 * management.
 */
class DatabaseHelper
{
public:
    enum SortOrder
    {
        SortByNone = 0,
        SortByName,
        SortByCount
    };

public:
    DatabaseHelper();
    ~DatabaseHelper();

    /**
     * @brief Load the database from the file specified. Connect to it and set
     * it as default across the application. Create all the tables if the file
     * is empty.
     *
     * @param filePath The database file path
     */
    void init(const QString& filePath);

    /**
     * @todo Should not expose SQL operations. Fix this.
     */
    QSqlQuery exec(const QString& query);

    Paper getPaper(int paperId);
    std::string getBookTitle(int bookTitleId);
    std::string getAuthor(int authorId);
    std::string getTag(int tagId);

    std::vector<CategoryStats> getYearStats(SortOrder sortOrder = SortByNone);
    std::vector<CategoryStats> getBookTitleStats(SortOrder sortOrder = SortByNone);
    std::vector<CategoryStats> getAuthorStats(SortOrder sortOrder = SortByNone);
    std::vector<CategoryStats> getTagStats(SortOrder sortOrder = SortByNone);

    int addPaper(const Paper& paper);
    int addBookTitle(const std::string& bookTitle);
    int addAuthor(const std::string& author);
    int addTag(const std::string& tag);

    int updatePaper(const Paper& paper);

    void removePaper(const Paper& paper);
    void removePaper(int paperId);
    void removeBookTitle(int bookTitleId);
    void removeAuthor(int authorId);
    void removeTag(int tagId);

private:
    QSqlDatabase db_;

private:
    void createTables();

    int getBookTitleId(const std::string& bookTitle);
    int getAuthorId(const std::string& author);
    int getTagId(const std::string& tag);
};

#endif // DATABASEHELPER_H
