#ifndef STARFIELDSAVEGAME_H
#define STARFIELDSAVEGAME_H

#include "gamebryosavegame.h"
#include <QDataStream>

#ifdef __unix__
#include "linux/windowsTypes.h"
#else
#include <Windows.h>
#endif

class GameStarfield;

class StarfieldSaveGame : public GamebryoSaveGame
{
public:
  StarfieldSaveGame(QString const& fileName, GameStarfield const* game);

protected:
  // Fetch easy-to-access information.
  void getData(FileWrapper& file) const;

  void fetchInformationFields(FileWrapper& file, uint32_t& saveNumber,
                              unsigned char& saveVersion, QString& playerName,
                              uint16_t& playerLevel, QString& playerLocation,
                              FILETIME& creationTime) const;

  std::unique_ptr<DataFields> fetchDataFields() const override;
};

#endif  // STARFIELDSAVEGAME_H
