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

#include "exiftool/ExifTool.h"

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

WORD GameGamebryo::getArch(QString const& program) const
{
  STUB();
  return 0;
}

QString GameGamebryo::identifyGamePath() const
{
  // perhaps try to look into prefix registry?
  // we could get the prefix path from steam/config/libraryfolders.vdf
  // but appid would be required
  // todo: check if steamAPPId() can be called at this point
  //  if not, get appid from gameShortName() instead?
  STUB();
  return {};
}

/*static*/ QString GameGamebryo::getLootPath()
{
  STUB();
  return {};
}

std::unique_ptr<BYTE[]> GameGamebryo::getRegValue(HKEY key, LPCWSTR path, LPCWSTR value,
                                                  DWORD flags, LPDWORD type = nullptr)
{
  STUB();
  return {};
}

QString GameGamebryo::findInRegistry(HKEY baseKey, LPCWSTR path, LPCWSTR value)
{
  STUB();
  return {};
}

QString GameGamebryo::getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault)
{
  // use folderId as QStandardPaths::StandardLocation
  return QStandardPaths::standardLocations(
             static_cast<QStandardPaths::StandardLocation>(folderId)).first();
}

QString GameGamebryo::getSpecialPath(const QString& name)
{
  STUB();
  return {};
}

QString GameGamebryo::determineMyGamesPath(const QString& gameName)
{
  const QString pattern = "%1/My Games/" + gameName;

  auto tryDir = [&](const QString& dir) -> std::optional<QString> {
    if (dir.isEmpty()) {
      return {};
    }

    const auto path = pattern.arg(dir);
    if (!QFileInfo(path).exists()) {
      return {};
    }

    return path;
  };

  // check inside user directory
  if (auto d = tryDir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first())) {
    return *d;
  }

  // TODO: check inside wine prefix

  return {};
}

QString GameGamebryo::parseEpicGamesLocation(const QStringList& manifests)
{
  STUB();
  return {};
}

QString GameGamebryo::parseSteamLocation(const QString& appid,
                                         const QString& directoryName)
{
  QString steamLibraryLocation;
  QString steamLibraries =
      QDir::homePath() + QStringLiteral("/.steam/steam/config/libraryfolders.vdf");
  if (QFile(steamLibraries).exists()) {
    std::ifstream file(steamLibraries.toStdString());
    auto root = tyti::vdf::read(file);
    for (auto child : root.childs) {
      tyti::vdf::object* library = child.second.get();
      auto apps                  = library->childs["apps"];
      if (apps->attribs.contains(appid.toStdString())) {
        steamLibraryLocation = QString::fromStdString(library->attribs["path"]);
        break;
      }
    }
  }
  if (!steamLibraryLocation.isEmpty()) {
    QString gameLocation = steamLibraryLocation + "/steamapps/common/" + directoryName;
    if (QDir(gameLocation).exists())
      return gameLocation;
  }

  return {};
}

QString GameGamebryo::getFileVersion(const QString& fileName)
{
    // requires exiftool to be installed
    // create our ExifTool object
    ExifTool *et = new ExifTool();
    // read metadata from the image
    TagInfo *info = et->ImageInfo(QFileInfo(fileName).absoluteFilePath().toStdString().c_str(),NULL,5);

    QString versionInfo;

    if (info) {
        // print returned information
        for (TagInfo *i=info; i; i=i->next) {
            if (strcmp(i->name, "FileVersion") == 0)
            {
                versionInfo = i->value;
            }

            // cout << i->name << " = " << i->value << endl;
            // cout << "  group[0] = " << (i->group[0] ? i->group[0] : "<null>") << endl;// family 0 group name
            // cout << "  group[1] = " << (i->group[1] ? i->group[1] : "<null>") << endl;// family 1 group name
            // cout << "  group[2] = " << (i->group[2] ? i->group[2] : "<null>") << endl;// family 2 group name
            // cout << "  name = " << (i->name ? i->name : "<null>") << endl;      // tag name
            // cout << "  desc = " << (i->desc ? i->desc : "<null>") << endl;      // tag description
            // cout << "  id = " << (i->id ? i->id : "<null>" ) << endl;           // tag ID
            // cout << "  value = " << (i->value ? i->value : "<null>") << endl;   // converted value
            // cout << "  valueLen = " << i->valueLen << endl;                     // length of value in bytes (not including null terminator)
            // cout << "  num = " << (i->num ? i->num :"<null>") << endl;          // "numerical" value
            // cout << "  numLen = " << i->numLen << endl;                         // length of numerical value
            // cout << "  copyNum = " << i->copyNum << endl;                       // copy number for this tag name
        }
        // we are responsible for deleting the information when done
        delete info;
    } else if (et->LastComplete() <= 0) {
        std::cerr << "Error executing exiftool!" << std::endl;
    }
    // print exiftool stderr messages
    char *err = et->GetError();
    if (err) std::cerr << err;
    delete et;      // delete our ExifTool object

    return versionInfo;
}