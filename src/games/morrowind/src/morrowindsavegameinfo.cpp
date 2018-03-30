#include "morrowindsavegameinfo.h"
#include "morrowindsavegameinfowidget.h"
#include "morrowindsavegame.h"
#include "gamegamebryo.h"

MorrowindSaveGameInfo::MorrowindSaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
  m_Game = dynamic_cast<GameMorrowind const *>(game);
}

MorrowindSaveGameInfo::~MorrowindSaveGameInfo()
{
}


MOBase::ISaveGame const *MorrowindSaveGameInfo::getSaveGameInfo(QString const &file) const
{
  return new MorrowindSaveGame(file, m_Game);
}

MOBase::ISaveGameInfoWidget *MorrowindSaveGameInfo::getSaveGameWidget(QWidget *parent) const
{
  return new MorrowindSaveGameInfoWidget(this, parent);
}
