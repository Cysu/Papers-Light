#include "gui/BubblesEdit.h"
#include "gui/FlowLayout.h"

using std::string;
using std::vector;

BubblesEdit::BubblesEdit(QWidget* parent)
    : QWidget(parent),
      bubbles_(),
      newBubble_(nullptr)
{
    createPanels();
}

BubblesEdit::~BubblesEdit()
{

}

vector<string> BubblesEdit::getBubbles() const
{
    vector<string> bubbles(bubbles_.size());
    for (vector<QLineEdit*>::size_type i = 0; i < bubbles_.size(); ++i) {
        bubbles[i] = bubbles_[i]->text().toStdString();
    }
    return bubbles;
}

void BubblesEdit::setBubbles(const vector<string>& contents)
{
    clear();

    bubbles_.resize(contents.size(), nullptr);
    for (vector<string>::size_type i = 0; i < contents.size(); ++i) {
        bubbles_[i] = new QLineEdit(contents[i].c_str());
        bubbles_[i]->setStyleSheet("QLineEdit {"
                                   "  border: 1px solid #AAAAAA;"
                                   "  border-radius: 5px;"
                                   "  font-size: 11pt;"
                                   "}");
        layout()->addWidget(bubbles_[i]);
    }
}

void BubblesEdit::clear()
{
    for (vector<QLineEdit*>::size_type i = 0; i < bubbles_.size(); ++i) {
        layout()->removeWidget(bubbles_[i]);
        delete bubbles_[i];
        bubbles_[i] = nullptr;
    }
    bubbles_.clear();
}

void BubblesEdit::createPanels()
{
    newBubble_ = new QPushButton("+");

    QFont font = newBubble_->font();
    font.setBold(true);
    newBubble_->setFont(font);

    newBubble_->setMaximumHeight(24);
    newBubble_->setMaximumWidth(24);
//    newBubble_->setStyleSheet("QPushButton {"
//                              "  border: 1px solid #39CCCC;"
//                              "  border-radius: 4px;"
//                              "  background-color: #7FDBFF;"
//                              "  font-weight: bold;"
//                              "  color: #0074D9;"
//                              "  width: 20px; height: 20px;"
//                              "}");

    FlowLayout* layout = new FlowLayout;
    layout->addWidget(newBubble_);

    setLayout(layout);
}


