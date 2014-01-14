#ifndef BUBBLESEDIT_H
#define BUBBLESEDIT_H

#include <string>
#include <vector>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class Bubble : public QLineEdit
{
    Q_OBJECT

public:
    explicit Bubble(QWidget* parent = 0);
    explicit Bubble(const QString& text, QWidget* parent = 0);
    ~Bubble();

private:
    void init();
};

class BubblesEdit : public QWidget
{
    Q_OBJECT

public:
    explicit BubblesEdit(QWidget* parent = 0);
    ~BubblesEdit();

    std::vector<std::string> getBubbles() const;

public slots:
    void setBubbles(const std::vector<std::string>& contents);
    void clear();

private:
    std::vector<Bubble*> bubbles_;
    QPushButton* newBubble_;

private slots:
    void addNewBubble();
    void removeEmptyBubbles();

private:
    void createPanels();
};

#endif // BUBBLESEDIT_H
