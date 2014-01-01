#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    QAction* openDatabaseAction_;

private slots:
    void setCodecs(const char* codec = "UTF-8");
    void openDatabase();

private:

    void createMenus();
    void createPanels();
};

#endif // MAINWINDOW_H
