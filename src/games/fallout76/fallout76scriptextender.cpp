#include "fallout76scriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

Fallout76ScriptExtender::Fallout76ScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString Fallout76ScriptExtender::BinaryName() const
{
  return u"f76se_loader.exe"_s;
}

QString Fallout76ScriptExtender::PluginPath() const
{
  return u"f76se/plugins"_s;
}
