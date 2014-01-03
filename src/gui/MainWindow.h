#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gui/CategoryList.h"
#include "gui/SearchBar.h"
#include "gui/PaperList.h"
#include "gui/PaperInfoTable.h"
#include "db/DatabaseHelper.h"
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

    DatabaseHelper db_;

private slots:
    void setCodecs(const char* codec = "UTF-8");
    void openDatabase();

private:
    void createMenus();
    void createPanels();

    void refreshAllPanels();
};

#endif // MAINWINDOW_H
