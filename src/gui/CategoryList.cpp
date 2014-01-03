#include "gui/CategoryList.h"
#include <QVBoxLayout>

CategoryList::CategoryList(const QString& title, QWidget* parent)
    : QWidget(parent),
      title_(title),
      titleLabel_(nullptr),
      view_(nullptr),
      model_(nullptr)
{
    createPanels();
}

CategoryList::~CategoryList()
{

}

void CategoryList::addItem(const QString& content)
{
    QStandardItem* item = new QStandardItem(content);
    item->setEditable(false);
    model_->appendRow(item);
}

void CategoryList::addItems(const QStringList& contents)
{
    for (int i = 0; i < contents.size(); ++i) {
        QStandardItem* item = new QStandardItem(contents[i]);
        item->setEditable(false);
        model_->appendRow(item);
    }
}

void CategoryList::clear()
{
    model_->clear();
}

void CategoryList::itemDoubleClicked(const QModelIndex& index)
{
    emit itemSelectedOnly(index.row());
}

void CategoryList::createPanels()
{
    titleLabel_ = new QLabel(title_);

    view_ = new QListView;
    model_ = new QStandardItemModel;

    view_->setModel(model_);
    connect(view_, &QListView::doubleClicked, this, &CategoryList::itemDoubleClicked);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(titleLabel_);
    layout->addWidget(view_);

    setLayout(layout);
}
