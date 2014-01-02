#ifndef PAPERINFOTABLE_H
#define PAPERINFOTABLE_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>

class PaperInfoTable : public QWidget
{
    Q_OBJECT

public:
    explicit PaperInfoTable(QWidget* parent = 0);
    ~PaperInfoTable();

private:
    QLineEdit* year_;
    QLineEdit* bookTitle_;
    QLineEdit* title_;
    QLineEdit* authors_;
    QLineEdit* tags_;
    QTextEdit* comment_;

private:
    void createPanels();
};

#endif // PAPERINFOTABLE_H
