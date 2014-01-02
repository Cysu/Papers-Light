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

private:
    QListView* view_;
    QStandardItemModel* model_;

private:
    void createPanels();
};

#endif // PAPERLIST_H
