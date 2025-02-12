#include "oblivionscriptextender.h"

#include <QString>

using namespace Qt::Literals::StringLiterals;

OblivionScriptExtender::OblivionScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

OblivionScriptExtender::~OblivionScriptExtender() {}

QString OblivionScriptExtender::BinaryName() const
{
  return u"obse_loader.exe"_s;
}

QString OblivionScriptExtender::PluginPath() const
{
  return u"OBSE/plugins"_s;
}
