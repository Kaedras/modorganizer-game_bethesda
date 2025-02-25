#include "gameskyrim.h"

#include "skyrimbsainvalidation.h"
#include "skyrimdataarchives.h"
#include "skyrimgameplugins.h"
#include "skyrimmoddatachecker.h"
#include "skyrimmoddatacontent.h"
#include "skyrimsavegame.h"
#include "skyrimscriptextender.h"

#include "executableinfo.h"
#include "pluginsetting.h"

#include <gamebryogameplugins.h>
#include <gamebryolocalsavegames.h>
#include <gamebryosavegameinfo.h>
#include <gamebryounmanagedmods.h>

#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QVersionNumber>
#include <QtDebug>

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameSkyrim::GameSkyrim() {}

bool GameSkyrim::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  auto dataArchives = std::make_shared<SkyrimDataArchives>(this);
  registerFeature(std::make_shared<SkyrimScriptExtender>(this));
  registerFeature(dataArchives);
  registerFeature(std::make_shared<SkyrimBSAInvalidation>(dataArchives.get(), this));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<GamebryoLocalSavegames>(this, u"Skyrim.ini"_s));
  registerFeature(std::make_shared<SkyrimModDataChecker>(this));
  registerFeature(std::make_shared<SkyrimModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<SkyrimGamePlugins>(moInfo));
  registerFeature(std::make_shared<GamebryoUnmangedMods>(this));

  return true;
}

QString GameSkyrim::gameName() const
{
  return u"Skyrim"_s;
}

QList<ExecutableInfo> GameSkyrim::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"SKSE"_s,
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
         << ExecutableInfo(u"SBW"_s, findInGameFolder(u"SBW.exe"_s))
         << ExecutableInfo(u"Skyrim"_s, findInGameFolder(binaryName()))
         << ExecutableInfo(u"Skyrim Launcher"_s, findInGameFolder(getLauncherName()))
         << ExecutableInfo(u"BOSS"_s, findInGameFolder(u"BOSS/BOSS.exe"_s))
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Skyrim\""_s)
         << ExecutableInfo(u"Creation Kit"_s, findInGameFolder(u"CreationKit.exe"_s))
                .withSteamAppId(u"202480"_s);
}

QList<ExecutableForcedLoadSetting> GameSkyrim::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameSkyrim::name() const
{
  return u"Skyrim Support Plugin"_s;
}

QString GameSkyrim::localizedName() const
{
  return tr("Skyrim Support Plugin");
}

QString GameSkyrim::author() const
{
  return u"Tannin & MO2 Team"_s;
}

QString GameSkyrim::description() const
{
  return tr("Adds support for the game Skyrim");
}

MOBase::VersionInfo GameSkyrim::version() const
{
  return VersionInfo(1, 6, 1, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameSkyrim::settings() const
{
  QList<PluginSetting> results;
  results.push_back(
      PluginSetting(u"sse_downloads"_s, u"allow Skyrim SE downloads"_s, QVariant(false)));
  return results;
}

void GameSkyrim::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % u"/Skyrim"_s, path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() % u"/Skyrim.ini"_s)) {
      copyToProfile(gameDirectory().absolutePath(), path, u"Skyrim_default.ini"_s,
                    u"Skyrim.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"Skyrim.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"SkyrimPrefs.ini"_s);
  }
}

QString GameSkyrim::savegameExtension() const
{
  return u"ess"_s;
}

QString GameSkyrim::savegameSEExtension() const
{
  return u"skse"_s;
}

std::shared_ptr<const GamebryoSaveGame> GameSkyrim::makeSaveGame(QString filepath) const
{
  return std::make_shared<const SkyrimSaveGame>(filepath, this);
}

QString GameSkyrim::steamAPPId() const
{
  return u"72850"_s;
}

QStringList GameSkyrim::primaryPlugins() const
{
  return {u"Skyrim.esm"_s, u"Update.esm"_s};
}

QString GameSkyrim::binaryName() const
{
  return u"TESV.exe"_s;
}

QString GameSkyrim::gameShortName() const
{
  return u"Skyrim"_s;
}

QString GameSkyrim::gameNexusName() const
{
  return u"skyrim"_s;
}

QStringList GameSkyrim::validShortNames() const
{
  QStringList results;
  if (m_Organizer->pluginSetting(name(), u"sse_downloads"_s).toBool()) {
    results.push_back(u"SkyrimSE"_s);
  }
  return results;
}

QStringList GameSkyrim::iniFiles() const
{
  return {u"Skyrim.ini"_s, u"SkyrimPrefs.ini"_s};
}

QStringList GameSkyrim::DLCPlugins() const
{
  return {u"Dawnguard.esm"_s,
          u"Dragonborn.esm"_s,
          u"HearthFires.esm"_s,
          u"HighResTexturePack01.esp"_s,
          u"HighResTexturePack02.esp"_s,
          u"HighResTexturePack03.esp"_s};
}

IPluginGame::LoadOrderMechanism GameSkyrim::loadOrderMechanism() const
{
  try {
    QString versionString = getFileVersion(gameDirectory().absoluteFilePath(binaryName()));
    QVersionNumber version = QVersionNumber::fromString(versionString);

    static const auto v1_5 = QVersionNumber(1,5);

    // version >= 1.5.x || version >= x.4.26
    if (version >= v1_5 || (version.minorVersion() >= 4 && version.microVersion() >= 26)) {
      return LoadOrderMechanism::PluginsTxt;
    }
  } catch (const std::exception& e) {
    qCritical() << "TESV.exe is invalid: " << e.what();
  }
  return LoadOrderMechanism::FileTime;
}

int GameSkyrim::nexusModOrganizerID() const
{
  return 0;
}

int GameSkyrim::nexusGameID() const
{
  return 110;
}
