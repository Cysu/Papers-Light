#ifndef PREFERENCEMANAGER_H
#define PREFERENCEMANAGER_H

class PreferenceManager
{
public:
    static PreferenceManager& instance();

private:
    PreferenceManager();
    PreferenceManager(const PreferenceManager&);
    const PreferenceManager& operator = (const PreferenceManager&);
    ~PreferenceManager();
};

#endif // PREFERENCEMANAGER_H
