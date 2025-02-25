#include "enderalsavegame.h"

#ifdef __unix__
#include "linux/windowsTypes.h"
#include "linux/windowsFunctionWrapper.h"
#else
#include <Windows.h>
#endif

#include "gameenderal.h"

using namespace Qt::Literals::StringLiterals;

EnderalSaveGame::EnderalSaveGame(QString const& fileName, GameEnderal const* game)
    : GamebryoSaveGame(fileName, game)
{
  FileWrapper file(getFilepath(), u"TESV_SAVEGAME"_s);

  FILETIME ftime;
  fetchInformationFields(file, m_SaveNumber, m_PCName, m_PCLevel, m_PCLocation, ftime);

  // A file time is a 64-bit value that represents the number of 100-nanosecond
  // intervals that have elapsed since 12:00 A.M. January 1, 1601 Coordinated Universal
  // Time (UTC). So we need to convert that to something useful
  SYSTEMTIME ctime;
  ::FileTimeToSystemTime(&ftime, &ctime);
  setCreationTime(ctime);
}

void EnderalSaveGame::fetchInformationFields(
    FileWrapper& file, uint32_t& saveNumber, QString& playerName,
    uint16_t& playerLevel, QString& playerLocation, FILETIME& creationTime) const
{
  file.skip<uint32_t>();  // header size
  file.skip<uint32_t>();  // header version
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

  file.read(creationTime);
}

std::unique_ptr<GamebryoSaveGame::DataFields> EnderalSaveGame::fetchDataFields() const
{
  FileWrapper file(getFilepath(), u"TESV_SAVEGAME"_s);
  std::unique_ptr<DataFields> fields = std::make_unique<DataFields>();

  {
    QString dummyName, dummyLocation;
    uint16_t dummyLevel;
    uint32_t dummySaveNumber;
    FILETIME dummyTime;

    fetchInformationFields(file, dummySaveNumber, dummyName, dummyLevel, dummyLocation,
                           dummyTime);
  }

  fields->Screenshot = file.readImage();

  file.skip<uint8_t>();  // form version
  file.skip<uint32_t>();  // plugin info size

  fields->Plugins = file.readPlugins();

  return fields;
}
