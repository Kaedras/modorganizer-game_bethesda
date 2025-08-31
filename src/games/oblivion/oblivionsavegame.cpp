#include "oblivionsavegame.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <linux/compatibility.h>
#endif

OblivionSaveGame::OblivionSaveGame(QString const& fileName, GameOblivion const* game)
    : GamebryoSaveGame(fileName, game)
{
  FileWrapper file(getFilepath(), "TES4SAVEGAME");
  file.setPluginString(GamebryoSaveGame::StringType::TYPE_BSTRING);

  SYSTEMTIME creationTime;
  fetchInformationFields(file, m_SaveNumber, m_PCName, m_PCLevel, m_PCLocation,
                         creationTime);
  setCreationTime(creationTime);
}

void OblivionSaveGame::fetchInformationFields(FileWrapper& file, uint32_t& saveNumber,
                                              QString& playerName,
                                              uint16_t& playerLevel,
                                              QString& playerLocation,
                                              SYSTEMTIME& creationTime) const
{
  file.skip<uint8_t>();  // Major version
  file.skip<uint8_t>();  // Minor version

  file.skip<SYSTEMTIME>();  // exe last modified (!)

  file.skip<uint32_t>();  // Header version
  file.skip<uint32_t>();  // Header size

  file.read(saveNumber);

  file.read(playerName);
  file.read(playerLevel);
  file.read(playerLocation);

  file.skip<float>();     // game days
  file.skip<uint32_t>();  // game ticks

  // there is a save time stored here. So use it rather than the file time, which
  // could have been copied.
  // Note: This says it uses getlocaltime api to obtain it which is u/s - if so
  // we should ignore this.
  file.read(creationTime);
}

std::unique_ptr<GamebryoSaveGame::DataFields> OblivionSaveGame::fetchDataFields() const
{
  FileWrapper file(getFilepath(), "TES4SAVEGAME");
  file.setPluginString(GamebryoSaveGame::StringType::TYPE_BSTRING);

  std::unique_ptr<DataFields> fields = std::make_unique<DataFields>();

  {
    QString dummyName, dummyLocation;
    uint16_t dummyLevel;
    uint32_t dummySaveNumber;
    SYSTEMTIME dummyTime;

    fetchInformationFields(file, dummySaveNumber, dummyName, dummyLevel, dummyLocation,
                           dummyTime);
  }

  // Note that screenshot size, width, height and data are apparently the same
  // structure
  uint32_t imageSize;
  file.read(imageSize);  // Screenshot size.
  if (imageSize > 0) {
    fields->Screenshot = file.readImage();
  }

  fields->Plugins = file.readPlugins();

  return fields;
}
