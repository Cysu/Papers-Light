#include "common/Paper.h"

using std::vector;
using std::string;

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
