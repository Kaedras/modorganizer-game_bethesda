#include "gameenderal.h"

#include "enderalbsainvalidation.h"
#include "enderaldataarchives.h"
#include "enderalgameplugins.h"
#include "enderallocalsavegames.h"
#include "enderalmoddatachecker.h"
#include "enderalmoddatacontent.h"
#include "enderalsavegame.h"
#include "enderalscriptextender.h"

#include "executableinfo.h"
#include "pluginsetting.h"
#include "steamutility.h"
#include "utility.h"

#include <gamebryogameplugins.h>
#include <gamebryosavegameinfo.h>
#include <gamebryounmanagedmods.h>

#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>

#include <QIcon>
#include <QtDebug>

#ifdef __unix__
#include "linux/windowsTypes.h"
#else
#include <Windows.h>
#include <winver.h>
#endif

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameEnderal::GameEnderal() {}

bool GameEnderal::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  auto dataArchives = std::make_shared<EnderalDataArchives>(this);
  registerFeature(std::make_shared<EnderalScriptExtender>(this));
  registerFeature(dataArchives);
  registerFeature(std::make_shared<EnderalBSAInvalidation>(dataArchives.get(), this));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<EnderalLocalSavegames>(this, u"Enderal.ini"_s));
  registerFeature(std::make_shared<EnderalModDataChecker>(this));
  registerFeature(std::make_shared<EnderalModDataContent>(moInfo->gameFeatures()));
  registerFeature(std::make_shared<EnderalGamePlugins>(moInfo));
  registerFeature(std::make_shared<GamebryoUnmangedMods>(this));

  return true;
}

QString GameEnderal::gameName() const
{
  return "Enderal";
}

QList<ExecutableInfo> GameEnderal::executables() const
{
  return QList<ExecutableInfo>()
         //<< ExecutableInfo("SKSE",
         // findInGameFolder(feature<ScriptExtender>()->loaderName()))
         //<< ExecutableInfo("SBW", findInGameFolder("SBW.exe"))
         << ExecutableInfo(u"Enderal (SKSE)"_s, findInGameFolder(binaryName()))
         << ExecutableInfo(u"Enderal Launcher"_s, findInGameFolder(getLauncherName()))
         << ExecutableInfo(u"BOSS"_s, findInGameFolder(u"BOSS/BOSS.exe"_s))
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Enderal\""_s)
         << ExecutableInfo(u"Creation Kit"_s, findInGameFolder(u"CreationKit.exe"_s))
                .withSteamAppId(u"202480"_s);
}

QList<ExecutableForcedLoadSetting> GameEnderal::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameEnderal::name() const
{
  return u"Enderal Support Plugin"_s;
}

QString GameEnderal::localizedName() const
{
  return tr("Enderal Support Plugin");
}

QString GameEnderal::author() const
{
  return u"AL12 & MO2 Team"_s;
}

QString GameEnderal::description() const
{
  return tr("Adds support for the game Enderal");
}

MOBase::VersionInfo GameEnderal::version() const
{
  return VersionInfo(1, 3, 1, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameEnderal::settings() const
{
  QList<PluginSetting> results;
  results.push_back(
      PluginSetting(u"sse_downloads"_s, u"allow Skyrim SE downloads"_s, QVariant(false)));
  return results;
}

void GameEnderal::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % u"/Enderal"_s, path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() % u"/Enderal.ini"_s)) {

      // there is no default ini, actually they are going to put them in for us!
      copyToProfile(gameDirectory().absolutePath(), path, u"enderal_default.ini"_s,
                    u"Enderal.ini"_s);
      copyToProfile(gameDirectory().absolutePath(), path, u"EnderalPrefs_default.ini"_s,
                    u"EnderalPrefs.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"Enderal.ini"_s);
      copyToProfile(myGamesPath(), path, u"EnderalPrefs.ini"_s);
    }
  }
}

bool GameEnderal::looksValid(QDir const& path) const
{
  // we need to check both launcher and binary because the binary also exists for
  // Skyrim and the launcher for Enderal SE
  return path.exists(getLauncherName()) && path.exists(binaryName());
}

QIcon GameEnderal::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath(getLauncherName()));
}

QString GameEnderal::savegameExtension() const
{
  return u"ess"_s;
}

QString GameEnderal::savegameSEExtension() const
{
  return u"skse"_s;
}

std::shared_ptr<const GamebryoSaveGame>
GameEnderal::makeSaveGame(QString filepath) const
{
  return std::make_shared<const EnderalSaveGame>(filepath, this);
}

QString GameEnderal::steamAPPId() const
{
  return u"933480"_s;
}

QStringList GameEnderal::primaryPlugins() const
{
  return {u"Skyrim.esm"_s, u"Enderal - Forgotten Stories.esm"_s, u"Update.esm"_s};
}

QString GameEnderal::binaryName() const
{
  return u"skse_loader.exe"_s;
}

QString GameEnderal::getLauncherName() const
{
  return u"Enderal Launcher.exe"_s;
}

QString GameEnderal::gameShortName() const
{
  return u"Enderal"_s;
}

QString GameEnderal::gameNexusName() const
{
  return u"enderal"_s;
}

QStringList GameEnderal::primarySources() const
{
  return {u"Skyrim"_s};
}

QStringList GameEnderal::validShortNames() const
{
  QStringList results;
  results.push_back(u"Skyrim"_s);
  if (m_Organizer->pluginSetting(name(), u"sse_downloads"_s).toBool()) {
    results.push_back(u"SkyrimSE"_s);
  }
  return results;
}

QStringList GameEnderal::iniFiles() const
{
  return {u"Enderal.ini"_s, u"EnderalPrefs.ini"_s};
}

QStringList GameEnderal::DLCPlugins() const
{
  return {};
}

IPluginGame::LoadOrderMechanism GameEnderal::loadOrderMechanism() const
{
  return LoadOrderMechanism::PluginsTxt;
}

int GameEnderal::nexusModOrganizerID() const
{
  return 0;
}

int GameEnderal::nexusGameID() const
{
  return 2736;
}

QString GameEnderal::identifyGamePath() const
{
  QString path = u"Software\\SureAI\\Enderal"_s;
  QString result;
  try {
    result =
        findInRegistry(HKEY_CURRENT_USER, path.toStdWString().c_str(), L"Install_Path");
  } catch (MOBase::MyException) {
    result = MOBase::findSteamGame(u"Enderal"_s, u"Data/Enderal - Forgotten Stories.esm"_s);
  }
  return result;
}
