#include "gui/CategoryList.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QApplication>

using std::string;
using std::vector;

static void fillData(QStandardItem* item, const CategoryStats& stats)
{
    item->setData(stats.getName().c_str(), CategoryListDelegate::NameRole);
    item->setData(stats.getCount(), CategoryListDelegate::CountRole);
}

CategoryListDelegate::CategoryListDelegate(QWidget* parent)
    : QStyledItemDelegate(parent)
{

}

CategoryListDelegate::~CategoryListDelegate()
{

}

void CategoryListDelegate::paint(QPainter* painter,
                                 const QStyleOptionViewItem& option,
                                 const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->save();

    QPen pen = QPen(Qt::black);

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, QColor(153, 153, 153));
        pen.setColor(Qt::white);
    }

    QString name = index.data(NameRole).toString();
    QString count = QString::number(index.data(CountRole).toInt());

    QRect rect = option.rect;
    rect.setLeft(rect.left() + 5);

    painter->setPen(pen);
    painter->drawText(rect, name + " (" + count + ")");
    painter->restore();
}

QSize CategoryListDelegate::sizeHint(const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const
{
    return QSize(250, 24);
}




CategoryList::CategoryList(const QString& title, QWidget* parent)
    : QWidget(parent),
      title_(title),
      titleLabel_(nullptr),
      view_(nullptr),
      model_(nullptr),
      delegate_(nullptr)
{
    createPanels();
}

CategoryList::~CategoryList()
{

}

void CategoryList::addItem(const CategoryStats& stats)
{
    QStandardItem* item = new QStandardItem;
    item->setEditable(false);
    fillData(item, stats);
    model_->appendRow(item);
}

void CategoryList::addItems(const vector<CategoryStats>& stats)
{
    for (vector<CategoryStats>::size_type i = 0; i < stats.size(); ++i) {
        QStandardItem* item = new QStandardItem;
        item->setEditable(false);
        fillData(item, stats[i]);
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
    delegate_ = new CategoryListDelegate;

    view_->setModel(model_);
    view_->setItemDelegate(delegate_);
    connect(view_, &QListView::doubleClicked, this, &CategoryList::itemDoubleClicked);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(titleLabel_);
    layout->addWidget(view_);

    setLayout(layout);
    setMaximumWidth(300);
}
