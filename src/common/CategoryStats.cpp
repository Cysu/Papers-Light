#include "common/CategoryStats.h"
#include "common/const.h"

using std::string;

CategoryStats::CategoryStats()
    : name_(),
      count_(0)
{

}

CategoryStats::CategoryStats(const string& name, int count)
    : name_(name),
      count_(count)
{

}

CategoryStats::~CategoryStats()
{

}

string CategoryStats::toString() const
{
    char buf[BUFSIZE];
    sprintf(buf, "%s (%d)", name_.c_str(), count_);
    return buf;
}
