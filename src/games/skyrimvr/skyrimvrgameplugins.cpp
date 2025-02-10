#include "skyrimvrgameplugins.h"

using namespace MOBase;

SkyrimVRGamePlugins::SkyrimVRGamePlugins(MOBase::IOrganizer* organizer)
    : CreationGamePlugins(organizer)
{}

bool SkyrimVRGamePlugins::lightPluginsAreSupported()
{
  auto files = m_Organizer->findFiles("SKSE/plugins", {"skyrimvresl.dll"});
  if (files.isEmpty())
    return false;
  return true;
}
