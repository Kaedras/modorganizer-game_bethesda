#include "starfieldscriptextender.h"

#include <QString>
#include <QStringList>

using namespace Qt::Literals::StringLiterals;

StarfieldScriptExtender::StarfieldScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString StarfieldScriptExtender::BinaryName() const
{
  return u"sfse_loader.exe"_s;
}

QString StarfieldScriptExtender::PluginPath() const
{
  return u"sfse/plugins"_s;
}
