#include "gamefalloutttw.h"

#include "falloutttwbsainvalidation.h"
#include "falloutttwdataarchives.h"
#include "falloutttwmoddatachecker.h"
#include "falloutttwmoddatacontent.h"
#include "falloutttwsavegame.h"
#include "falloutttwscriptextender.h"

#include "executableinfo.h"
#include "pluginsetting.h"
#include "versioninfo.h"
#include <gamebryogameplugins.h>
#include <gamebryolocalsavegames.h>
#include <gamebryosavegameinfo.h>
#include <gamebryounmanagedmods.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameFalloutTTW::GameFalloutTTW() {}

bool GameFalloutTTW::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  auto dataArchives = std::make_shared<FalloutTTWDataArchives>(this);
  registerFeature(std::make_shared<FalloutTTWScriptExtender>(this));
  registerFeature(dataArchives);
  registerFeature(
      std::make_shared<FalloutTTWBSAInvalidation>(dataArchives.get(), this));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<GamebryoLocalSavegames>(this, u"fallout.ini"_s));
  registerFeature(std::make_shared<FalloutTTWModDataChecker>(this));
  registerFeature(
      std::make_shared<FalloutTTWModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<GamebryoGamePlugins>(moInfo));
  registerFeature(std::make_shared<GamebryoUnmangedMods>(this));

  return true;
}

void GameFalloutTTW::setVariant(QString variant)
{
  m_GameVariant = variant;
}

void GameFalloutTTW::checkVariants()
{
  bool gog_dll_exists = QFileInfo::exists(m_GamePath % u"/Galaxy.dll"_s);
  bool epic_dll_exists = QFileInfo::exists(m_GamePath % u"/EOSSDK-Win32-Shipping.dll"_s);
  if (gog_dll_exists)
    setVariant(u"GOG"_s);
  else if (epic_dll_exists)
    setVariant(u"Epic Games"_s);
  else
    setVariant(u"Steam"_s);
}

QDir GameFalloutTTW::documentsDirectory() const
{
  return m_MyGamesPath;
}

QString GameFalloutTTW::identifyGamePath() const
{
  QString path = u"Software\\Bethesda Softworks\\FalloutNV"_s;
  auto result  = findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(),
                                L"Installed Path");
  // EPIC Game Store
  if (result.isEmpty()) {
    /**
     * Basegame: 5daeb974a22a435988892319b3a4f476
     * Dead Money: b290229eb58045cbab9501640f3278f3
     * Honest Hearts: 562d4a2c1b3147b089a7c453e3ddbcbe
     * Old World Blues: c8dae1ab0570475a8b38a9041e614840
     * Lonesome Road: 4fa3d8d9b2cb4714a19a38d1a598be8f
     * Gun Runners' Arsenal: 7dcfb9cd9d134728b2646466c34c7b3b
     * Courier's Stash: ee9a44b4530942499ef1c8c390731fce
     */
    result = parseEpicGamesLocation({u"5daeb974a22a435988892319b3a4f476"_s});
    if (QFileInfo(result).isDir()) {
      QDir startPath = QDir(result);
      auto subDirs   = startPath.entryList({u"Fallout New Vegas*"_s},
                                           QDir::Dirs | QDir::NoDotAndDotDot);
      if (!subDirs.isEmpty())
        result += '/' % subDirs.first();
    }
  }
  return result;
}

void GameFalloutTTW::setGamePath(const QString& path)
{
  m_GamePath = path;
  checkVariants();
  m_MyGamesPath = determineMyGamesPath(gameDirectoryName());
}

QDir GameFalloutTTW::savesDirectory() const
{
  return QDir(m_MyGamesPath % u"/Saves"_s);
}

QString GameFalloutTTW::myGamesPath() const
{
  return m_MyGamesPath;
}

bool GameFalloutTTW::isInstalled() const
{
  return !m_GamePath.isEmpty();
}

QString GameFalloutTTW::gameName() const
{
  return u"TTW"_s;
}

QString GameFalloutTTW::displayGameName() const
{
  return u"Tale of Two Wastelands"_s;
}

QString GameFalloutTTW::gameDirectoryName() const
{
  if (selectedVariant() == u"Epic Games"_s)
    return u"FalloutNV_Epic"_s;
  else
    return u"FalloutNV"_s;
}

void GameFalloutTTW::detectGame()
{
  m_GamePath = identifyGamePath();
  checkVariants();
  m_MyGamesPath = determineMyGamesPath(gameDirectoryName());
}

QList<ExecutableInfo> GameFalloutTTW::executables() const
{
  ExecutableInfo game(u"Tale of Two Wastelands"_s, findInGameFolder(binaryName()));
  ExecutableInfo launcher(u"Fallout Launcher"_s, findInGameFolder(getLauncherName()));
  QList<ExecutableInfo> extraExecutables =
      QList<ExecutableInfo>() << ExecutableInfo(u"GECK"_s, findInGameFolder(u"geck.exe"_s))
                              << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                                     .withArgument(u"--game=\"FalloutNV\""_s);
  if (selectedVariant() != u"Epic Games"_s) {
    extraExecutables.prepend(ExecutableInfo(
        u"NVSE"_s, findInGameFolder(m_Organizer->gameFeatures()
                                     ->gameFeature<MOBase::ScriptExtender>()
                                     ->loaderName())));
  } else {
    game.withArgument(u"-EpicPortal"_s);
    launcher.withArgument(u"-EpicPortal"_s);
  }
  QList<ExecutableInfo> executables = {game, launcher};
  executables += extraExecutables;
  return executables;
}

QList<ExecutableForcedLoadSetting> GameFalloutTTW::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameFalloutTTW::name() const
{
  return u"Fallout TTW Support Plugin"_s;
}

QString GameFalloutTTW::localizedName() const
{
  return tr("Fallout TTW Support Plugin");
}

QString GameFalloutTTW::author() const
{
  return u"SuperSandro2000 & MO2 Team"_s;
}

QString GameFalloutTTW::description() const
{
  return tr("Adds support for the game Fallout TTW");
}

MOBase::VersionInfo GameFalloutTTW::version() const
{
  return VersionInfo(1, 6, 0, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameFalloutTTW::settings() const
{
  return QList<PluginSetting>()
         << PluginSetting(u"enable_loot_sorting"_s,
                          tr("While not recommended by the TTW modding community, "
                             "enables LOOT sorting"),
                          false);
}

void GameFalloutTTW::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % '/' % gameDirectoryName(), path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() % u"/fallout.ini"_s)) {
      copyToProfile(gameDirectory().absolutePath(), path, u"fallout_default.ini"_s,
                    u"fallout.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"fallout.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"falloutprefs.ini"_s);
    copyToProfile(myGamesPath(), path, u"falloutcustom.ini"_s);
    copyToProfile(myGamesPath(), path, u"GECKCustom.ini"_s);
    copyToProfile(myGamesPath(), path, u"GECKPrefs.ini"_s);
  }
}

QString GameFalloutTTW::savegameExtension() const
{
  return u"fos"_s;
}

QString GameFalloutTTW::savegameSEExtension() const
{
  return u"nvse"_s;
}

std::shared_ptr<const GamebryoSaveGame>
GameFalloutTTW::makeSaveGame(QString filePath) const
{
  return std::make_shared<const FalloutTTWSaveGame>(filePath, this);
}

QString GameFalloutTTW::steamAPPId() const
{
  if (selectedVariant() == u"Steam"_s) {
    if (m_GamePath.endsWith(u"enplczru"_s)) {
      return u"22490"_s;
    } else {
      return u"22380"_s;
    }
  }
  return QString();
}

QStringList GameFalloutTTW::primaryPlugins() const
{
  return {u"falloutnv.esm"_s,     u"deadmoney.esm"_s,        u"honesthearts.esm"_s,
          u"oldworldblues.esm"_s, u"lonesomeroad.esm"_s,        u"gunrunnersarsenal.esm"_s,
          u"fallout3.esm"_s,      u"anchorage.esm"_s,           u"thepitt.esm"_s,
          u"brokensteel.esm"_s,   u"pointlookout.esm"_s,        u"zeta.esm"_s,
          u"caravanpack.esm"_s,   u"classicpack.esm"_s,         u"mercenarypack.esm"_s,
          u"tribalpack.esm"_s,    u"taleoftwowastelands.esm"_s, u"YUPTTW.esm"_s};
}

QStringList GameFalloutTTW::gameVariants() const
{
  return {u"Steam"_s, u"GOG"_s, u"Epic Games"_s};
}

QString GameFalloutTTW::binaryName() const
{
  return u"FalloutNV.exe"_s;
}

QString GameFalloutTTW::gameShortName() const
{
  return u"TTW"_s;
}

QStringList GameFalloutTTW::primarySources() const
{
  return {u"FalloutNV"_s};
}

QStringList GameFalloutTTW::validShortNames() const
{
  return {u"FalloutNV"_s, u"Fallout3"_s};
}

QString GameFalloutTTW::gameNexusName() const
{
  return u""_s;
}

QStringList GameFalloutTTW::iniFiles() const
{
  return {u"fallout.ini"_s, u"falloutprefs.ini"_s, u"falloutcustom.ini"_s, u"GECKCustom.ini"_s,
          u"GECKPrefs.ini"_s};
}

QStringList GameFalloutTTW::DLCPlugins() const
{
  return {};
}

MOBase::IPluginGame::SortMechanism GameFalloutTTW::sortMechanism() const
{
  if (m_Organizer->pluginSetting(name(), u"enable_loot_sorting"_s).toBool())
    return IPluginGame::SortMechanism::LOOT;
  return IPluginGame::SortMechanism::NONE;
}

QString GameFalloutTTW::lootGameName() const
{
  return u"FalloutNV"_s;
}

int GameFalloutTTW::nexusModOrganizerID() const
{
  return 0;
}

int GameFalloutTTW::nexusGameID() const
{
  return 0;
}

QDir GameFalloutTTW::gameDirectory() const
{
  return QDir(m_GamePath);
}

QString GameFalloutTTW::getLauncherName() const
{
  return u"FalloutNVLauncher.exe"_s;
}

MappingType GameFalloutTTW::mappings() const
{
  MappingType result;

  for (const QString& profileFile : {u"plugins.txt"_s, u"loadorder.txt"_s}) {
    result.push_back({m_Organizer->profilePath() % '/' % profileFile,
                      localAppFolder() % u"/FalloutNV/"_s % profileFile, false});
    if (selectedVariant() == u"Epic Games"_s) {
      result.push_back(
          {m_Organizer->profilePath() % '/' % profileFile,
           localAppFolder() % '/' % gameDirectoryName() % '/' % profileFile, false});
    }
  }
  return result;
}
