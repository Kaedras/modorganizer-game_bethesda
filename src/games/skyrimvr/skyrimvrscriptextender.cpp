#include "skyrimvrscriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

SkyrimVRScriptExtender::SkyrimVRScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString SkyrimVRScriptExtender::BinaryName() const
{
  return u"sksevr_loader.exe"_s;
}

QString SkyrimVRScriptExtender::PluginPath() const
{
  return u"SKSE/plugins"_s;
}
