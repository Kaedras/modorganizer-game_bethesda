#include "fallout3bsainvalidation.h"

using namespace Qt::Literals::StringLiterals;

Fallout3BSAInvalidation::Fallout3BSAInvalidation(MOBase::DataArchives* dataArchives,
                                                 MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, u"fallout.ini"_s, game)
{}

QString Fallout3BSAInvalidation::invalidationBSAName() const
{
  return u"Fallout - Invalidation.bsa"_s;
}

unsigned long Fallout3BSAInvalidation::bsaVersion() const
{
  return 0x68;
}
