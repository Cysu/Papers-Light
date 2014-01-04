#include "common/Paper.h"

using std::vector;
using std::string;

Paper::Paper()
    : id_(-1),
      year_(),
      bookTitle_(),
      title_(),
      authors_(),
      tags_(),
      comment_()
{

}

Paper::Paper(int id,
             int year,
             const string& bookTitle,
             const string& title,
             const vector<string>& authors,
             const vector<string>& tags,
             const string& comment)
    : id_(id),
      year_(year),
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
    return id_ == -1;
}
