#include "gamefallout4.h"

#include "fallout4bsainvalidation.h"
#include "fallout4dataarchives.h"
#include "fallout4moddatachecker.h"
#include "fallout4moddatacontent.h"
#include "fallout4savegame.h"
#include "fallout4scriptextender.h"
#include "fallout4unmanagedmods.h"

#include "versioninfo.h"
#include <creationgameplugins.h>
#include <executableinfo.h>
#include <gamebryolocalsavegames.h>
#include <gamebryosavegameinfo.h>
#include <pluginsetting.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

#include "scopeguard.h"

#include <QSettings>

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameFallout4::GameFallout4() {}

bool GameFallout4::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  auto dataArchives = std::make_shared<Fallout4DataArchives>(this);

  registerFeature(std::make_shared<Fallout4ScriptExtender>(this));
  registerFeature(dataArchives);
  registerFeature(std::make_shared<GamebryoLocalSavegames>(this, u"fallout4custom.ini"_s));
  registerFeature(std::make_shared<Fallout4ModDataChecker>(this));
  registerFeature(
      std::make_shared<Fallout4ModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<CreationGamePlugins>(moInfo));
  registerFeature(std::make_shared<Fallout4UnmangedMods>(this));
  registerFeature(std::make_shared<Fallout4BSAInvalidation>(dataArchives.get(), this));

  return true;
}

QString GameFallout4::gameName() const
{
  return u"Fallout 4"_s;
}

void GameFallout4::detectGame()
{
  m_GamePath    = identifyGamePath();
  m_MyGamesPath = determineMyGamesPath(u"Fallout4"_s);
}

QList<ExecutableInfo> GameFallout4::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"F4SE"_s,
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
         << ExecutableInfo(u"Fallout 4"_s, findInGameFolder(binaryName()))
         << ExecutableInfo(u"Fallout Launcher"_s, findInGameFolder(getLauncherName()))
         << ExecutableInfo(u"Creation Kit"_s, findInGameFolder(u"CreationKit.exe"_s))
                .withSteamAppId(u"1946160"_s)
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Fallout4\""_s);
}

QList<ExecutableForcedLoadSetting> GameFallout4::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameFallout4::name() const
{
  return u"Fallout 4 Support Plugin"_s;
}

QString GameFallout4::localizedName() const
{
  return tr("Fallout 4 Support Plugin");
}

QString GameFallout4::author() const
{
  return u"Tannin & MO2 Team"_s;
}

QString GameFallout4::description() const
{
  return tr("Adds support for the game Fallout 4.\n"
            "Splash by %1")
      .arg("nekoyoubi");
}

MOBase::VersionInfo GameFallout4::version() const
{
  return VersionInfo(1, 8, 0, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameFallout4::settings() const
{
  return QList<PluginSetting>();
}

MappingType GameFallout4::mappings() const
{
  MappingType result;
  if (testFilePlugins().isEmpty()) {
    for (const auto& profileFile : {u"plugins.txt"_s, u"loadorder.txt"_s}) {
      result.push_back({m_Organizer->profilePath() % '/' % profileFile,
                        localAppFolder() % '/' % gameShortName() % '/' % profileFile,
                        false});
    }
  }
  return result;
}

void GameFallout4::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + u"/Fallout4"_s, path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() + u"/fallout4.ini"_s)) {
      copyToProfile(gameDirectory().absolutePath(), path, u"fallout4_default.ini"_s,
                    u"fallout4.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"fallout4.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"fallout4prefs.ini"_s);
    copyToProfile(myGamesPath(), path, u"fallout4custom.ini"_s);
  }
}

QString GameFallout4::savegameExtension() const
{
  return u"fos"_s;
}

QString GameFallout4::savegameSEExtension() const
{
  return u"f4se"_s;
}

std::shared_ptr<const GamebryoSaveGame>
GameFallout4::makeSaveGame(QString filePath) const
{
  return std::make_shared<const Fallout4SaveGame>(filePath, this);
}

QString GameFallout4::steamAPPId() const
{
  return u"377160"_s;
}

QStringList GameFallout4::testFilePlugins() const
{
  QStringList plugins;
  if (m_Organizer != nullptr && m_Organizer->profile() != nullptr) {
    QString customIni(
        m_Organizer->profile()->absoluteIniFilePath(u"Fallout4Custom.ini"_s));
    if (QFile(customIni).exists()) {
      for (int i = 1; i <= 10; ++i) {
        QString setting(u"sTestFile"_s);
        setting += std::to_string(i);

        QSettings ini(customIni, QSettings::IniFormat);
        QString value = ini.value("General/" % setting, u""_s).toString();

        if (value != u""_s) {
          if (!value.isEmpty() && !plugins.contains(value))
            plugins.append(value);
        }
      }
    }
  }
  return plugins;
}

QStringList GameFallout4::primaryPlugins() const
{
  QStringList plugins = {u"fallout4.esm"_s,      u"dlcrobot.esm"_s,
                         u"dlcworkshop01.esm"_s, u"dlccoast.esm"_s,
                         u"dlcworkshop02.esm"_s, u"dlcworkshop03.esm"_s,
                         u"dlcnukaworld.esm"_s,  u"dlcultrahighresolution.esm"_s};

  auto testPlugins = testFilePlugins();
  if (loadOrderMechanism() == LoadOrderMechanism::None) {
    plugins << testPlugins;
  } else {
    plugins << CCPlugins();
  }

  return plugins;
}

QStringList GameFallout4::gameVariants() const
{
  return {u"Regular"_s};
}

QString GameFallout4::gameShortName() const
{
  return u"Fallout4"_s;
}

QString GameFallout4::gameNexusName() const
{
  return u"fallout4"_s;
}

QStringList GameFallout4::iniFiles() const
{
  return {u"fallout4.ini"_s, u"fallout4prefs.ini"_s, u"fallout4custom.ini"_s};
}

QStringList GameFallout4::DLCPlugins() const
{
  return {u"dlcrobot.esm"_s,
          u"dlcworkshop01.esm"_s,
          u"dlccoast.esm"_s,
          u"dlcworkshop02.esm"_s,
          u"dlcworkshop03.esm"_s,
          u"dlcnukaworld.esm"_s,
          u"dlcultrahighresolution.esm"_s};
}

QStringList GameFallout4::CCPlugins() const
{
  QStringList plugins = {};
  QFile file(gameDirectory().absoluteFilePath(u"Fallout4.ccc"_s));
  if (file.open(QIODevice::ReadOnly)) {
    ON_BLOCK_EXIT([&file]() {
      file.close();
    });

    if (file.size() == 0) {
      return plugins;
    }
    while (!file.atEnd()) {
      QByteArray line = file.readLine().trimmed();
      QString modName;
      if ((line.size() > 0) && (line.at(0) != '#')) {
        modName = QString::fromUtf8(line.constData()).toLower();
      }

      if (modName.size() > 0) {
        if (!plugins.contains(modName, Qt::CaseInsensitive)) {
          plugins.append(modName);
        }
      }
    }
  }
  return plugins;
}

IPluginGame::SortMechanism GameFallout4::sortMechanism() const
{
  if (!testFilePresent())
    return IPluginGame::SortMechanism::LOOT;
  return IPluginGame::SortMechanism::NONE;
}

IPluginGame::LoadOrderMechanism GameFallout4::loadOrderMechanism() const
{
  if (!testFilePresent())
    return IPluginGame::LoadOrderMechanism::PluginsTxt;
  return IPluginGame::LoadOrderMechanism::None;
}

int GameFallout4::nexusModOrganizerID() const
{
  return 28715;
}

int GameFallout4::nexusGameID() const
{
  return 1151;
}

// Start Diagnose
std::vector<unsigned int> GameFallout4::activeProblems() const
{
  std::vector<unsigned int> result;
  if (m_Organizer->managedGame() == this) {
    if (testFilePresent())
      result.push_back(PROBLEM_TEST_FILE);
  }
  return result;
}

bool GameFallout4::testFilePresent() const
{
  if (!testFilePlugins().isEmpty())
    return true;
  return false;
}

QString GameFallout4::shortDescription(unsigned int key) const
{
  switch (key) {
  case PROBLEM_TEST_FILE:
    return tr("sTestFile entries are present");
  }
}

QString GameFallout4::fullDescription(unsigned int key) const
{
  switch (key) {
  case PROBLEM_TEST_FILE: {
    return tr("<p>You have sTestFile settings in your "
              "Fallout4Custom.ini. These must be removed or "
              "the game will not read the plugins.txt file. "
              "Management is disabled.</p>");
  }
  }
}
