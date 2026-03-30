#include "../gamefallout4vr.h"

#include <executableinfo.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QList<ExecutableInfo> GameFallout4VR::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("Fallout 4 VR", findInGameFolder(binaryName()))
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Fallout4VR\"");
}

QString GameFallout4VR::identifyGamePath() const
{
  QString path = "Software\\Bethesda Softworks\\" + gameName();
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(),
                        L"Installed Path");
}
