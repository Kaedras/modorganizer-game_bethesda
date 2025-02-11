#include "gameoblivion.h"

#include "oblivionbsainvalidation.h"
#include "obliviondataarchives.h"
#include "oblivionmoddatachecker.h"
#include "oblivionmoddatacontent.h"
#include "oblivionsavegame.h"
#include "oblivionscriptextender.h"

#include "executableinfo.h"
#include "pluginsetting.h"
#include <gamebryogameplugins.h>
#include <gamebryolocalsavegames.h>
#include <gamebryosavegameinfo.h>
#include <gamebryounmanagedmods.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include <memory>

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameOblivion::GameOblivion() {}

bool GameOblivion::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  auto dataArchives = std::make_shared<OblivionDataArchives>(this);
  registerFeature(std::make_shared<OblivionScriptExtender>(this));
  registerFeature(dataArchives);
  registerFeature(std::make_shared<OblivionBSAInvalidation>(dataArchives.get(), this));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<GamebryoLocalSavegames>(this, u"oblivion.ini"_s));
  registerFeature(std::make_shared<OblivionModDataChecker>(this));
  registerFeature(
      std::make_shared<OblivionModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<GamebryoGamePlugins>(moInfo));
  registerFeature(std::make_shared<GamebryoUnmangedMods>(this));
  return true;
}

QString GameOblivion::gameName() const
{
  return u"Oblivion"_s;
}

QList<ExecutableInfo> GameOblivion::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"Oblivion"_s, findInGameFolder(binaryName()))
         << ExecutableInfo(u"Oblivion Launcher"_s, findInGameFolder(getLauncherName()))
         << ExecutableInfo(u"Oblivion Mod Manager"_s,
                           findInGameFolder(u"OblivionModManager.exe"_s))
         << ExecutableInfo(u"BOSS"_s, findInGameFolder(u"BOSS/BOSS.exe"_s))
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Oblivion\""_s)
         << ExecutableInfo(u"Construction Set"_s,
                           findInGameFolder(u"TESConstructionSet.exe"_s));
}

QList<ExecutableForcedLoadSetting> GameOblivion::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>()
         << ExecutableForcedLoadSetting(u"Oblivion.exe"_s, u"obse_1_2_416.dll"_s)
                .withForced()
                .withEnabled()
         << ExecutableForcedLoadSetting(u"TESConstructionSet.exe"_s, u"obse_editor_1_2.dll"_s)
                .withForced()
                .withEnabled();
}

QString GameOblivion::name() const
{
  return u"Oblivion Support Plugin"_s;
}

QString GameOblivion::localizedName() const
{
  return tr("Oblivion Support Plugin");
}

QString GameOblivion::author() const
{
  return u"Tannin & MO2 Team"_s;
}

QString GameOblivion::description() const
{
  return tr("Adds support for the game Oblivion");
}

MOBase::VersionInfo GameOblivion::version() const
{
  return VersionInfo(1, 6, 1, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameOblivion::settings() const
{
  return {PluginSetting(u"nehrim_downloads"_s, u"allow Nehrim downloads"_s, QVariant(false))};
}

void GameOblivion::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % u"/Oblivion"_s, path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() % u"/oblivion.ini"_s)) {
      copyToProfile(gameDirectory().absolutePath(), path, u"oblivion_default.ini"_s,
                    u"oblivion.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"oblivion.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"oblivionprefs.ini"_s);
  }
}

QString GameOblivion::savegameExtension() const
{
  return u"ess"_s;
}

QString GameOblivion::savegameSEExtension() const
{
  return u"obse"_s;
}

std::shared_ptr<const GamebryoSaveGame>
GameOblivion::makeSaveGame(QString filePath) const
{
  return std::make_shared<const OblivionSaveGame>(filePath, this);
}

QString GameOblivion::steamAPPId() const
{
  return u"22330"_s;
}

QStringList GameOblivion::primaryPlugins() const
{
  return {u"oblivion.esm"_s, u"update.esm"_s};
}

QString GameOblivion::gameShortName() const
{
  return u"Oblivion"_s;
}

QStringList GameOblivion::validShortNames() const
{
  QStringList shortNames;
  if (m_Organizer->pluginSetting(name(), u"nehrim_downloads"_s).toBool()) {
    shortNames.append(u"Nehrim"_s);
  }
  return shortNames;
}

QString GameOblivion::gameNexusName() const
{
  return u"Oblivion"_s;
}

QStringList GameOblivion::iniFiles() const
{
  return {u"oblivion.ini"_s, u"oblivionprefs.ini"_s};
}

QStringList GameOblivion::DLCPlugins() const
{
  return {u"DLCBattlehornCastle.esp"_s, u"DLCShiveringIsles.esp"_s, u"Knights.esp"_s,
          u"DLCFrostcrag.esp"_s,        u"DLCSpellTomes.esp"_s,     u"DLCMehrunesRazor.esp"_s,
          u"DLCOrrery.esp"_s,           u"DLCThievesDen.esp"_s,     u"DLCVileLair.esp"_s,
          u"DLCHorseArmor.esp"_s};
}

int GameOblivion::nexusModOrganizerID() const
{
  return 38277;
}

int GameOblivion::nexusGameID() const
{
  return 101;
}
