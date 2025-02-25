#include "fo4londonsavegame.h"

#ifdef __unix__
#include "linux/windowsTypes.h"
#include "linux/windowsFunctionWrapper.h"
#else
#include <Windows.h>
#endif

#include "gamefo4london.h"

using namespace Qt::Literals::StringLiterals;

Fallout4LondonSaveGame::Fallout4LondonSaveGame(QString const& fileName,
                                               GameFallout4London const* game)
    : GamebryoSaveGame(fileName, game, true)
{
  FileWrapper file(getFilepath(), u"FO4_SAVEGAME"_s);

  FILETIME creationTime;
  fetchInformationFields(file, m_SaveNumber, m_PCName, m_PCLevel, m_PCLocation,
                         creationTime);

  // A file time is a 64-bit value that represents the number of 100-nanosecond
  // intervals that have elapsed since 12:00 A.M. January 1, 1601 Coordinated Universal
  // Time (UTC). So we need to convert that to something useful
  SYSTEMTIME ctime;
  ::FileTimeToSystemTime(&creationTime, &ctime);

  setCreationTime(ctime);
}

void Fallout4LondonSaveGame::fetchInformationFields(
    FileWrapper& file, uint32_t& saveNumber, QString& playerName,
    uint16_t& playerLevel, QString& playerLocation, FILETIME& creationTime) const
{
  file.skip<uint32_t>();  // header size
  file.skip<uint32_t>();       // header version
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
  file.skip<float>(2);          // experience gathered, experience required

  file.read(creationTime);
}

std::unique_ptr<GamebryoSaveGame::DataFields>
Fallout4LondonSaveGame::fetchDataFields() const
{
  FileWrapper file(getFilepath(), u"FO4_SAVEGAME"_s);  // 10bytes

  {
    QString dummyName, dummyLocation;
    uint16_t dummyLevel;
    uint32_t dummySaveNumber;
    FILETIME dummyTime;

    fetchInformationFields(file, dummySaveNumber, dummyName, dummyLevel, dummyLocation,
                           dummyTime);
  }

  QString ignore;
  std::unique_ptr<DataFields> fields = std::make_unique<DataFields>();

  fields->Screenshot = file.readImage(384, true);

  uint8_t saveGameVersion = file.readChar();
  file.read(ignore);      // game version
  file.skip<uint32_t>();  // plugin info size

  fields->Plugins = file.readPlugins();
  if (saveGameVersion >= 68) {
    fields->LightPlugins = file.readLightPlugins();
  }

  return fields;
}
