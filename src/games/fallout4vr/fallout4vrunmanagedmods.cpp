#include "fallout4vrunmanagedmods.h"

using namespace Qt::Literals::StringLiterals;

Fallout4VRUnmangedMods::Fallout4VRUnmangedMods(const GameGamebryo* game)
    : GamebryoUnmangedMods(game)
{}

Fallout4VRUnmangedMods::~Fallout4VRUnmangedMods() {}

QStringList Fallout4VRUnmangedMods::mods(bool onlyOfficial) const
{
  QStringList result;

  QStringList pluginList   = game()->primaryPlugins();
  QStringList otherPlugins = game()->DLCPlugins();
  otherPlugins.append(game()->CCPlugins());
  for (const QString& plugin : otherPlugins) {
    pluginList.removeAll(plugin);
  }
  QDir dataDir(game()->dataDirectory());
  for (const QString& fileName : dataDir.entryList({u"*.esp"_s, u"*.esl"_s, u"*.esm"_s})) {
    if (!pluginList.contains(fileName, Qt::CaseInsensitive)) {
      if (!onlyOfficial || pluginList.contains(fileName, Qt::CaseInsensitive)) {
        result.append(fileName.chopped(4));  // trims the extension off
      }
    }
  }

  return result;
}

QStringList Fallout4VRUnmangedMods::secondaryFiles(const QString& modName) const
{
  // file extension in FO4 is .ba2 instead of bsa
  QStringList archives;
  QDir dataDir = game()->dataDirectory();
  for (const QString& archiveName : dataDir.entryList({modName % u"*.ba2"_s})) {
    archives.append(dataDir.absoluteFilePath(archiveName));
  }
  return archives;
}

QString Fallout4VRUnmangedMods::displayName(const QString& modName) const
{
  // unlike in earlier games, in fallout 4 the file name doesn't correspond to
  // the public name
  if (modName.compare(u"dlcrobot"_s, Qt::CaseInsensitive) == 0) {
    return u"Automatron"_s;
  } else if (modName.compare(u"dlcworkshop01"_s, Qt::CaseInsensitive) == 0) {
    return u"Wasteland Workshop"_s;
  } else if (modName.compare(u"dlccoast"_s, Qt::CaseInsensitive) == 0) {
    return u"Far Harbor"_s;
  } else if (modName.compare(u"dlcworkshop02"_s, Qt::CaseInsensitive) == 0) {
    return u"Contraptions Workshop"_s;
  } else if (modName.compare(u"dlcworkshop03"_s, Qt::CaseInsensitive) == 0) {
    return u"Vault-Tec Workshop"_s;
  } else if (modName.compare(u"dlcnukaworld"_s, Qt::CaseInsensitive) == 0) {
    return u"Nuka-World"_s;
  } else if (modName.compare(u"dlcultrahighresolution"_s, Qt::CaseInsensitive) == 0) {
    return u"Ultra High Resolution Texture Pack"_s;
  } else {
    return modName;
  }
}
