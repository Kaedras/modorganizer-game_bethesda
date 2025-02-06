#include "skyrimsescriptextender.h"

#include <QString>

SkyrimSEScriptExtender::SkyrimSEScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString SkyrimSEScriptExtender::BinaryName() const
{
  return "skse64_loader.exe";
}

QString SkyrimSEScriptExtender::PluginPath() const
{
  return "skse/plugins";
}
