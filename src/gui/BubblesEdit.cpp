#include "gui/BubblesEdit.h"
#include "gui/FlowLayout.h"

using std::string;
using std::vector;

Bubble::Bubble(QWidget* parent)
    : QLineEdit(parent)
{
    init();
}

Bubble::Bubble(const QString& text, QWidget* parent)
    : QLineEdit(text, parent)
{
    init();
}

Bubble::~Bubble()
{

}

void Bubble::init()
{
    setStyleSheet("QLineEdit {"
                  "  border: 1px solid #AAAAAA;"
                  "  border-radius: 5px;"
                  "  font-size: 11pt;"
                  "}");
}




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
    for (vector<Bubble*>::size_type i = 0; i < bubbles_.size(); ++i) {
        bubbles[i] = bubbles_[i]->text().toStdString();
    }
    return bubbles;
}

void BubblesEdit::setBubbles(const vector<string>& contents)
{
    clear();

    bubbles_.resize(contents.size(), nullptr);
    for (vector<string>::size_type i = 0; i < contents.size(); ++i) {
        bubbles_[i] = new Bubble(contents[i].c_str());
        connect(bubbles_[i], &Bubble::editingFinished, this, &BubblesEdit::removeEmptyBubbles);
        layout()->addWidget(bubbles_[i]);
    }
}

void BubblesEdit::clear()
{
    for (vector<Bubble*>::size_type i = 0; i < bubbles_.size(); ++i) {
        layout()->removeWidget(bubbles_[i]);
        delete bubbles_[i];
        bubbles_[i] = nullptr;
    }
    bubbles_.clear();
}

void BubblesEdit::addNewBubble()
{
    Bubble* bubble = new Bubble;
    bubbles_.push_back(bubble);
    connect(bubble, &Bubble::editingFinished, this, &BubblesEdit::removeEmptyBubbles);
    layout()->addWidget(bubbles_.back());
}

void BubblesEdit::removeEmptyBubbles()
{
    for (vector<Bubble*>::iterator it = bubbles_.begin(); it != bubbles_.end(); ) {
        if ((*it)->text().isEmpty()) {
            delete *it;
            it = bubbles_.erase(it);
        } else ++it;
    }
}

void BubblesEdit::createPanels()
{
    newBubble_ = new QPushButton("+");
    connect(newBubble_, &QPushButton::clicked, this, &BubblesEdit::addNewBubble);

    QFont font = newBubble_->font();
    font.setBold(true);
    newBubble_->setFont(font);

    newBubble_->setMaximumHeight(24);
    newBubble_->setMaximumWidth(24);

    FlowLayout* layout = new FlowLayout;
    layout->addWidget(newBubble_);

    setLayout(layout);
}


