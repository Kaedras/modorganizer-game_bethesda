#ifndef MORROWINDBSAINVALIDATION_H
#define MORROWINDBSAINVALIDATION_H

#include "gamebryobsainvalidation.h"
#include "morrowinddataarchives.h"

#include <memory>

class MorrowindBSAInvalidation : public GamebryoBSAInvalidation
{
public:
  MorrowindBSAInvalidation(MOBase::DataArchives* dataArchives,
                           MOBase::IPluginGame const* game);

private:
  virtual QString invalidationBSAName() const override;
  virtual uint32_t bsaVersion() const override;
};

#endif  // MORROWINDBSAINVALIDATION_H
