#ifndef BUBBLESEDIT_H
#define BUBBLESEDIT_H

#include <string>
#include <vector>
#include <QWidget>
#include <QLineEdit>

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
    std::vector<QLineEdit*> bubbles_;
    QLineEdit* newBubble_;

private:
    void createPanels();
};

#endif // BUBBLESEDIT_H
