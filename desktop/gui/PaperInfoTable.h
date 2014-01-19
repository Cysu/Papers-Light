#ifndef PAPERINFOTABLE_H
#define PAPERINFOTABLE_H

#include "common/Paper.h"
#include "gui/BubblesEdit.h"
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

    void clear();

signals:
    void paperSaved();
    void paperRemoved(Paper paper);

private:
    QLineEdit* year_;
    QLineEdit* bookTitle_;
    QLineEdit* title_;
    BubblesEdit* authors_;
    BubblesEdit* tags_;
    QLineEdit* path_;
    QTextEdit* comment_;

    Paper paper_;

private slots:
    void chooseFilePath();
    void savePaper();
    void removePaper();

private:
    void createPanels();
};

#endif // PAPERINFOTABLE_H
