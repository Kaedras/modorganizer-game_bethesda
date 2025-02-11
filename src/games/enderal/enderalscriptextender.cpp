#include "enderalscriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

EnderalScriptExtender::EnderalScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString EnderalScriptExtender::BinaryName() const
{
  return u"skse_loader.exe"_s;
}

QString EnderalScriptExtender::PluginPath() const
{
  return u"SKSE/plugins"_s;
}
