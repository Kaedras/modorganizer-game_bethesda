#include "../gameskyrimse.h"

using namespace MOBase;

QString GameSkyrimSE::identifyGamePath() const
{
  QMap<QString, QString> paths = {
      {"Software\\Bethesda Softworks\\" + gameName(), "Installed Path"},
      {"Software\\GOG.com\\Games\\1162721350", "path"},
      {"Software\\GOG.com\\Games\\1711230643", "path"},
  };

  QString result;
  for (auto& path : paths.toStdMap()) {
    result = findInRegistry(HKEY_LOCAL_MACHINE, path.first.toStdWString().c_str(),
                            path.second.toStdWString().c_str());
    if (!result.isEmpty())
      break;
  }

  // Check Epic Games Manifests
  // AppName: ac82db5035584c7f8a2c548d98c86b2c
  //      AE Update: 5d600e4f59974aeba0259c7734134e27
  if (result.isEmpty()) {
    result = parseEpicGamesLocation(
        {"ac82db5035584c7f8a2c548d98c86b2c", "5d600e4f59974aeba0259c7734134e27"});
  }

  return result;
}
