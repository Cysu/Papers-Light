#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class SearchBar : public QWidget
{
    Q_OBJECT

public:
    explicit SearchBar(QWidget *parent = 0);
    ~SearchBar();

private:
    QLineEdit* searchText_;
    QPushButton* searchButton_;

private:
    void createPanels();
};

#endif // SEARCHBAR_H
