#include "../gamebryogameplugins.h"
#include "util.h"

using namespace Qt::StringLiterals;

QString GamebryoGamePlugins::getLoadOrderPath() const
{
  return findFileCaseInsensitive(organizer()->profile()->absolutePath() %
                                 "/loadorder.txt"_L1);
}

QString GamebryoGamePlugins::getPluginsPath() const
{
  return findFileCaseInsensitive(organizer()->profile()->absolutePath() %
                                 "/plugins.txt"_L1);
}
