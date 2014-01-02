#ifndef CATEGORYSTATS_H
#define CATEGORYSTATS_H

#include <string>

class CategoryStats
{
public:
    CategoryStats();
    CategoryStats(const std::string& name, int count);
    ~CategoryStats();

    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }

    int getCount() const { return count_; }
    void setCount(int count) { count_ = count; }

    std::string toString() const;

private:
    std::string name_;
    int count_;
};

#endif // CATEGORYSTATS_H
