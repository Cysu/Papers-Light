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

void CategoryList::createPanels()
{
    titleLabel_ = new QLabel(title_);

    view_ = new QListView;
    model_ = new QStandardItemModel;

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(titleLabel_);
    layout->addWidget(view_);

    setLayout(layout);
}
