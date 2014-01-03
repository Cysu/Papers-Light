#include "utils/utils.h"

using std::string;
using std::vector;

QStringList stats2QStringList(const vector<CategoryStats>& stats)
{
    QStringList ret;
    for (vector<CategoryStats>::size_type i = 0; i < stats.size(); ++i) {
        ret.push_back(stats[i].toString().c_str());
    }
    return ret;
}

QStringList papers2QStringList(const vector<Paper>& papers)
{
    QStringList ret;
    for (vector<Paper>::size_type i = 0; i < papers.size(); ++i) {
        ret.push_back(papers[i].getTitle().c_str());
    }
    return ret;
}
