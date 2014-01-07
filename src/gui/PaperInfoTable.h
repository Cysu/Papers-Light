#ifndef PAPERINFOTABLE_H
#define PAPERINFOTABLE_H

#include "common/Paper.h"
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>

class PaperInfoTable : public QWidget
{
    Q_OBJECT

public:
    explicit PaperInfoTable(QWidget* parent = 0);
    ~PaperInfoTable();

    const Paper& getPaper() const;
    void setPaper(const Paper& paper);

    void saveChanges();

private:
    QLineEdit* year_;
    QLineEdit* bookTitle_;
    QLineEdit* title_;
    QLineEdit* authors_;
    QLineEdit* tags_;
    QLineEdit* path_;
    QTextEdit* comment_;

    Paper paper_;

private slots:
    void chooseFilePath();

private:
    void createPanels();
};

#endif // PAPERINFOTABLE_H
