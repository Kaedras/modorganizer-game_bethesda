#include "gamefallout4vr.h"

#include "fallout4vrdataarchives.h"
#include "fallout4vrgameplugins.h"
#include "fallout4vrmoddatachecker.h"
#include "fallout4vrmoddatacontent.h"
#include "fallout4vrsavegame.h"
#include "fallout4vrunmanagedmods.h"

#include "versioninfo.h"
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

using namespace MOBase;

GameFallout4VR::GameFallout4VR() {}

bool GameFallout4VR::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  registerFeature(std::make_shared<Fallout4VRDataArchives>(this));
  registerFeature(std::make_shared<GamebryoLocalSavegames>(this, u"fallout4custom.ini"_s));
  registerFeature(std::make_shared<Fallout4VRModDataChecker>(this));
  registerFeature(
      std::make_shared<Fallout4VRModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<Fallout4VRGamePlugins>(moInfo));
  registerFeature(std::make_shared<Fallout4VRUnmangedMods>(this));

  return true;
}

QString GameFallout4VR::gameName() const
{
  return "Fallout 4 VR";
}

void GameFallout4VR::detectGame()
{
  m_GamePath    = identifyGamePath();
  m_MyGamesPath = determineMyGamesPath(u"Fallout4VR"_s);
}

QList<ExecutableInfo> GameFallout4VR::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"Fallout 4 VR"_s, findInGameFolder(binaryName()))
         << ExecutableInfo(u"Creation Kit"_s, findInGameFolder(u"CreationKit.exe"_s))
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Fallout4VR\""_s);
}

QList<ExecutableForcedLoadSetting> GameFallout4VR::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameFallout4VR::name() const
{
  return u"Fallout 4 VR Support Plugin"_s;
}

QString GameFallout4VR::localizedName() const
{
  return tr("Fallout 4 VR Support Plugin");
}

QString GameFallout4VR::author() const
{
  return u"MO2 Contibutors"_s;
}

QString GameFallout4VR::description() const
{
  return tr("Adds support for the game Fallout 4 VR.\n"
            "Splash by %1")
      .arg(u"nekoyoubi"_s);
}

MOBase::VersionInfo GameFallout4VR::version() const
{
  return VersionInfo(1, 6, 1, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameFallout4VR::settings() const
{
  return QList<PluginSetting>();
}

void GameFallout4VR::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % u"/Fallout4VR"_s, path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() % u"/fallout4.ini"_s)) {
      copyToProfile(gameDirectory().absolutePath(), path, u"fallout4.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"fallout4.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"fallout4prefs.ini"_s);
    copyToProfile(myGamesPath(), path, u"fallout4custom.ini"_s);
  }
}

QString GameFallout4VR::savegameExtension() const
{
  return u"fos"_s;
}

QString GameFallout4VR::savegameSEExtension() const
{
  return u"f4se"_s;
}

std::shared_ptr<const GamebryoSaveGame>
GameFallout4VR::makeSaveGame(QString filePath) const
{
  return std::make_shared<const Fallout4VRSaveGame>(filePath, this);
}

QString GameFallout4VR::steamAPPId() const
{
  return u"611660"_s;
}

QStringList GameFallout4VR::primaryPlugins() const
{
  QStringList plugins = {u"fallout4.esm_s"_s, u"fallout4_vr.esm"_s};

  plugins.append(CCPlugins());

  return plugins;
}

QStringList GameFallout4VR::gameVariants() const
{
  return {u"Regular"_s};
}

QString GameFallout4VR::gameShortName() const
{
  return u"Fallout4VR"_s;
}

QStringList GameFallout4VR::validShortNames() const
{
  return {u"Fallout4"_s};
}

QString GameFallout4VR::gameNexusName() const
{
  return QString();
}

QStringList GameFallout4VR::iniFiles() const
{
  return {u"fallout4.ini"_s, u"fallout4custom.ini"_s, u"fallout4prefs.ini"_s};
}

QStringList GameFallout4VR::DLCPlugins() const
{
  return {u"dlcrobot.esm"_s,
          u"dlcworkshop01.esm"_s,
          u"dlccoast.esm"_s,
          u"dlcworkshop02.esm"_s,
          u"dlcworkshop03.esm"_s,
          u"dlcnukaworld.esm"_s,
          u"dlcultrahighresolution.esm"_s};
}

QStringList GameFallout4VR::CCPlugins() const
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

IPluginGame::LoadOrderMechanism GameFallout4VR::loadOrderMechanism() const
{
  return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

int GameFallout4VR::nexusModOrganizerID() const
{
  return 0;  //...
}

int GameFallout4VR::nexusGameID() const
{
  return 0;
}

QString GameFallout4VR::getLauncherName() const
{
  return binaryName();  // Fallout 4 VR has no Launcher, so we just return the name of
                        // the game binary
}

QString GameFallout4VR::identifyGamePath() const
{
  QString path = u"Software\\Bethesda Softworks\\"_s % gameName();
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(),
                        L"Installed Path");
}
