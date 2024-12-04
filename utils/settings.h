#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings
{

public:
    static Settings& instance();

public:
    ~Settings();

    Settings(Settings const&) = delete;
    Settings& operator= (Settings const&) = delete;

    void loadSettings();
    void saveSettings();

    int getGameVolume() const;
    void setGameVolume(int newGameVolume);

    int getMoveVolume() const;
    void setMoveVolume(int newMoveVolume);

    int getVisualType() const;
    void setVisualType(int newVisualType);

private:
    Settings();

private:
    static Settings *m_settingsInstance;
    std::string m_appDirPath;

    int gameVolume;
    int moveVolume;
    int visualType;
};

#endif // SETTINGS_H
