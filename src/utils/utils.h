#ifndef UTILS_H
#define UTILS_H

#include "common/CategoryStats.h"
#include <string>
#include <vector>
#include <QStringList>

QStringList stats2QStringList(const std::vector<CategoryStats>& stats);

#endif // UTILS_H
