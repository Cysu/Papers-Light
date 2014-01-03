#include "db/SearchHelper.h"
#include "common/const.h"

using std::string;
using std::multimap;
using std::pair;

SearchHelper::SearchHelper()
    : filters_()
{

}

SearchHelper::~SearchHelper()
{

}

string SearchHelper::getSqlQueryString() const
{

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
