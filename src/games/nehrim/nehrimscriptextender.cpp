#include "nehrimscriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

NehrimScriptExtender::NehrimScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

NehrimScriptExtender::~NehrimScriptExtender() {}

QString NehrimScriptExtender::BinaryName() const
{
  return u"obse_loader.exe"_s;
}

QString NehrimScriptExtender::PluginPath() const
{
  return u"OBSE/plugins"_s;
}
