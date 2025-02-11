#include "gamenehrim.h"

#include "nehrimbsainvalidation.h"
#include "nehrimdataarchives.h"
#include "nehrimmoddatachecker.h"
#include "nehrimmoddatacontent.h"
#include "nehrimsavegame.h"
#include "nehrimscriptextender.h"

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

GameNehrim::GameNehrim() {}

bool GameNehrim::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  auto dataArchives = std::make_shared<NehrimDataArchives>(this);
  registerFeature(std::make_shared<NehrimScriptExtender>(this));
  registerFeature(dataArchives);
  registerFeature(std::make_shared<NehrimBSAInvalidation>(dataArchives.get(), this));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<GamebryoLocalSavegames>(this, u"oblivion.ini"_s));
  registerFeature(std::make_shared<NehrimModDataChecker>(this));
  registerFeature(std::make_shared<NehrimModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<GamebryoGamePlugins>(moInfo));
  registerFeature(std::make_shared<GamebryoUnmangedMods>(this));
  return true;
}

QString GameNehrim::gameName() const
{
  return u"Nehrim"_s;
}

QList<ExecutableInfo> GameNehrim::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"Nehrim"_s, findInGameFolder(u"Oblivion.exe"_s))
         << ExecutableInfo(u"Nehrim Launcher"_s, findInGameFolder(u"NehrimLauncher.exe"_s))
         << ExecutableInfo(u"Oblivion Mod Manager"_s,
                           findInGameFolder(u"OblivionModManager.exe"_s))
         << ExecutableInfo(u"BOSS"_s, findInGameFolder(u"BOSS/BOSS.exe"_s))
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Nehrim\""_s)
         << ExecutableInfo(u"Construction Set"_s,
                           findInGameFolder(u"TESConstructionSet.exe"_s));
}

QList<ExecutableForcedLoadSetting> GameNehrim::executableForcedLoads() const
{
  // TODO Search game directory for OBSE DLLs
  return QList<ExecutableForcedLoadSetting>()
         << ExecutableForcedLoadSetting(u"Oblvion.exe"_s, u"obse_1_2_416.dll"_s)
                .withForced()
                .withEnabled()
         << ExecutableForcedLoadSetting(u"TESConstructionSet.exe"_s, u"obse_editor_1_2.dll"_s)
                .withForced()
                .withEnabled();
}

QString GameNehrim::name() const
{
  return u"Nehrim Support Plugin"_s;
}

QString GameNehrim::localizedName() const
{
  return tr("Nehrim Support Plugin");
}

QString GameNehrim::author() const
{
  return u"Tannin & MO2 Team"_s;
}

QString GameNehrim::description() const
{
  return tr("Adds support for the game Nehrim");
}

MOBase::VersionInfo GameNehrim::version() const
{
  return VersionInfo(1, 1, 1, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameNehrim::settings() const
{
  return QList<PluginSetting>();
}

void GameNehrim::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % u"/Oblvion"_s, path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo(myGamesPath() % u"/oblivion.ini"_s).exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, u"oblivion_default.ini"_s,
                    u"oblivion.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"oblivion.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"oblivionprefs.ini"_s);
  }
}

QString GameNehrim::savegameExtension() const
{
  return u"ess"_s;
}

QString GameNehrim::savegameSEExtension() const
{
  return u"obse"_s;
}

std::shared_ptr<const GamebryoSaveGame> GameNehrim::makeSaveGame(QString filePath) const
{
  return std::make_shared<const NehrimSaveGame>(filePath, this);
}

QString GameNehrim::steamAPPId() const
{
  return u"22330"_s;
}

QStringList GameNehrim::primaryPlugins() const
{
  return {u"Nehrim.esm"_s, u"Translation.esp"_s};
}

QString GameNehrim::gameShortName() const
{
  return u"Nehrim"_s;
}

QString GameNehrim::gameNexusName() const
{
  return u"Nehrim"_s;
}

QStringList GameNehrim::iniFiles() const
{
  return {u"oblivion.ini"_s, u"oblivionprefs.ini"_s};
}

QStringList GameNehrim::DLCPlugins() const
{
  return {};
}

int GameNehrim::nexusModOrganizerID() const
{
  return -1;
}

int GameNehrim::nexusGameID() const
{
  return 3312;
}

QStringList GameNehrim::primarySources() const
{
  return {u"Oblivion"_s};
}

QStringList GameNehrim::validShortNames() const
{
  return {u"Oblivion"_s};
}

QString GameNehrim::identifyGamePath() const
{
  QString path = u"Software\\Bethesda Softworks\\Oblivion"_s;
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(),
                        L"Installed Path");
}

QString GameNehrim::binaryName() const
{
  return u"NehrimLauncher.exe"_s;
}
