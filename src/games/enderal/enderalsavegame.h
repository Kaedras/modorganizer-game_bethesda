#ifndef ENDERALSAVEGAME_H
#define ENDERALSAVEGAME_H

#include "gamebryosavegame.h"

#ifdef __unix__
#include "linux/windowsTypes.h"
#else
#include <Windows.h>
#endif

class GameEnderal;

class EnderalSaveGame : public GamebryoSaveGame
{
public:
  EnderalSaveGame(QString const& fileName, GameEnderal const* game);

protected:
  // Fetch easy-to-access information.
  void fetchInformationFields(FileWrapper& wrapper, uint32_t& saveNumber,
                              QString& playerName, uint16_t& playerLevel,
                              QString& playerLocation, FILETIME& creationTime) const;

  std::unique_ptr<DataFields> fetchDataFields() const override;
};

#endif  // ENDERALSAVEGAME_H
