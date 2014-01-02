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

void PaperList::createPanels()
{
    view_ = new QListView;
    model_ = new QStandardItemModel;

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(view_);

    setLayout(layout);
}
