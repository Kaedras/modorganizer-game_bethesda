#include "morrowindbsainvalidation.h"

MorrowindBSAInvalidation::MorrowindBSAInvalidation(MOBase::DataArchives* dataArchives,
                                                   MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, "morrowind.ini", game)
{}

QString MorrowindBSAInvalidation::invalidationBSAName() const
{
  return "Morrowind - Invalidation.bsa";
}

uint32_t MorrowindBSAInvalidation::bsaVersion() const
{
  return 0x68;
}
