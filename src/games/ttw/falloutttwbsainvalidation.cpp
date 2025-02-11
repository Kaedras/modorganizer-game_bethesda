#include "falloutttwbsainvalidation.h"

using namespace Qt::Literals::StringLiterals;

FalloutTTWBSAInvalidation::FalloutTTWBSAInvalidation(MOBase::DataArchives* dataArchives,
                                                     MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, u"fallout.ini"_s, game)
{}

QString FalloutTTWBSAInvalidation::invalidationBSAName() const
{
  return u"Fallout - Invalidation.bsa"_s;
}

unsigned long FalloutTTWBSAInvalidation::bsaVersion() const
{
  return 0x68;
}
