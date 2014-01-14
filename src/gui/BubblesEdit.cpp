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

    FlowLayout* layout = new FlowLayout;
    layout->addWidget(newBubble_);

    setLayout(layout);
}


