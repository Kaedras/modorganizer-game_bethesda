#include "enderalsescriptextender.h"

#include <QString>
#include <QStringList>

using namespace Qt::Literals::StringLiterals;

EnderalSEScriptExtender::EnderalSEScriptExtender(GameGamebryo const* game)
    : GamebryoScriptExtender(game)
{}

QString EnderalSEScriptExtender::BinaryName() const
{
  return u"skse64_loader.exe"_s;
}

QString EnderalSEScriptExtender::PluginPath() const
{
  return u"SKSE/plugins"_s;
}
