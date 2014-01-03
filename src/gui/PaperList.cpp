#include "gui/PaperList.h"
#include <QVBoxLayout>

PaperList::PaperList(QWidget* parent)
    : QWidget(parent),
      view_(nullptr),
      model_(nullptr)
{
    createPanels();
}

PaperList::~PaperList()
{

}

void PaperList::addItem(const QString& content)
{
    QStandardItem* item = new QStandardItem(content);
    item->setEditable(false);
    model_->appendRow(item);
}

void PaperList::addItems(const QStringList& contents)
{
    for (int i = 0; i < contents.size(); ++i) {
        QStandardItem* item = new QStandardItem(contents[i]);
        item->setEditable(false);
        model_->appendRow(item);
    }
}

void PaperList::clear()
{
    model_->clear();
}

void PaperList::createPanels()
{
    view_ = new QListView;
    model_ = new QStandardItemModel;

    view_->setModel(model_);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(view_);

    setLayout(layout);
}
