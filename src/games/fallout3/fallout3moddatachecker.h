#ifndef FALLOUT3_MODATACHECKER_H
#define FALLOUT3_MODATACHECKER_H

#include <gamebryomoddatachecker.h>

using Qt::StringLiterals::operator ""_s;

class Fallout3ModDataChecker : public GamebryoModDataChecker
{
public:
  using GamebryoModDataChecker::GamebryoModDataChecker;

protected:
  virtual const FileNameSet& possibleFolderNames() const override
  {
    static FileNameSet result{
        u"fonts"_s,    u"interface"_s,  u"menus"_s,         u"meshes"_s,
        u"music"_s,    u"scripts"_s,    u"shaders"_s,       u"sound"_s,
        u"strings"_s,  u"textures"_s,   u"trees"_s,         u"video"_s,
        u"facegen"_s,  u"materials"_s,  u"fose"_s,          u"distantlod"_s,
        u"asi"_s,      u"Tools"_s,      u"MCM"_s,           u"distantland"_s,
        u"mits"_s,     u"dllplugins"_s, u"CalienteTools"_s, u"NetScriptFramework"_s,
        u"shadersfx"_s};
    return result;
  }
  virtual const FileNameSet& possibleFileExtensions() const override
  {
    static FileNameSet result{u"esp"_s, u"esm"_s, u"bsa"_s, u"modgroups"_s, u"ini"_s};
    return result;
  }
};

#endif  // FALLOUT3_MODATACHECKER_H
