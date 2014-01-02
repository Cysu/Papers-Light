#ifndef CATEGORYLIST_H
#define CATEGORYLIST_H

#include <QWidget>
#include <QLabel>
#include <QListView>
#include <QStandardItemModel>

class CategoryList : public QWidget
{
    Q_OBJECT

public:
    CategoryList(const QString& title, QWidget* parent = 0);
    ~CategoryList();

private:
    QString title_;

    QLabel* titleLabel_;
    QListView* view_;
    QStandardItemModel* model_;

private:
    void createPanels();
};

#endif // CATEGORYLIST_H
