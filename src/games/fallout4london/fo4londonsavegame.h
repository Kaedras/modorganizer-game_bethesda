#ifndef FO4LONDONSAVEGAME_H
#define FO4LONDONSAVEGAME_H

#include "gamebryosavegame.h"

#ifdef __unix__
#include "linux/windowsTypes.h"
#else
#include <Windows.h>
#endif

class GameFallout4London;

class Fallout4LondonSaveGame : public GamebryoSaveGame
{
public:
  Fallout4LondonSaveGame(QString const& fileName, GameFallout4London const* game);

protected:
  // Fetch easy-to-access information.
  void fetchInformationFields(FileWrapper& file, unsigned long& saveNumber,
                              QString& playerName, unsigned short& playerLevel,
                              QString& playerLocation, FILETIME& creationTime) const;

  std::unique_ptr<DataFields> fetchDataFields() const override;
};

#endif  // FO4LONDONSAVEGAME_H
