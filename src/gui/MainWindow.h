#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gui/CategoryList.h"
#include "gui/SearchBar.h"
#include "gui/PaperList.h"
#include "gui/PaperInfoTable.h"
#include "db/DatabaseHelper.h"
#include "db/SearchHelper.h"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    QAction* openDatabaseAction_;

    CategoryList* yearList_;
    CategoryList* bookTitleList_;
    CategoryList* authorList_;
    CategoryList* tagList_;
    SearchBar* searchBar_;
    PaperList* paperList_;
    PaperInfoTable* paperInfoTable_;

    std::vector<CategoryStats> yearStats_;
    std::vector<CategoryStats> bookTitleStats_;
    std::vector<CategoryStats> authorStats_;
    std::vector<CategoryStats> tagStats_;

    DatabaseHelper databaseHelper_;
    SearchHelper searchHelper_;

private slots:
    void openDatabase();
    void yearSelectedOnly(int index);
    void bookTitleSelectedOnly(int index);
    void authorSelectedOnly(int index);
    void tagSelectedOnly(int index);

private:
    void setCodecs(const char* codec = "UTF-8");

    void createMenus();
    void createPanels();

    void refreshAllPanels();
    void refreshPaperList();
};

#endif // MAINWINDOW_H
