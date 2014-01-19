#include "gui/SearchBar.h"
#include <QHBoxLayout>

SearchBar::SearchBar(QWidget* parent)
    : QWidget(parent),
      searchText_(nullptr),
      searchButton_(nullptr)
{
    createPanels();
}

SearchBar::~SearchBar()
{

}

void SearchBar::createPanels()
{
    searchText_ = new QLineEdit;
    searchButton_ = new QPushButton(tr("Search"));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(searchText_);
    layout->addWidget(searchButton_);

    setLayout(layout);
}
