#include "fallout4scriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

Fallout4ScriptExtender::Fallout4ScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString Fallout4ScriptExtender::BinaryName() const
{
  return u"f4se_loader.exe"_s;
}

QString Fallout4ScriptExtender::PluginPath() const
{
  return u"f4se/plugins"_s;
}
