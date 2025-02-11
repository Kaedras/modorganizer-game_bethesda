#include "nehrimbsainvalidation.h"

using namespace Qt::Literals::StringLiterals;

NehrimBSAInvalidation::NehrimBSAInvalidation(MOBase::DataArchives* dataArchives,
                                             MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, u"oblivion.ini"_s, game)
{}

QString NehrimBSAInvalidation::invalidationBSAName() const
{
  return u"Nehrim - Invalidation.bsa"_s;
}

unsigned long NehrimBSAInvalidation::bsaVersion() const
{
  return 0x67;
}
