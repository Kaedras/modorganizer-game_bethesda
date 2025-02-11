#include "fallout3scriptextender.h"

#include <QString>
#include <QStringList>

using namespace Qt::Literals::StringLiterals;

Fallout3ScriptExtender::Fallout3ScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString Fallout3ScriptExtender::BinaryName() const
{
  return u"fose_loader.exe"_s;
}

QString Fallout3ScriptExtender::PluginPath() const
{
  return u"fose/plugins"_s;
}
