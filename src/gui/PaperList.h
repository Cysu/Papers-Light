#ifndef PAPERLIST_H
#define PAPERLIST_H

#include "common/Paper.h"
#include <vector>
#include <QWidget>
#include <QListView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

class PaperListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    enum DataRole
    {
        TitleRole = Qt::UserRole,
        YearRole,
        BookTitleRole,
        AuthorRole,
        NumberRoles
    };

public:
    explicit PaperListDelegate(QWidget* parent = 0);
    ~PaperListDelegate();

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const;

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:

};

class PaperList : public QWidget
{
    Q_OBJECT

public:
    explicit PaperList(QWidget *parent = 0);
    ~PaperList();

    void addItem(const Paper& paper);
    void addItems(const std::vector<Paper>& papers);

    void clear();

signals:
    void itemSelectedOnly(int index);

private:
    QListView* view_;
    QStandardItemModel* model_;
    PaperListDelegate* delegate_;

private slots:
    void itemDoubleClicked(const QModelIndex& index);

private:
    void createPanels();
};

#endif // PAPERLIST_H
