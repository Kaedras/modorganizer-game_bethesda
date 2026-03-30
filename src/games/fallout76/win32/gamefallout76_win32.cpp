#include "../gamefallout76.h"

#include <executableinfo.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QList<ExecutableInfo> GameFallout76::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("Fallout 76", findInGameFolder(binaryName()));
}
