#include "common/Paper.h"

using std::vector;
using std::string;

Paper::Paper()
    : year_(-1),
      bookTitle_(),
      title_(),
      authors_(),
      tags_(),
      comment_()
{

}

Paper::Paper(int year,
             const string& bookTitle,
             const string& title,
             const vector<string>& authors,
             const vector<string>& tags,
             const string& comment)
    : year_(year),
      bookTitle_(bookTitle),
      title_(title),
      authors_(authors),
      tags_(tags),
      comment_(comment)
{

}

Paper::~Paper()
{

}

bool Paper::isNull() const
{
    return year_ == -1;
}
