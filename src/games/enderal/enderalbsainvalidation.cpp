#include "enderalbsainvalidation.h"

EnderalBSAInvalidation::EnderalBSAInvalidation(MOBase::DataArchives* dataArchives,
                                               MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, "enderal.ini", game)
{}

QString EnderalBSAInvalidation::invalidationBSAName() const
{
  return "Enderal - Invalidation.bsa";
}

uint32_t EnderalBSAInvalidation::bsaVersion() const
{
  return 0x68;
}
