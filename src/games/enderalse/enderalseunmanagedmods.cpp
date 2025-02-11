#include "enderalseunmanagedmods.h"

using namespace Qt::Literals::StringLiterals;

EnderalSEUnmangedMods::EnderalSEUnmangedMods(const GameGamebryo* game)
    : GamebryoUnmangedMods(game)
{}

EnderalSEUnmangedMods::~EnderalSEUnmangedMods() {}

QStringList EnderalSEUnmangedMods::mods(bool onlyOfficial) const
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
