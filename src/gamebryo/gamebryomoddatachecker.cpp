#include <ifiletree.h>

#include "gamebryomoddatachecker.h"

using namespace Qt::Literals::StringLiterals;

/**
 * @return the list of possible folder names in data.
 */
auto GamebryoModDataChecker::possibleFolderNames() const -> const FileNameSet&
{
  static FileNameSet result{u"fonts"_s,
                            u"interface"_s,
                            u"menus"_s,
                            u"meshes"_s,
                            u"music"_s,
                            u"scripts"_s,
                            u"shaders"_s,
                            u"sound"_s,
                            u"strings"_s,
                            u"textures"_s,
                            u"trees"_s,
                            u"video"_s,
                            u"facegen"_s,
                            u"materials"_s,
                            u"skse"_s,
                            u"obse"_s,
                            u"mwse"_s,
                            u"nvse"_s,
                            u"fose"_s,
                            u"f4se"_s,
                            u"distantlod"_s,
                            u"asi"_s,
                            u"SkyProc Patchers"_s,
                            u"Tools"_s,
                            u"MCM"_s,
                            u"icons"_s,
                            u"bookart"_s,
                            u"distantland"_s,
                            u"mits"_s,
                            u"splash"_s,
                            u"dllplugins"_s,
                            u"CalienteTools"_s,
                            u"NetScriptFramework"_s,
                            u"shadersfx"_s};
  return result;
}

/**
 * @return the extensions of possible files in data.
 */
auto GamebryoModDataChecker::possibleFileExtensions() const -> const FileNameSet&
{
  static FileNameSet result{u"esp"_s, u"esm"_s, u"esl"_s, u"bsa"_s, u"ba2"_s, u"modgroups"_s, u"ini"_s};
  return result;
}

GamebryoModDataChecker::GamebryoModDataChecker(const GameGamebryo* game) : m_Game(game)
{}

GamebryoModDataChecker::CheckReturn GamebryoModDataChecker::dataLooksValid(
    std::shared_ptr<const MOBase::IFileTree> fileTree) const
{
  auto& folders  = possibleFolderNames();
  auto& suffixes = possibleFileExtensions();
  for (auto entry : *fileTree) {
    if (entry->isDir()) {
      if (folders.count(entry->name()) > 0) {
        return CheckReturn::VALID;
      }
    } else {
      if (suffixes.count(entry->suffix()) > 0) {
        return CheckReturn::VALID;
      }
    }
  }
  return CheckReturn::INVALID;
}
