#include "fallout76savegame.h"

#include "gamefallout76.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <linux/compatibility.h>
#endif

#include <utility.h>

Fallout76SaveGame::Fallout76SaveGame(QString const& fileName, GameFallout76 const* game)
    : GamebryoSaveGame(fileName, game, true)
{
  FileWrapper file(fileName, "FO76_SAVEGAME");

  FILETIME ftime;
  fetchInformationFields(file, m_PCName, m_PCLevel, m_PCLocation, m_SaveNumber, ftime);

  // A file time is a 64-bit value that represents the number of 100-nanosecond
  // intervals that have elapsed since 12:00 A.M. January 1, 1601 Coordinated Universal
  // Time (UTC). So we need to convert that to something useful

  setCreationTime(MOBase::fileTimeToQDateTime(ftime));
}

void Fallout76SaveGame::fetchInformationFields(FileWrapper& file, QString playerName,
                                               uint16_t playerLevel,
                                               QString playerLocation,
                                               uint32_t saveNumber,
                                               FILETIME& creationTime) const
{

  file.skip<uint32_t>();  // header size
  file.skip<uint32_t>();  // header version
  file.read(saveNumber);

  file.read(playerName);

  uint32_t temp;
  file.read(temp);
  playerLevel = static_cast<uint16_t>(temp);
  file.read(playerLocation);

  QString ignore;
  file.read(ignore);  // playtime as ascii hh.mm.ss
  file.read(ignore);  // race name (i.e. BretonRace)

  file.skip<uint16_t>();  // Player gender (0 = male)
  file.skip<float>(2);    // experience gathered, experience required

  FILETIME ftime;
  file.read(ftime);
}

std::unique_ptr<GamebryoSaveGame::DataFields> Fallout76SaveGame::fetchDataFields() const
{

  FileWrapper file(getFilepath(), "TESV_SAVEGAME");  // 10bytes
  {

    FILETIME ftime;
    fetchInformationFields(file, m_PCName, m_PCLevel, m_PCLocation, m_SaveNumber,
                           ftime);
  }

  std::unique_ptr<DataFields> fields = std::make_unique<DataFields>();

  file.readImage(384, true);

  uint8_t saveGameVersion = file.readChar();
  QString ignore;
  file.read(ignore);      // game version
  file.skip<uint32_t>();  // plugin info size

  file.readPlugins();
  if (saveGameVersion >= 68) {
    file.readLightPlugins();
  }

  return fields;
}
