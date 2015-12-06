#ifndef SKYRIMSAVEGAMEINFO_H
#define SKYRIMSAVEGAMEINFO_H

#include "savegameinfo.h"

class SkyrimSaveGameInfo : public SaveGameInfo
{
public:
  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // SKYRIMSAVEGAMEINFO_H
