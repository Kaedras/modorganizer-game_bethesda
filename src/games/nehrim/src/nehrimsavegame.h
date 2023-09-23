#ifndef NEHRIMSAVEGAME_H
#define NEHRIMSAVEGAME_H

#include "gamebryosavegame.h"
#include "gamenehrim.h"

class NehrimSaveGame : public GamebryoSaveGame
{
public:
  NehrimSaveGame(QString const& fileName, GameNehrim const* game);

protected:
  // Fetch easy-to-access information.
  void fetchInformationFields(FileWrapper& wrapper, unsigned long& saveNumber,
                              QString& playerName, unsigned short& playerLevel,
                              QString& playerLocation, SYSTEMTIME& creationTime) const;

  std::unique_ptr<DataFields> fetchDataFields() const override;
};

#endif  // NEHRIMSAVEGAME_H
