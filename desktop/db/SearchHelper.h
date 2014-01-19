#ifndef SEARCHHELPER_H
#define SEARCHHELPER_H

#include <map>
#include <string>

class SearchHelper
{
public:
    enum FilterType
    {
        None = 0,
        Year,
        BookTitle,
        Author,
        Tag
    };

    typedef std::pair<FilterType, std::string> Filter;
    typedef std::multimap<FilterType, std::string> Filters;

public:
    SearchHelper();
    ~SearchHelper();

    std::string getSqlQueryString() const;

    void addFilter(const Filter& filter);

    void clear();

private:
    Filters filters_;

private:
    bool hasFilter(const Filter& filter);

    std::string getYearClause() const;
    std::string getBookTitleClause() const;
    std::string getAuthorClause() const;
    std::string getTagClause() const;
};

#endif // SEARCHHELPER_H
