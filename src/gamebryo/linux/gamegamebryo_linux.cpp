#include "../gamegamebryo.h"
#include "util.h"
#include "vdf_parser.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QRegularExpression>
#include <QString>
#include <iostream>
#include <pwd.h>
#include <sys/types.h>
#include <utility.h>

using namespace MOBase;
using namespace Qt::StringLiterals;

// set the default variant to steam to enable getting the appid earlier
GameGamebryo::GameGamebryo() : m_GameVariant(u"Steam"_s), m_Organizer(nullptr) {}

void GameGamebryo::setPrefixPath(const QString& path)
{
  // get username
  // this is steamuser in proton, and the normal username in wine
  if (QFile::exists(path % "/pfx"_L1)) {
    m_PrefixUserPath = path % "/pfx/drive_c/users/steamuser"_L1;
  } else {
    bool found = false;
    QDir usersDir(path % "/drive_c/users"_L1);
    QStringList userDirs = usersDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    // there are usually two user dirs: Public and the normal user
    if (userDirs.size() == 2) {
      for (const QString& dir : userDirs) {
        if (dir != "Public"_L1) {
          m_PrefixUserPath = path % "/drive_c/users/"_L1 % dir;
          found            = true;
          break;
        }
      }
    } else {
      // parse user.reg as fallback
      QFile userReg(path % "/user.reg");
      if (!userReg.open(QIODevice::ReadOnly | QIODevice::Text)) {
        log::error("Cannot set prefix path: Error opening {}, {}", userReg.fileName(),
                   userReg.errorString());
        return;
      }

      const QByteArray data = userReg.readAll();
      QTextStream in(data);
      QString line;
      while (in.readLineInto(&line)) {
        if (line.startsWith("\"USERPROFILE\"="_L1)) {
          line.remove(0, 15);
          line.removeLast();
          line.replace("\\\\"_L1, "/"_L1);
          line.replace("C:"_L1, "/drive_c"_L1);
          m_PrefixUserPath = path % line;
          found            = true;
          break;
        }
      }
    }
    if (!found) {
      log::error(
          "Error setting prefix path: could not find user directory inside prefix");
      return;
    }
  }

  m_PrefixPath = path;
  // update m_MyGamesPath
  m_MyGamesPath = m_PrefixUserPath % "/Documents/My Games/"_L1 % gameName();
}

QString GameGamebryo::identifyGamePath() const
{
  return parseSteamLocation(steamAPPId(), gameName());
}

QString GameGamebryo::getLootPath()
{
  constexpr auto desktopFile = "/io.github.loot.loot.desktop"_L1;

  const QStringList applicationDirs =
      QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);
  for (const auto& dir : applicationDirs) {
    QString path = dir % desktopFile;
    if (QFileInfo::exists(path)) {
      return path;
    }
  }

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
      QFile file(filePath);
      if (!file.open(QIODevice::WriteOnly)) {
        log::error("Error creating file {}, {}", filePath, file.errorString());
      }
    }
  }
}

QString GameGamebryo::localAppFolder() const
{
  if (m_PrefixUserPath.isEmpty()) {
    return {};
  }
  return QStringLiteral("%1/AppData/Local").arg(m_PrefixUserPath);
}

std::unique_ptr<BYTE[]> GameGamebryo::getRegValue(HKEY, LPCWSTR, LPCWSTR, DWORD,
                                                  LPDWORD)
{
  // no-op
  return {};
}

QString GameGamebryo::findInRegistry(HKEY, LPCWSTR, LPCWSTR)
{
  // no-op
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
  // no-op
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

  // check inside the user directory
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

  // check inside the user directory
  if (auto d =
          tryDir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
                     .first())) {
    return *d;
  }
  return {};
}

QString GameGamebryo::parseEpicGamesLocation(const QStringList& manifests)
{
  /*
   * TODO:
   *  - support native EGS once it's available
   *  - check for other applications that should be supported besides heroic
   */

  QString heroicDir =
      QStandardPaths::locate(QStandardPaths::GenericConfigLocation, u"heroic"_s,
                             QStandardPaths::LocateDirectory);
  if (heroicDir.isEmpty()) {
    return {};
  }

  QFile installed(heroicDir % "/legendaryConfig/legendary/installed.json"_L1);
  if (!installed.open(QIODevice::ReadOnly)) {
    qWarning("Couldn't open Heroic installed.json file.");
    return {};
  }

  const QByteArray installedData = installed.readAll();
  const QJsonDocument installedJson(QJsonDocument::fromJson(installedData));

  for (const auto& manifest : manifests) {
    const QJsonValue value = installedJson[manifest];
    if (!value.isUndefined()) {
      return value["install_path"_L1].toString();
    }
  }

  return {};
}
