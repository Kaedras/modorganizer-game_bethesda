#include "skyrimscriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

SkyrimScriptExtender::SkyrimScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString SkyrimScriptExtender::BinaryName() const
{
  return u"skse_loader.exe"_s;
}

QString SkyrimScriptExtender::PluginPath() const
{
  return u"skse/plugins"_s;
}
