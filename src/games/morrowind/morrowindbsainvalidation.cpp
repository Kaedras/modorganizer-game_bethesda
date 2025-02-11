#include "morrowindbsainvalidation.h"

using namespace Qt::Literals::StringLiterals;

MorrowindBSAInvalidation::MorrowindBSAInvalidation(MOBase::DataArchives* dataArchives,
                                                   MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, u"morrowind.ini"_s, game)
{}

QString MorrowindBSAInvalidation::invalidationBSAName() const
{
  return u"Morrowind - Invalidation.bsa"_s;
}

unsigned long MorrowindBSAInvalidation::bsaVersion() const
{
  return 0x68;
}
