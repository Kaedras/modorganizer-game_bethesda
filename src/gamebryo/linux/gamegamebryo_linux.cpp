#include "../gamegamebryo.h"
#include "util.h"
#include "vdf_parser.h"

#include <steamutility.h>

#include <QStandardPaths>
#include <QString>
#include <iostream>
#include <utility.h>

#define STUB() std::cout << __FUNCTION__ << ": STUB\n"

using namespace Qt::StringLiterals;

// set the default variant to steam to enable getting the appid earlier
GameGamebryo::GameGamebryo() : m_GameVariant(u"Steam"_s) {}

QString GameGamebryo::identifyGamePath() const
{
  return parseSteamLocation(steamAPPId(), gameName());
}

QString GameGamebryo::getLootPath()
{
  STUB();
  return {};
}

void GameGamebryo::copyToProfile(QString const& sourcePath,
                                 QDir const& destinationDirectory,
                                 QString const& sourceFileName)
{
  QString src = findFileNameCaseInsensitive(sourcePath, sourceFileName);
  copyToProfile(sourcePath, destinationDirectory, src, src);
}

void GameGamebryo::copyToProfile(QString const& sourcePath,
                                 QDir const& destinationDirectory,
                                 QString const& sourceFileName,
                                 QString const& destinationFileName)
{
  QString srcName  = findFileNameCaseInsensitive(sourcePath, sourceFileName);
  QString filePath = destinationDirectory.absoluteFilePath(destinationFileName);
  if (!QFileInfo::exists(filePath)) {
    if (!MOBase::shellCopy(sourcePath % "/"_L1 % srcName, filePath)) {
      // if copy file fails, create the file empty
      QFile(filePath).open(QIODevice::WriteOnly);
    }
  }
}

QString GameGamebryo::localAppFolder(const QString& appID)
{
  QString compatData = MOBase::findCompatDataByAppID(appID);

  QString path = compatData % "/pfx/drive_c/users/steamuser/AppData/Local/"_L1;
  if (QFile::exists(path)) {
    return path;
  }

  return {};
}

std::unique_ptr<BYTE[]> GameGamebryo::getRegValue(HKEY, LPCWSTR, LPCWSTR, DWORD,
                                                  LPDWORD)
{
  // this function is probably not needed
  return {};
}

QString GameGamebryo::findInRegistry(HKEY, LPCWSTR, LPCWSTR)
{
  // this function is probably not needed
  return {};
}

QString GameGamebryo::getKnownFolderPath(REFKNOWNFOLDERID folderId, bool)
{
  // use folderId as QStandardPaths::StandardLocation
  return QStandardPaths::standardLocations(
             static_cast<QStandardPaths::StandardLocation>(folderId))
      .first();
}

QString GameGamebryo::getSpecialPath(const QString&)
{
  // this function is probably not needed
  return {};
}

QString GameGamebryo::determineMyGamesPath(const QString& gameName)
{
  const QString pattern = "%1/My Games/"_L1 % gameName;

  auto tryDir = [&](const QString& dir) -> std::optional<QString> {
    if (dir.isEmpty()) {
      return {};
    }

    auto path = pattern.arg(dir);
    if (!QFileInfo::exists(path)) {
      return {};
    }

    return path;
  };

  // check inside user directory
  if (auto d =
          tryDir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
                     .first())) {
    return *d;
  }

  return {};
}

QString GameGamebryo::determineMyGamesPath(const QString& gameName,
                                           const QString& appID)
{
  const QString pattern = "%1/My Games/"_L1 % gameName;

  auto tryDir = [&](const QString& dir) -> std::optional<QString> {
    if (dir.isEmpty()) {
      return {};
    }

    auto path = pattern.arg(dir);
    if (!QFileInfo::exists(path)) {
      return {};
    }

    return path;
  };

  // check inside wine prefix
  QString steamLocation = parseSteamLocation(appID, gameName);

  QDir steamuserDocumentsDir = QDir(steamLocation);
  if (!steamuserDocumentsDir.cd(
          QStringLiteral("../../compatdata/%1/pfx/drive_c/users/steamuser/Documents")
              .arg(appID))) {
    return {};
  }

  if (auto d = tryDir(steamuserDocumentsDir.absolutePath())) {
    return *d;
  }

  // check inside user directory
  if (auto d =
          tryDir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
                     .first())) {
    return *d;
  }
  return {};
}

QString GameGamebryo::parseEpicGamesLocation(const QStringList& manifests)
{
  STUB();
  return {};
}
