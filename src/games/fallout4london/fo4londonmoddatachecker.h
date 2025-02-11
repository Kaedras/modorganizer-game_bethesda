#ifndef FO4LONDON_MODATACHECKER_H
#define FO4LONDON_MODATACHECKER_H

#include <gamebryomoddatachecker.h>

using Qt::StringLiterals::operator ""_s;

class Fallout4LondonModDataChecker : public GamebryoModDataChecker
{
public:
  using GamebryoModDataChecker::GamebryoModDataChecker;

protected:
  virtual const FileNameSet& possibleFolderNames() const override
  {
    static FileNameSet result{u"interface"_s,     u"meshes"_s,    u"music"_s,
                              u"scripts"_s,       u"sound"_s,     u"strings"_s,
                              u"textures"_s,      u"trees"_s,     u"video"_s,
                              u"materials"_s,     u"f4se"_s,      u"distantlod"_s,
                              u"asi"_s,           u"Tools"_s,     u"MCM"_s,
                              u"distantland"_s,   u"mits"_s,      u"dllplugins"_s,
                              u"CalienteTools"_s, u"shadersfx"_s, u"aaf"_s};
    return result;
  }
  virtual const FileNameSet& possibleFileExtensions() const override
  {
    static FileNameSet result{u"esp"_s,       u"esm"_s, u"esl"_s, u"ba2"_s,
                              u"modgroups"_s, u"ini"_s, u"csg"_s, u"cdx"_s};
    return result;
  }
};

#endif  // FO4LONDON_MODATACHECKER_H
