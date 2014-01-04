#ifndef PAPERLIST_H
#define PAPERLIST_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

class PaperList : public QWidget
{
    Q_OBJECT

public:
    explicit PaperList(QWidget *parent = 0);
    ~PaperList();

    void addItem(const QString& content);
    void addItems(const QStringList& contents);

    void clear();

signals:
    void itemSelectedOnly(int index);

private:
    QListView* view_;
    QStandardItemModel* model_;

private:
    void itemDoubleClicked(const QModelIndex& index);

private:
    void createPanels();
};

#endif // PAPERLIST_H
