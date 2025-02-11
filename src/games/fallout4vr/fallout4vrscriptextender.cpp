#include "fallout4vrscriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

Fallout4VRScriptExtender::Fallout4VRScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString Fallout4VRScriptExtender::BinaryName() const
{
  return u"f4se_loader.exe"_s;
}

QString Fallout4VRScriptExtender::PluginPath() const
{
  return u"f4se/plugins"_s;
}
