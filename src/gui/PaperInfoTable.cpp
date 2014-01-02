#include "gui/PaperInfoTable.h"
#include <QLabel>
#include <QGridLayout>

PaperInfoTable::PaperInfoTable(QWidget* parent)
    : QWidget(parent),
      year_(nullptr),
      bookTitle_(nullptr),
      title_(nullptr),
      authors_(nullptr),
      tags_(nullptr),
      comment_(nullptr)
{
    createPanels();
}

PaperInfoTable::~PaperInfoTable()
{

}

void PaperInfoTable::createPanels()
{
    year_ = new QLineEdit;
    bookTitle_ = new QLineEdit;
    title_ = new QLineEdit;
    authors_ = new QLineEdit;
    tags_ = new QLineEdit;
    comment_ = new QTextEdit;

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Year")), 0, 0);
    layout->addWidget(year_, 0, 1, 1, 1);
    layout->addWidget(new QLabel(tr("Book Title")), 0, 2);
    layout->addWidget(bookTitle_, 0, 3, 1, 3);
    layout->addWidget(new QLabel(tr("Title")), 1, 0);
    layout->addWidget(title_, 1, 1, 1, 5);
    layout->addWidget(new QLabel(tr("Authors")), 2, 0);
    layout->addWidget(authors_, 2, 1, 1, 5);
    layout->addWidget(new QLabel(tr("Tags")), 3, 0);
    layout->addWidget(tags_, 3, 1, 1, 5);
    layout->addWidget(comment_, 4, 0, 1, 6);

    setLayout(layout);
}
