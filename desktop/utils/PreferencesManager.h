#ifndef PREFERENCESMANAGER_H
#define PREFERENCESMANAGER_H

#include <QJsonObject>

class PreferencesManager
{
public:
    static PreferencesManager& instance();

    bool save();

    QString getPaperFolderPath() const;
    void setPaperFolderPath(const QString& paperFolderPath);

    QString getDatabaseFilePath() const;
    void setDatabaseFilePath(const QString& databaseFilePath);

private:
    QJsonObject preferences_;

private:
    PreferencesManager();
    PreferencesManager(const PreferencesManager&);
    const PreferencesManager& operator = (const PreferencesManager&);
    ~PreferencesManager();

    QJsonObject getDefaultPreferences() const;
};

#endif // PREFERENCESMANAGER_H
