#include "../gamefallout4vr.h"

#include <executableinfo.h>
#include <steamutility.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QList<ExecutableInfo> GameFallout4VR::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("Fallout 4 VR", findInGameFolder(binaryName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Fallout4VR\"");
}

QString GameFallout4VR::identifyGamePath() const
{
  return findSteamGame(gameName(), "Data/Fallout4.esm");
}
