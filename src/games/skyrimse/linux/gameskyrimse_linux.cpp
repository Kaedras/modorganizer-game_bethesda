#include "../gameskyrimse.h"

QString GameSkyrimSE::identifyGamePath() const
{
  // TODO: check gog
  QString result = parseSteamLocation(QStringLiteral("489830"), gameDirectoryName());

  // Check Epic Games Manifests
  // AppName: ac82db5035584c7f8a2c548d98c86b2c
  //      AE Update: 5d600e4f59974aeba0259c7734134e27
  if (result.isEmpty()) {
    result = parseEpicGamesLocation(
        {"ac82db5035584c7f8a2c548d98c86b2c", "5d600e4f59974aeba0259c7734134e27"});
  }

  return result;
}
