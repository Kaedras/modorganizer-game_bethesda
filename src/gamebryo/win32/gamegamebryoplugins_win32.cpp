#include "../gamebryogameplugins.h"

using namespace Qt::StringLiterals;

QString GamebryoGamePlugins::getLoadOrderPath() const
{
  return organizer()->profile()->absolutePath() % "/loadorder.txt"_L1;
}

QString GamebryoGamePlugins::getPluginsPath() const
{
  return organizer()->profile()->absolutePath() % "/plugins.txt"_L1;
}
