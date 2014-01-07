#include "utils/PreferencesManager.h"
#include "common/const.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>

PreferencesManager& PreferencesManager::instance()
{
    static PreferencesManager instance;
    return instance;
}

PreferencesManager::PreferencesManager()
{
    QString preferenceFilePath = QDir(QDir::homePath()).filePath(PREFERENCE_FILENAME);

    QFile preferenceFile(preferenceFilePath);

    if (preferenceFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc(QJsonDocument::fromJson(preferenceFile.readAll()));
        preferences_ = doc.object();
    } else {
        preferences_ = getDefaultPreferences();
    }
}

PreferencesManager::~PreferencesManager()
{

}

bool PreferencesManager::save()
{
    QString preferenceFilePath = QDir::homePath() + "/" + PREFERENCE_FILENAME;

    QFile preferenceFile(preferenceFilePath);

    if (preferenceFile.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(preferences_);
        preferenceFile.write(doc.toJson());
        return true;
    }

    return false;
}

QString PreferencesManager::getPaperFolderPath() const
{
    return preferences_["paperFolderPath"].toString();
}

void PreferencesManager::setPaperFolderPath(const QString& paperFolderPath)
{
    preferences_["paperFolderPath"] = paperFolderPath;
}

QString PreferencesManager::getDatabaseFilePath() const
{
    return preferences_["databaseFilePath"].toString();
}

void PreferencesManager::setDatabaseFilePath(const QString& databaseFilePath)
{
    preferences_["databaseFilePath"] = databaseFilePath;
}

QJsonObject PreferencesManager::getDefaultPreferences() const
{
    QJsonObject preference;
    preference["paperFolderPath"] = QDir::homePath();
    preference["databaseFilePath"] = QString();
    return preference;
}
