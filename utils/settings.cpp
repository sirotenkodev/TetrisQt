#include <QApplication>
#include <QSettings>

#include "settings.h"

///////////////////////////////////////////////////////////////////////////////
Settings *Settings::m_settingsInstance = nullptr;
static const char *SETTINGS_FILENAME = "tetris.ini";

//-----------------------------------------------------------------------------
Settings::Settings()
    : m_appDirPath(qApp->applicationDirPath().toStdString())
{
    loadSettings();
}

//-----------------------------------------------------------------------------
int Settings::getVisualType() const
{
    return visualType;
}

//-----------------------------------------------------------------------------
void Settings::setVisualType(int newVisualType)
{
    visualType = newVisualType;
}

//-----------------------------------------------------------------------------
int Settings::getMoveVolume() const
{
    return moveVolume;
}

//-----------------------------------------------------------------------------
void Settings::setMoveVolume(int newMoveVolume)
{
    moveVolume = newMoveVolume;
}

//-----------------------------------------------------------------------------
int Settings::getGameVolume() const
{
    return gameVolume;
}

//-----------------------------------------------------------------------------
void Settings::setGameVolume(int newGameVolume)
{
    gameVolume = newGameVolume;
}

//-----------------------------------------------------------------------------
Settings &Settings::instance()
{
    static Settings s_settings;
    return s_settings;
}

//-----------------------------------------------------------------------------
Settings::~Settings()
{
    saveSettings();
}

//-----------------------------------------------------------------------------
void Settings::loadSettings()
{
    auto settingsPath = m_appDirPath + "/" + SETTINGS_FILENAME;
    QSettings settings(settingsPath.c_str(), QSettings::IniFormat);
    settings.beginGroup("/Settings");
    if(!settings.contains("/gameVolume")) {
        gameVolume = 50;
    } else {
        gameVolume = settings.value("/gameVolume").toInt();
    }

    if(!settings.contains("/moveVolume")) {
        moveVolume = 50;
    } else {
        moveVolume = settings.value("/moveVolume").toInt();
    }

    if(!settings.contains("/visualType")) {
        visualType = 0;
    } else {
        visualType = settings.value("/visualType").toInt();
    }
    settings.endGroup();
}

//-----------------------------------------------------------------------------
void Settings::saveSettings()
{
    auto settingsPath = m_appDirPath + "/" + SETTINGS_FILENAME;
    QSettings settings(settingsPath.c_str(), QSettings::IniFormat);

    settings.beginGroup("/Settings");
    settings.setValue("gameVolume", gameVolume);
    settings.setValue("moveVolume", moveVolume);
    settings.setValue("visualType", visualType);
    settings.endGroup();
}
