#ifndef ENDERALSESAVEGAME_H
#define ENDERALSESAVEGAME_H

#include "gamebryosavegame.h"
#include "gameenderalse.h"

namespace MOBase
{
class IPluginGame;
}

class EnderalSESaveGame : public GamebryoSaveGame
{
public:
  EnderalSESaveGame(QString const& fileName, GameEnderalSE const* game);

protected:
  // Fetch easy-to-access information.
  void fetchInformationFields(FileWrapper& wrapper, uint32_t& version,
                              QString& playerName, uint16_t& playerLevel,
                              QString& playerLocation, uint32_t& saveNumber,
                              FILETIME& creationTime) const;

  std::unique_ptr<DataFields> fetchDataFields() const override;
};

#endif  // _SKYRIMSESAVEGAME_H
