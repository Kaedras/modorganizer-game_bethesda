#ifndef OBLIVION_MODATACHECKER_H
#define OBLIVION_MODATACHECKER_H

#include <gamebryomoddatachecker.h>

using Qt::StringLiterals::operator ""_s;

class OblivionModDataChecker : public GamebryoModDataChecker
{
public:
  using GamebryoModDataChecker::GamebryoModDataChecker;

  CheckReturn
  dataLooksValid(std::shared_ptr<const MOBase::IFileTree> fileTree) const override;
  std::shared_ptr<MOBase::IFileTree>
  fix(std::shared_ptr<MOBase::IFileTree> fileTree) const override;

protected:
  virtual const FileNameSet& possibleFolderNames() const override
  {
    static FileNameSet result{u"fonts"_s,      u"interface"_s,     u"menus"_s,
                              u"meshes"_s,     u"music"_s,         u"scripts"_s,
                              u"shaders"_s,    u"sound"_s,         u"strings"_s,
                              u"textures"_s,   u"trees"_s,         u"video"_s,
                              u"facegen"_s,    u"obse"_s,          u"distantlod"_s,
                              u"asi"_s,        u"distantland"_s,   u"mits"_s,
                              u"dllplugins"_s, u"CalienteTools"_s, u"NetScriptFramework"_s};
    return result;
  }
  virtual const FileNameSet& possibleFileExtensions() const override
  {
    static FileNameSet result{u"esp"_s, u"esm"_s, u"bsa"_s, u"modgroups"_s, u"ini"_s};
    return result;
  }
};

#endif  // OBLIVION_MODATACHECKER_H
