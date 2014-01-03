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

    /**
     * @todo The item should be CategoryStats vector. It's CategoryList and its
     * delegator's responsibility to parse and display the information.
     * The same issue should be concerned in PaperList and PaperInfoTable.
     */
    void addItem(const QString& content);
    void addItems(const QStringList& contents);

    void clear();

signals:
    void itemSelectedOnly(int index);

private:
    QString title_;

    QLabel* titleLabel_;
    QListView* view_;
    QStandardItemModel* model_;

private slots:
    void itemDoubleClicked(const QModelIndex& index);

private:
    void createPanels();
};

#endif // CATEGORYLIST_H
