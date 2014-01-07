#include "gui/PreferencesDialog.h"
#include "utils/PreferencesManager.h"
#include <QIcon>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

PreferencesDialog::PreferencesDialog(QWidget* parent)
    : QDialog(parent)
{
    createPanels();
    loadPreferences();
}

PreferencesDialog::~PreferencesDialog()
{

}

void PreferencesDialog::save()
{
    PreferencesManager& pm = PreferencesManager::instance();
    pm.setPaperFolderPath(paperFolderPath_->text());
    pm.setDatabaseFilePath(databaseFilePath_->text());

    if (!pm.save()) {
        QMessageBox::critical(this,
                              tr("Cannot Save Preferences"),
                              tr("Please check the file's write permission."),
                              QMessageBox::Ok);
    } else {
        close();
    }
}

void PreferencesDialog::choosePaperFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(
                this, tr("Choose Paper Folder"), QDir::homePath());

    if (!folderPath.isEmpty()) {
        paperFolderPath_->setText(folderPath);
    }
}

void PreferencesDialog::chooseDatabaseFile()
{
    QString filePath = QFileDialog::getOpenFileName(
                this, tr("Choose Database File"), QDir::homePath());

    if (!filePath.isEmpty()) {
        databaseFilePath_->setText(filePath);
    }
}

void PreferencesDialog::createPanels()
{
    paperFolderPath_ = new QLineEdit;
    databaseFilePath_ = new QLineEdit;

    QIcon folderOpenIcon(":/icons/folder_open.png");
    QPushButton* paperFolderPathButton = new QPushButton(folderOpenIcon, tr(""));
    QPushButton* databaseFilePathButton = new QPushButton(folderOpenIcon, tr(""));

    connect(paperFolderPathButton, &QPushButton::clicked,
            this, &PreferencesDialog::choosePaperFolder);
    connect(databaseFilePathButton, &QPushButton::clicked,
            this, &PreferencesDialog::chooseDatabaseFile);

    QPushButton* saveButton = new QPushButton(tr("Save"));
    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    connect(saveButton, &QPushButton::clicked, this, &PreferencesDialog::save);
    connect(cancelButton, &QPushButton::clicked, this, &PreferencesDialog::close);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Paper Folder")), 0, 0);
    layout->addWidget(paperFolderPath_, 0, 1, 1, 2);
    layout->addWidget(paperFolderPathButton, 0, 3);
    layout->addWidget(new QLabel(tr("Database File")), 1, 0);
    layout->addWidget(databaseFilePath_, 1, 1, 1, 2);
    layout->addWidget(databaseFilePathButton, 1, 3);
    layout->addWidget(saveButton, 2, 0, 1, 2);
    layout->addWidget(cancelButton, 2, 2, 1, 2);

    setLayout(layout);
}

void PreferencesDialog::loadPreferences()
{
    PreferencesManager& pm = PreferencesManager::instance();
    paperFolderPath_->setText(pm.getPaperFolderPath());
    databaseFilePath_->setText(pm.getDatabaseFilePath());
}
