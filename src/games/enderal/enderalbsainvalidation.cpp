#include "enderalbsainvalidation.h"

using namespace Qt::Literals::StringLiterals;

EnderalBSAInvalidation::EnderalBSAInvalidation(MOBase::DataArchives* dataArchives,
                                               MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, u"Enderal.ini"_s, game)
{}

QString EnderalBSAInvalidation::invalidationBSAName() const
{
  return u"Enderal - Invalidation.bsa"_s;
}

unsigned long EnderalBSAInvalidation::bsaVersion() const
{
  return 0x68;
}
