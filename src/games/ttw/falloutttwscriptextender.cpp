#include "falloutttwscriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

FalloutTTWScriptExtender::FalloutTTWScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString FalloutTTWScriptExtender::BinaryName() const
{
  return u"nvse_loader.exe"_s;
}

QString FalloutTTWScriptExtender::PluginPath() const
{
  return u"nvse/plugins"_s;
}
