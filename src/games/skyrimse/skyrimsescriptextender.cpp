#include "skyrimsescriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

SkyrimSEScriptExtender::SkyrimSEScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString SkyrimSEScriptExtender::BinaryName() const
{
  return u"skse64_loader.exe"_s;
}

QString SkyrimSEScriptExtender::PluginPath() const
{
  return u"skse/plugins"_s;
}
