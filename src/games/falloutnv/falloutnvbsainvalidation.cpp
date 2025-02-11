#include "falloutnvbsainvalidation.h"

using namespace Qt::Literals::StringLiterals;

FalloutNVBSAInvalidation::FalloutNVBSAInvalidation(MOBase::DataArchives* dataArchives,
                                                   MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, u"fallout.ini"_s, game)
{}

QString FalloutNVBSAInvalidation::invalidationBSAName() const
{
  return u"Fallout - Invalidation.bsa"_s;
}

unsigned long FalloutNVBSAInvalidation::bsaVersion() const
{
  return 0x68;
}
