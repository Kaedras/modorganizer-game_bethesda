#include "falloutnvscriptextender.h"

#include <QString>
#include <QStringList>

using namespace Qt::Literals::StringLiterals;

FalloutNVScriptExtender::FalloutNVScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString FalloutNVScriptExtender::BinaryName() const
{
  return u"nvse_loader.exe"_s;
}

QString FalloutNVScriptExtender::PluginPath() const
{
  return u"nvse/plugins"_s;
}
