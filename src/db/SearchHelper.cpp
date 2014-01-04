#include "db/SearchHelper.h"
#include "common/const.h"

using std::string;
using std::multimap;
using std::pair;

static string emptyClauseCheck(const string& clause)
{
    if (clause == "()") return "(1)";
    return clause;
}

SearchHelper::SearchHelper()
    : filters_()
{

}

SearchHelper::~SearchHelper()
{

}

string SearchHelper::getSqlQueryString() const
{
    string yearClause = getYearClause();
    string bookTitleClause = getBookTitleClause();
    string authorClause = getAuthorClause();
    string tagClause = getTagClause();

    string header = "SELECT pl_paper.paper_id FROM "
                    "pl_paper";

    string whereClause = " WHERE ";

    if (bookTitleClause != "(1)") {
        header += ", pl_book_title";
        whereClause += "(pl_paper.book_title_id = pl_book_title.book_title_id) AND ";
    }

    if (authorClause != "(1)") {
        header += ", pl_author, pl_paper2author";
        whereClause += "(pl_paper.paper_id = pl_paper2author.paper_id) AND "
                       "(pl_paper2author.author_id = pl_author.author_id) AND ";
    }

    if (tagClause != "(1)") {
        header += ", pl_tag, pl_paper2tag";
        whereClause += "(pl_paper.paper_id = pl_paper2tag.paper_id) AND "
                       "(pl_paper2tag.tag_id = pl_tag.tag_id) AND ";
    }

    string query = header + whereClause;
    query += yearClause + " AND ";
    query += bookTitleClause + " AND ";
    query += authorClause + " AND ";
    query += tagClause;
    query += " ORDER BY pl_paper.paper_id ASC";
    return query;
}

void SearchHelper::addFilter(const Filter& filter)
{
    if (hasFilter(filter)) return;
    filters_.insert(filter);
}

void SearchHelper::clear()
{
    filters_.clear();
}

bool SearchHelper::hasFilter(const Filter& filter)
{
    auto range = filters_.equal_range(filter.first);
    for (auto it = range.first; it != range.second; ++it) {
        if (filter.second == it->second) return true;
    }
    return false;
}

string SearchHelper::getYearClause() const
{
    string clause = "(";
    auto range = filters_.equal_range(Year);
    for (auto it = range.first; it != range.second; ) {
        clause += "(pl_paper.year = " + it->second + ")";
        if (++it != range.second) clause += " OR ";
    }
    clause += ")";
    return emptyClauseCheck(clause);
}

string SearchHelper::getBookTitleClause() const
{
    string clause = "(";
    auto range = filters_.equal_range(BookTitle);
    for (auto it = range.first; it != range.second; ) {
        clause += "(pl_book_title.book_title_name = \"" + it->second + "\")";
        if (++it != range.second) clause += " OR ";
    }
    clause += ")";
    return emptyClauseCheck(clause);
}

string SearchHelper::getAuthorClause() const
{
    string clause = "(";
    auto range = filters_.equal_range(Author);
    for (auto it = range.first; it != range.second; ) {
        clause += "(pl_author.author_name = \"" + it->second + "\")";
        if (++it != range.second) clause += " OR ";
    }
    clause += ")";
    return emptyClauseCheck(clause);
}

string SearchHelper::getTagClause() const
{
    string clause = "(";
    auto range = filters_.equal_range(Tag);
    for (auto it = range.first; it != range.second; ) {
        clause += "(pl_tag.tag_name = \"" + it->second + "\")";
        if (++it != range.second) clause += " OR ";
    }
    clause += ")";
    return emptyClauseCheck(clause);
}
