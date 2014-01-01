#include "common/Paper.h"

using std::vector;
using std::string;

Paper::Paper(int year,
             const string& bookTitle,
             const string& title,
             const vector<string>& authors)
    : year_(year),
      bookTitle_(bookTitle),
      title_(title),
      authors_(authors)
{

}
