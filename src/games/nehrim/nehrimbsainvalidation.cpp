#include "nehrimbsainvalidation.h"

NehrimBSAInvalidation::NehrimBSAInvalidation(MOBase::DataArchives* dataArchives,
                                             MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, "oblivion.ini", game)
{}

QString NehrimBSAInvalidation::invalidationBSAName() const
{
  return "Nehrim - Invalidation.bsa";
}

uint32_t NehrimBSAInvalidation::bsaVersion() const
{
  return 0x67;
}
