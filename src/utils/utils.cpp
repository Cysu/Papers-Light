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
