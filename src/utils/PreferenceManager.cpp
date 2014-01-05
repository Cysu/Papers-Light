#include "utils/PreferenceManager.h"
#include "common/const.h"

PreferenceManager& PreferenceManager::instance()
{
    static PreferenceManager instance;
    return instance;
}

PreferenceManager::PreferenceManager()
{

}

PreferenceManager::~PreferenceManager()
{

}
