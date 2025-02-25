#include "gamegamebryo.h"

#include "bsainvalidation.h"
#include "dataarchives.h"
#include "gamebryomoddatacontent.h"
#include "gamebryosavegame.h"
#include "gameplugins.h"
#include "iprofile.h"
#include "log.h"
#include "savegameinfo.h"
#include "scopeguard.h"
#include "scriptextender.h"
#include "utility.h"
#include "vdf_parser.h"

#include <uibase/peextractor.h>

#include <QBuffer>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QJsonDocument>
#include <QSettings>
#include <QStandardPaths>

#include <QtDebug>
#include <QtGlobal>

#include <optional>
#include <string>
#include <vector>

#include "stub.h"

using namespace Qt::StringLiterals;

// helper function
int getAppIdByGameName(const QString& gameName)
{
  if (gameName == "Morrowind"_L1) {
    return 22320;
  }
  if (gameName == "Oblivion"_L1) {
    return 22330;
  }
  if (gameName == "Nehrim"_L1) {
    return 1014940;
  }
  if (gameName == "Skyrim"_L1) {
    return 72850;
  }
  if (gameName == "Skyrim Special Edition"_L1) {
    return 489830;
  }
  if (gameName == "Skyrim VR"_L1) {
    return 611670;
  }
  if (gameName == "Enderal"_L1) {
    return 933480;
  }
  if (gameName == "Enderal Special Edition"_L1) {
    return 976620;
  }
  if (gameName == "Fallout3"_L1) {
    return 22370;  // GOTY, normal is 22300
  }
  if (gameName == "FalloutNV"_L1) {
    return 22380;
  }
  if (gameName == "Fallout4"_L1) {
    return 377160;
  }
  if (gameName == "Fallout4VR"_L1) {
    return 611660;
  }
  if (gameName == "Fallout 76"_L1) {
    return 1151340;
  }
  if (gameName == "Starfield"_L1) {
    return 1716740;
  }
  return 0;
}

WORD GameGamebryo::getArch(QString const& program) const
{
  // brief summary of this function:
  // seek to 0x3C
  // the next 4 bytes contain either "PE\0\0" or the offset of the aforementioned value, in which case seek to the offset
  // read the 2 bytes after "PE\0\0" and return them as uint16_t

  static constexpr uint16_t offset = 0x3C;
  static constexpr char pe[4] = {'P', 'E', 0, 0};

  QFile file(program);
  if (!file.open(QIODevice::ReadOnly)) {
    return 0;
  }

  // this location contains either "PE\0\0" or the offset the value can be found
  if (!file.seek(offset)) {
    return 0;
  }
  uint32_t peValue;
  if (file.read(reinterpret_cast<char*>(&peValue), sizeof(uint32_t)) != sizeof(uint32_t)) {
    return 0;
  }

  if (memcmp(&peValue, pe, sizeof(uint32_t)) != 0) {
    // read value is offset, seek to real value
    // convert bytes to uint64
    if (!file.seek(peValue)) {
      return 0;
    }
    if (file.read(reinterpret_cast<char*>(&peValue), sizeof(uint32_t)) != sizeof(uint32_t)) {
      return 0;
    }
    if (memcmp(&peValue, pe, sizeof(uint32_t)) != 0) {
      return 0;
    }
  }

  uint16_t value;
  if (file.read(reinterpret_cast<char*>(&value), sizeof(uint16_t)) != sizeof(uint16_t)) {
    return 0;
  }

  return value;
}

QString GameGamebryo::identifyGamePath() const
{
  return parseSteamLocation(steamAPPId(), gameShortName());
}

/*static*/ QString GameGamebryo::getLootPath()
{
  STUB();
  return {};
}

std::unique_ptr<BYTE[]> GameGamebryo::getRegValue([[maybe_unused]] HKEY key, [[maybe_unused]] LPCWSTR path, [[maybe_unused]] LPCWSTR value,
                                                  [[maybe_unused]] DWORD flags, [[maybe_unused]] LPDWORD type = nullptr)
{
  // this function is probably not needed
  return {};
}

QString GameGamebryo::findInRegistry([[maybe_unused]] HKEY baseKey, [[maybe_unused]] LPCWSTR path, [[maybe_unused]] LPCWSTR value)
{
  // this function is probably not needed
  return {};
}

QString GameGamebryo::getKnownFolderPath(REFKNOWNFOLDERID folderId,
                                         [[maybe_unused]] bool useDefault)
{
  // use folderId as QStandardPaths::StandardLocation
  return QStandardPaths::standardLocations(
             static_cast<QStandardPaths::StandardLocation>(folderId))
      .first();
}

QString GameGamebryo::getSpecialPath([[maybe_unused]] const QString& name)
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

  // check inside wine prefix
  QString steamLocation =
      parseSteamLocation(QString::number(getAppIdByGameName(gameName)), gameName);

  QDir steamuserDocumentsDir = QDir(steamLocation);
  if (!steamuserDocumentsDir.cd(
          QStringLiteral("../../compatdata/%1/pfx/drive_c/users/steamuser/Documents")
              .arg(getAppIdByGameName(gameName)))) {
    return {};
  }

  if (auto d = tryDir(steamuserDocumentsDir.absolutePath())) {
    return *d;
  }
  return {};
}

QString GameGamebryo::parseEpicGamesLocation(const QStringList& manifests)
{
  // todo: check if this could even be implemented properly
  (void)manifests;

  STUB();
  return {};
}

QString GameGamebryo::parseSteamLocation(const QString& appid,
                                         const QString& directoryName)
{
  QString steamLibraryLocation;
  QString steamLibraries =
      QDir::homePath() % QStringLiteral("/.steam/steam/config/libraryfolders.vdf");
  if (QFile(steamLibraries).exists()) {
    std::ifstream file(steamLibraries.toStdString());
    auto root = tyti::vdf::read(file);
    for (const auto& child : root.childs) {
      tyti::vdf::object* library = child.second.get();
      auto apps                  = library->childs["apps"];
      if (apps->attribs.contains(appid.toStdString())) {
        steamLibraryLocation = QString::fromStdString(library->attribs["path"]);
        break;
      }
    }
  }
  if (!steamLibraryLocation.isEmpty()) {
    QString gameLocation =
        steamLibraryLocation % "/steamapps/common/"_L1 % directoryName;
    if (QDir(gameLocation).exists())
      return gameLocation;
  }

  return {};
}

QString GameGamebryo::getFileVersion(const QString& fileName)
{
  QFile f(fileName);
  QBuffer buf;
  if (!f.open(QIODeviceBase::ReadOnly) || !buf.open(QIODeviceBase::ReadWrite)) {
    return {};
  }
  if (!PeExtractor::loadVersionData(&f, &buf)) {
    return {};
  }
  buf.seek(0);
  QDataStream stream(&buf);
  QString fileVersion, productVersion;
  stream >> fileVersion >> productVersion;
  return fileVersion;
}