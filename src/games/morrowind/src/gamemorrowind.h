#ifndef GAMEMORROWIND_H
#define GAMEMORROWIND_H

#include "gamegamebryo.h"

#include <QObject>
#include <QtGlobal>

class GameMorrowind : public GameGamebryo
{
  Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID "org.schilduin.GameMorrowind" FILE "gamemorrowind.json")
#endif

public:

  GameMorrowind();

  virtual bool init(MOBase::IOrganizer *moInfo) override;

public: // IPluginGame interface

  virtual QString gameName() const override;
  virtual QDir dataDirectory() const override;
  virtual QDir documentsDirectory() const override;
  virtual QDir savesDirectory() const override;
  virtual QList<MOBase::ExecutableInfo> executables() const override;
  virtual void initializeProfile(const QDir &path, ProfileSettings settings) const override;
  virtual QString savegameExtension() const override;
  virtual QString savegameSEExtension() const override;
  virtual QString steamAPPId() const override;
  virtual QStringList primaryPlugins() const override;
  virtual QString gameShortName() const override;
  virtual QString getLauncherName() const override;
  virtual QString gameNexusName() const override;
  virtual QStringList iniFiles() const override;
  virtual QStringList DLCPlugins() const override;
  virtual LoadOrderMechanism loadOrderMechanism() const override;
  virtual int nexusModOrganizerID() const override;
  virtual int nexusGameID() const override;

public: // IPlugin interface

  virtual QString name() const;
  virtual QString author() const;
  virtual QString description() const;
  virtual MOBase::VersionInfo version() const;
  virtual bool isActive() const;
  virtual QList<MOBase::PluginSetting> settings() const;
};

#endif // GAMEMORROWIND_H
