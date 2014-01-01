#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include "common/Paper.h"
#include <string>

/**
 * @brief The DatabaseHelper class provides basic operations of papers
 * management.
 */
class DatabaseHelper
{
public:
    /**
     * @brief Load the database from the file specified. Connect to it and set
     * it as default across the application. Create all the tables if the file
     * is empty.
     *
     * @param filePath The database file path
     */
    DatabaseHelper(const std::string& filePath);
    ~DatabaseHelper();

    const Paper& getPaper(int paperId);

    int addPaper(const Paper& paper);

    bool updatePaper(int paperId, const Paper& paper);

    bool removePaper(int paperId);

private:

};

#endif // DATABASEHELPER_H
