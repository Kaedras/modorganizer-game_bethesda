#include "skyrimvrgameplugins.h"

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

SkyrimVRGamePlugins::SkyrimVRGamePlugins(MOBase::IOrganizer* organizer)
    : CreationGamePlugins(organizer)
{}

bool SkyrimVRGamePlugins::lightPluginsAreSupported()
{
  auto files = m_Organizer->findFiles(u"SKSE/plugins"_s, {u"skyrimvresl.dll"_s});
  if (files.isEmpty())
    return false;
  return true;
}
