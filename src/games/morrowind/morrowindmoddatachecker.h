#ifndef MORROWIND_MODATACHECKER_H
#define MORROWIND_MODATACHECKER_H

#include <gamebryomoddatachecker.h>

using Qt::StringLiterals::operator ""_s;

class MorrowindModDataChecker : public GamebryoModDataChecker
{
public:
  using GamebryoModDataChecker::GamebryoModDataChecker;

protected:
  virtual const FileNameSet& possibleFolderNames() const override
  {
    static FileNameSet result{u"fonts"_s,    u"meshes"_s,  u"music"_s, u"shaders"_s,     u"sound"_s,
                              u"textures"_s, u"video"_s,   u"mwse"_s,  u"distantland"_s, u"mits"_s,
                              u"icons"_s,    u"bookart"_s, u"splash"_s};
    return result;
  }
  virtual const FileNameSet& possibleFileExtensions() const override
  {
    static FileNameSet result{u"esp"_s, u"esm"_s, u"bsa"_s, u"modgroups"_s};
    return result;
  }
};

#endif  // MORROWIND_MODATACHECKER_H
