#include "fo4londonscriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

Fallout4LondonScriptExtender::Fallout4LondonScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString Fallout4LondonScriptExtender::BinaryName() const
{
  return u"f4se_loader.exe"_s;
}

QString Fallout4LondonScriptExtender::PluginPath() const
{
  return u"f4se/plugins"_s;
}
