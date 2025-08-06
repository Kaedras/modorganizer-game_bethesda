#ifndef FALLOUT76SAVEGAME_H
#define FALLOUT76SAVEGAME_H

#include <Windows.h>

#include "gamebryosavegame.h"

class GameFallout76;

class Fallout76SaveGame : public GamebryoSaveGame
{
public:
  Fallout76SaveGame(QString const& fileName, GameFallout76 const* game);

protected:
  // Fetch easy-to-access information.
  void fetchInformationFields(FileWrapper& wrapper, QString playerName,
                              uint16_t playerLevel, QString playerLocation,
                              uint32_t saveNumber, FILETIME& creationTime) const;

  std::unique_ptr<DataFields> fetchDataFields() const override;
};

#endif  // FALLOUT76SAVEGAME_H
