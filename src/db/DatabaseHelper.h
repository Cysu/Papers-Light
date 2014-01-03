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

    Paper getPaper(int paperId);

    std::vector<CategoryStats> getYearStats();
    std::vector<CategoryStats> getBookTitleStats();
    std::vector<CategoryStats> getAuthorStats();
    std::vector<CategoryStats> getTagStats();

    int addPaper(const Paper& paper);
    int addBookTitle(const std::string& bookTitle);
    int addAuthor(const std::string& author);

    bool updatePaper(int paperId, const Paper& paper);

    bool removePaper(int paperId);

private:
    QSqlDatabase db_;

private:
    void createTables();

    int getBookTitleId(const std::string& bookTitle);
    int getAuthorId(const std::string& author);
};

#endif // DATABASEHELPER_H
