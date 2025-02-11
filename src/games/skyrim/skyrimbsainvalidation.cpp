#include "skyrimbsainvalidation.h"

using namespace Qt::Literals::StringLiterals;

SkyrimBSAInvalidation::SkyrimBSAInvalidation(MOBase::DataArchives* dataArchives,
                                             MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, u"skyrim.ini"_s, game)
{}

QString SkyrimBSAInvalidation::invalidationBSAName() const
{
  return u"Skyrim - Invalidation.bsa"_s;
}

unsigned long SkyrimBSAInvalidation::bsaVersion() const
{
  return 0x68;
}
