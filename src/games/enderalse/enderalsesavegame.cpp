#include "enderalsesavegame.h"

#include <utils.h>

#ifdef __unix__
#include "linux/windowsTypes.h"
#else
#include <Windows.h>
#endif

using namespace Qt::Literals::StringLiterals;

EnderalSESaveGame::EnderalSESaveGame(QString const& fileName, GameEnderalSE const* game)
    : GamebryoSaveGame(fileName, game, true)
{
  FileWrapper file(fileName, u"TESV_SAVEGAME"_s);  // 10bytes

  uint32_t version;
  FILETIME ftime;
  fetchInformationFields(file, version, m_PCName, m_PCLevel, m_PCLocation, m_SaveNumber,
                         ftime);

  // A file time is a 64-bit value that represents the number of 100-nanosecond
  // intervals that have elapsed since 12:00 A.M. January 1, 1601 Coordinated Universal
  // Time (UTC). So we need to convert that to something useful

  // For some reason, the file time is off by about 6 hours.
  // So we need to subtract those 6 hours from the filetime.
  QDateTime dt = fileTimeToQDateTime(ftime);

  // add -6 hours
  dt = dt.addSecs(-60 * 60 * 6);

  setCreationTime(dt);
}

void EnderalSESaveGame::fetchInformationFields(
    FileWrapper& file, uint32_t& version, QString& playerName,
    uint16_t& playerLevel, QString& playerLocation, uint32_t& saveNumber,
    FILETIME& creationTime) const
{
  uint32_t headerSize;
  file.read(headerSize);  // header size "TESV_SAVEGAME"
  file.read(version);
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
  file.skip<float>(2);          // experience gathered, experience required

  file.read(creationTime);  // filetime
}

std::unique_ptr<GamebryoSaveGame::DataFields> EnderalSESaveGame::fetchDataFields() const
{
  FileWrapper file(getFilepath(), u"TESV_SAVEGAME"_s);  // 10bytes

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

  bool alpha = false;

  // compatibility between LE and SE:
  //  SE has an additional uin16_t for compression
  //  SE uses an alpha channel, whereas LE does not
  if (version == 12) {
    uint16_t compressionType;
    file.read(compressionType);
    file.setCompressionType(compressionType);
    alpha = true;
  }

  fields->Screenshot = file.readImage(width, height, 320, alpha);

  file.openCompressedData();

  uint8_t saveGameVersion = file.readChar();
  [[maybe_unused]] uint8_t pluginInfoSize  = file.readChar();
  [[maybe_unused]] uint16_t other          = file.readShort();  // Unknown

  fields->Plugins = file.readPlugins(1);  // Just empty data

  if (saveGameVersion >= 78) {
    fields->LightPlugins = file.readLightPlugins();
  }

  file.closeCompressedData();

  return fields;
}
