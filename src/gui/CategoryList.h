#ifndef CATEGORYLIST_H
#define CATEGORYLIST_H

#include "common/CategoryStats.h"
#include <vector>
#include <QWidget>
#include <QLabel>
#include <QListView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

class CategoryListDelegate : public QStyledItemDelegate
{
     Q_OBJECT

public:
    enum DataRole
    {
        NameRole = Qt::UserRole,
        CountRole
    };

public:
    explicit CategoryListDelegate(QWidget* parent = 0);
    ~CategoryListDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

class CategoryList : public QWidget
{
    Q_OBJECT

public:
    CategoryList(const QString& title, QWidget* parent = 0);
    ~CategoryList();

    /**
     * @todo The item should be CategoryStats vector. It's CategoryList and its
     * delegator's responsibility to parse and display the information.
     * The same issue should be concerned in PaperList and PaperInfoTable.
     */
    void addItem(const CategoryStats& stats);
    void addItems(const std::vector<CategoryStats>& statsList);

    void clear();

signals:
    void itemSelectedOnly(int index);

private:
    QString title_;

    QLabel* titleLabel_;
    QListView* view_;
    QStandardItemModel* model_;
    CategoryListDelegate* delegate_;

private slots:
    void itemDoubleClicked(const QModelIndex& index);

private:
    void createPanels();
};

#endif // CATEGORYLIST_H
