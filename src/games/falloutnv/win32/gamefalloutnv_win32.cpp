#include "../gamefalloutnv.h"

#include <executableinfo.h>
#include <gamebryogameplugins.h>
#include <scriptextender.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QString GameFalloutNV::identifyGamePath() const
{
  auto result = GameGamebryo::identifyGamePath();  // Default registry path
  // EPIC Game Store
  if (result.isEmpty()) {
    /**
         * Basegame: 5daeb974a22a435988892319b3a4f476
         * Dead Money:
     * b290229eb58045cbab9501640f3278f3
         * Honest Hearts:
     * 562d4a2c1b3147b089a7c453e3ddbcbe
         * Old World Blues:
     * c8dae1ab0570475a8b38a9041e614840
         * Lonesome Road:
     * 4fa3d8d9b2cb4714a19a38d1a598be8f
         * Gun Runners' Arsenal:
     * 7dcfb9cd9d134728b2646466c34c7b3b
         * Courier's Stash:
     * ee9a44b4530942499ef1c8c390731fce
         */
    result = parseEpicGamesLocation({"5daeb974a22a435988892319b3a4f476"});
    if (QFileInfo(result).isDir()) {
      QDir startPath = QDir(result);
      auto subDirs   = startPath.entryList({"Fallout New Vegas*"},
                                           QDir::Dirs | QDir::NoDotAndDotDot);
      if (!subDirs.isEmpty())
        result = startPath.absoluteFilePath(subDirs.first());
    }
  }
  return result;
}

QList<ExecutableInfo> GameFalloutNV::executables() const
{
  ExecutableInfo game("New Vegas", findInGameFolder(binaryName()));
  ExecutableInfo launcher("Fallout Launcher", findInGameFolder(getLauncherName()));
  QList<ExecutableInfo> extraExecutables =
      QList<ExecutableInfo>() << ExecutableInfo("Fallout Mod Manager",
                                                findInGameFolder("fomm/fomm.exe"))
                              << ExecutableInfo("BOSS",
                                                findInGameFolder("BOSS/BOSS.exe"))
                              << ExecutableInfo("GECK", findInGameFolder("geck.exe"))
                              << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                                     .withArgument("--game=\"FalloutNV\"");
  if (selectedVariant() != "Epic Games") {
    extraExecutables.prepend(ExecutableInfo(
        "NVSE", findInGameFolder(m_Organizer->gameFeatures()
                                     ->gameFeature<MOBase::ScriptExtender>()
                                     ->loaderName())));
  } else {
    game.withArgument("-EpicPortal");
    launcher.withArgument("-EpicPortal");
  }
  QList<ExecutableInfo> executables = {game, launcher};
  executables += extraExecutables;
  return executables;
}
