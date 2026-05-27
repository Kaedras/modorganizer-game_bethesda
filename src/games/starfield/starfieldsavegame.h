#ifndef STARFIELDSAVEGAME_H
#define STARFIELDSAVEGAME_H

#include "gamebryosavegame.h"
#include <QDataStream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <linux/compatibility.h>
#endif

class GameStarfield;

class StarfieldSaveGame : public GamebryoSaveGame
{
public:
  const QStringList v122CorePlugins = {"Starfield.esm", "Constellation.esm",
                                       "OldMars.esm",   "BlueprintShips-Starfield.esm",
                                       "SFBGS003.esm",  "SFBGS006.esm",
                                       "SFBGS007.esm",  "SFBGS008.esm"};

  StarfieldSaveGame(QString const& fileName, GameStarfield const* game);

protected:
  // Fetch easy-to-access information.
  void getData(FileWrapper& file) const;

  void fetchInformationFields(FileWrapper& file, uint32_t& saveNumber,
                              uint8_t& saveVersion, QString& playerName,
                              uint16_t& playerLevel, QString& playerLocation,
                              FILETIME& creationTime) const;

  std::unique_ptr<DataFields> fetchDataFields() const override;
};

#endif  // STARFIELDSAVEGAME_H
