#include "skyrimvrsavegame.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <uibase/linux/compatibility.h>
#endif
#include <utility.h>

#include "gameskyrimvr.h"

SkyrimVRSaveGame::SkyrimVRSaveGame(QString const& fileName, GameSkyrimVR const* game)
    : GamebryoSaveGame(fileName, game, true)
{
  FileWrapper file(fileName, "TESV_SAVEGAME");  // 10bytes

  uint32_t version;
  FILETIME ftime;
  fetchInformationFields(file, version, m_PCName, m_PCLevel, m_PCLocation, m_SaveNumber,
                         ftime);

  // A file time is a 64-bit value that represents the number of 100-nanosecond
  // intervals that have elapsed since 12:00 A.M. January 1, 1601 Coordinated Universal
  // Time (UTC). So we need to convert that to something useful

  // For some reason, the file time is off by about 6 hours.
  // So we need to subtract those 6 hours from the filetime.
  QDateTime qtime = MOBase::fileTimeToQDateTime(ftime);
  qtime           = qtime.addSecs(-6 * 60 * 60);

  setCreationTime(qtime);
}

void SkyrimVRSaveGame::fetchInformationFields(
    FileWrapper& file, uint32_t& version, QString& playerName, uint16_t& playerLevel,
    QString& playerLocation, uint32_t& saveNumber, FILETIME& creationTime) const
{
  uint32_t headerSize;
  file.read(headerSize);  // header size "TESV_SAVEGAME"
  file.read(version);     // header version 74 (original Skyrim is 79)
  file.read(saveNumber);

  file.read(playerName);

  uint32_t temp;
  file.read(temp);
  playerLevel = static_cast<uint16_t>(temp);

  file.read(playerLocation);

  QString timeOfDay;
  file.read(timeOfDay);

  QString race;
  file.read(race);  // race name (i.e. BretonRace)

  file.skip<uint16_t>();  // Player gender (0 = male)
  file.skip<float>(2);    // experience gathered, experience required

  file.read(creationTime);  // filetime
}

std::unique_ptr<GamebryoSaveGame::DataFields> SkyrimVRSaveGame::fetchDataFields() const
{
  FileWrapper file(getFilepath(), "TESV_SAVEGAME");  // 10bytes

  uint32_t version = 0;
  {
    QString dummyName, dummyLocation;
    uint16_t dummyLevel;
    uint32_t dummySaveNumber;
    FILETIME dummyTime;

    fetchInformationFields(file, version, dummyName, dummyLevel, dummyLocation,
                           dummySaveNumber, dummyTime);
  }

  std::unique_ptr<DataFields> fields = std::make_unique<DataFields>();

  uint32_t width;
  uint32_t height;
  file.read(width);
  file.read(height);

  uint16_t compressionType;
  file.read(compressionType);
  file.setCompressionType(compressionType);

  fields->Screenshot = file.readImage(width, height, 320, true);

  file.openCompressedData();

  uint8_t saveGameVersion = file.readChar();
  uint8_t pluginInfoSize  = file.readChar();
  uint16_t other          = file.readShort();  // Unknown

  fields->Plugins = file.readPlugins(1);  // Just empty data

  if (saveGameVersion >= 78) {
    fields->LightPlugins = file.readLightPlugins();
  }

  file.closeCompressedData();

  return fields;
}
