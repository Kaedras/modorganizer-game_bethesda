/*
Copyright (C) 2015 Sebastian Herbord. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "gamebryolocalsavegames.h"
#include "registry.h"
#include <QtDebug>
#include <iprofile.h>

#include "gamegamebryo.h"

using namespace MOBase;
using namespace Qt::StringLiterals;

GamebryoLocalSavegames::GamebryoLocalSavegames(const GameGamebryo* game,
                                               const QString& iniFileName)
    : m_Game{game}, m_IniFileName(iniFileName)
{}

MappingType GamebryoLocalSavegames::mappings(const QDir& profileSaveDir) const
{
  return {{profileSaveDir.absolutePath(), localSavesDirectory().absolutePath(), true,
           true}};
}

QString GamebryoLocalSavegames::localSavesDummy() const
{
  return "__MO_Saves/";
}

QDir GamebryoLocalSavegames::localSavesDirectory() const
{
  return QDir(m_Game->myGamesPath()).absoluteFilePath(localSavesDummy());
}

QDir GamebryoLocalSavegames::localGameDirectory() const
{
  return QDir(m_Game->myGamesPath()).absolutePath();
}

bool GamebryoLocalSavegames::prepareProfile(MOBase::IProfile* profile)
{
  bool enable = profile->localSavesEnabled();

  QString basePath    = profile->localSettingsEnabled()
                            ? profile->absolutePath()
                            : localGameDirectory().absolutePath();
  QString iniFilePath = basePath + "/" + m_IniFileName;
  QString saveIni     = profile->absolutePath() + "/" + "savepath.ini";

  // Get the current sLocalSavePath
  QString currentPath =
      ReadRegistryValue(u"General"_s, u"sLocalSavePath"_s, u"SKIP_ME"_s, iniFilePath);
  bool alreadyEnabled = currentPath == localSavesDummy();

  // Get the current bUseMyGamesDirectory
  QString currentMyGames = ReadRegistryValue(u"General"_s, u"bUseMyGamesDirectory"_s,
                                             u"SKIP_ME"_s, iniFilePath);

  // Create the __MO_Saves directory if local saves are enabled and it doesn't exist
  if (enable) {
    QDir saves = localSavesDirectory();
    if (!saves.exists()) {
      saves.mkdir(".");
    }
  }

  // Set the path to __MO_Saves if it's not already
  if (enable && !alreadyEnabled) {
    // If the path is not blank, save it to savepath.ini
    if (currentPath == "SKIP_ME"_L1) {
      WriteRegistryValue(u"General"_s, u"sLocalSavePath"_s, currentPath, saveIni);
    }
    if (currentMyGames == "SKIP_ME"_L1) {
      WriteRegistryValue(u"General"_s, u"bUseMyGamesDirectory"_s, currentMyGames,
                         saveIni);
    }
    WriteRegistryValue(u"General"_s, u"sLocalSavePath"_s, localSavesDummy(),
                       iniFilePath);
    WriteRegistryValue(u"General"_s, u"bUseMyGamesDirectory"_s, u"1"_s, iniFilePath);
  }

  // Get rid of the local saves setting if it's still there
  if (!enable && alreadyEnabled) {
    // If savepath.ini exists, use it and delete it
    if (QFile::exists(saveIni)) {
      QString savedPath =
          ReadRegistryValue(u"General"_s, u"sLocalSavePath"_s, u"DELETE_ME"_s, saveIni);
      QString savedMyGames = ReadRegistryValue(u"General"_s, u"bUseMyGamesDirectory"_s,
                                               u"DELETE_ME"_s, saveIni);
      if (savedPath == "DELETE_ME"_L1) {
        WriteRegistryValue(u"General"_s, u"sLocalSavePath"_s, savedPath, iniFilePath);
      } else {
        WriteRegistryValue(u"General"_s, u"sLocalSavePath"_s, QString(), iniFilePath);
      }

      if (savedMyGames == "DELETE_ME"_L1) {
        WriteRegistryValue(u"General"_s, u"bUseMyGamesDirectory"_s, savedMyGames,
                           iniFilePath);
      } else {
        WriteRegistryValue(u"General"_s, u"bUseMyGamesDirectory"_s, QString(),
                           iniFilePath);
      }
      QFile::remove(saveIni);
    }
    // Otherwise just delete the setting
    else {
      WriteRegistryValue(u"General"_s, u"sLocalSavePath"_s, QString(), iniFilePath);
      WriteRegistryValue(u"General"_s, u"bUseMyGamesDirectory"_s, QString(),
                         iniFilePath);
    }
  }

  return enable != alreadyEnabled;
}
