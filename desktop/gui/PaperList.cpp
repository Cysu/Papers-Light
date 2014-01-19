#include "gui/PaperList.h"
#include <string>
#include <QVBoxLayout>
#include <QPainter>
#include <QApplication>

using std::vector;
using std::string;

static QStringList toQStringList(const vector<string>& list)
{
    QStringList ret;
    for (vector<string>::size_type i = 0; i < list.size(); ++i) {
        ret.push_back(list[i].c_str());
    }
    return ret;
}

static void fillData(QStandardItem* item, const Paper& paper)
{
    item->setData(paper.getTitle().c_str(), PaperListDelegate::TitleRole);
    item->setData(paper.getYear(), PaperListDelegate::YearRole);
    item->setData(paper.getBookTitle().c_str(), PaperListDelegate::BookTitleRole);
    item->setData(toQStringList(paper.getAuthors()), PaperListDelegate::AuthorRole);
}

PaperListDelegate::PaperListDelegate(QWidget* parent)
    : QStyledItemDelegate(parent)
{

}

PaperListDelegate::~PaperListDelegate()
{

}

void PaperListDelegate::paint(QPainter* painter,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->save();

    QPen titlePen = QPen(Qt::black);
    QPen infoPen = QPen(QColor(17, 17, 17));

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, QColor(153, 153, 153));
        titlePen.setColor(Qt::white);
        infoPen.setColor(QColor(238, 238, 238));
    }

    QString title = index.data(TitleRole).toString();
    QString year = QString::number(index.data(YearRole).toInt());
    QString bookTitle = index.data(BookTitleRole).toString();
    QStringList authors = index.data(AuthorRole).toStringList();

    QRect titleRect = option.rect;
    titleRect.setTopLeft(titleRect.topLeft() + QPoint(8, 5));
    titleRect.setHeight(20);

    QRect yearBookTitleRect = QRect(titleRect.bottomLeft() + QPoint(0, 5),
                                    QSize(titleRect.width(), titleRect.height()-5));

    QRect authorRect = QRect(yearBookTitleRect.bottomLeft() + QPoint(0, 5),
                             QSize(yearBookTitleRect.width(), yearBookTitleRect.height()));

    QFont titleFont = QApplication::font();
    titleFont.setBold(true);
    QFont infoFont = QApplication::font();
    infoFont.setPointSize(infoFont.pointSize() - 2);

    painter->setPen(titlePen);
    painter->setFont(titleFont);
    painter->drawText(titleRect, title);
    painter->setFont(infoFont);
    painter->setPen(infoPen);
    painter->drawText(yearBookTitleRect, year + " " + bookTitle);
    painter->drawText(authorRect, authors.join(", "));
    painter->restore();
}

QSize PaperListDelegate::sizeHint(const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const
{
    return QSize(450, 70);
}


PaperList::PaperList(QWidget* parent)
    : QWidget(parent),
      view_(nullptr),
      model_(nullptr),
      delegate_(nullptr)
{
    createPanels();
}

PaperList::~PaperList()
{

}

void PaperList::addItem(const Paper& paper)
{
    QStandardItem* item = new QStandardItem;
    item->setEditable(false);
    fillData(item, paper);
    model_->appendRow(item);
}

void PaperList::addItems(const vector<Paper>& papers)
{
    for (vector<Paper>::size_type i = 0; i < papers.size(); ++i) {
        QStandardItem* item = new QStandardItem;
        item->setEditable(false);
        fillData(item, papers[i]);
        model_->appendRow(item);
    }
}

void PaperList::clear()
{
    model_->clear();
}

void PaperList::itemDoubleClicked(const QModelIndex& index)
{
    emit itemSelectedOnly(index.row());
}

void PaperList::createPanels()
{
    view_ = new QListView;
    model_ = new QStandardItemModel;
    delegate_ = new PaperListDelegate;

    view_->setModel(model_);
    view_->setItemDelegate(delegate_);
    connect(view_, &QListView::doubleClicked, this, &PaperList::itemDoubleClicked);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(view_);

    setLayout(layout);
}
