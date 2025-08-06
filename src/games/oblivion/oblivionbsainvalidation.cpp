#include "oblivionbsainvalidation.h"

OblivionBSAInvalidation::OblivionBSAInvalidation(MOBase::DataArchives* dataArchives,
                                                 MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, "oblivion.ini", game)
{}

QString OblivionBSAInvalidation::invalidationBSAName() const
{
  return "Oblivion - Invalidation.bsa";
}

uint32_t OblivionBSAInvalidation::bsaVersion() const
{
  return 0x67;
}
