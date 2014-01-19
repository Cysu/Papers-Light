#ifndef UTILS_H
#define UTILS_H

#include "common/CategoryStats.h"
#include "common/Paper.h"
#include <string>
#include <vector>
#include <QStringList>

QStringList stats2QStringList(const std::vector<CategoryStats>& stats);
QStringList papers2QStringList(const std::vector<Paper>& papers);

#endif // UTILS_H
