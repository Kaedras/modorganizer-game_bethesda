#include "oblivionbsainvalidation.h"

using namespace Qt::Literals::StringLiterals;

OblivionBSAInvalidation::OblivionBSAInvalidation(MOBase::DataArchives* dataArchives,
                                                 MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, u"oblivion.ini"_s, game)
{}

QString OblivionBSAInvalidation::invalidationBSAName() const
{
  return u"Oblivion - Invalidation.bsa"_s;
}

unsigned long OblivionBSAInvalidation::bsaVersion() const
{
  return 0x67;
}
