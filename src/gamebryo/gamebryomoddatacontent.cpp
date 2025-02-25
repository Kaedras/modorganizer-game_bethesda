#include "gamebryomoddatacontent.h"

#include <igamefeatures.h>
#include <scriptextender.h>

using namespace Qt::Literals::StringLiterals;

GamebryoModDataContent::GamebryoModDataContent(
    MOBase::IGameFeatures const* gameFeatures)
    : m_GameFeatures(gameFeatures), m_Enabled(CONTENT_MODGROUP + 1, true)
{}

std::vector<GamebryoModDataContent::Content>
GamebryoModDataContent::getAllContents() const
{
  static std::vector<Content> GAMEBRYO_CONTENTS{
      {CONTENT_PLUGIN, QT_TR_NOOP("Plugins (ESP/ESM/ESL)"),
       u":/MO/gui/content/plugin"_s},
      {CONTENT_OPTIONAL, QT_TR_NOOP("Optional Plugins"), "", true},
      {CONTENT_INTERFACE, QT_TR_NOOP("Interface"), u":/MO/gui/content/interface"_s},
      {CONTENT_MESH, QT_TR_NOOP("Meshes"), u":/MO/gui/content/mesh"_s},
      {CONTENT_BSA, QT_TR_NOOP("Bethesda Archive"), u":/MO/gui/content/bsa"_s},
      {CONTENT_SCRIPT, QT_TR_NOOP("Scripts (Papyrus)"), u":/MO/gui/content/script"_s},
      {CONTENT_SKSE, QT_TR_NOOP("Script Extender Plugin"), u":/MO/gui/content/skse"_s},
      {CONTENT_SKSE_FILES, QT_TR_NOOP("Script Extender Files"), "", true},
      {CONTENT_SKYPROC, QT_TR_NOOP("SkyProc Patcher"), u":/MO/gui/content/skyproc"_s},
      {CONTENT_SOUND, QT_TR_NOOP("Sound or Music"), u":/MO/gui/content/sound"_s},
      {CONTENT_TEXTURE, QT_TR_NOOP("Textures"), u":/MO/gui/content/texture"_s},
      {CONTENT_MCM, QT_TR_NOOP("MCM Configuration"), u":/MO/gui/content/menu"_s},
      {CONTENT_INI, QT_TR_NOOP("INI Files"), u":/MO/gui/content/inifile"_s},
      {CONTENT_FACEGEN, QT_TR_NOOP("FaceGen Data"), u":/MO/gui/content/facegen"_s},
      {CONTENT_MODGROUP, QT_TR_NOOP("ModGroup Files"), u":/MO/gui/content/modgroup"_s}};

  // Copy the list of enabled contents:
  std::vector<Content> contents;
  std::copy_if(std::begin(GAMEBRYO_CONTENTS), std::end(GAMEBRYO_CONTENTS),
               std::back_inserter(contents), [this](auto e) {
                 return m_Enabled[e.id()];
               });
  return contents;
}

std::vector<int> GamebryoModDataContent::getContentsFor(
    std::shared_ptr<const MOBase::IFileTree> fileTree) const
{
  std::vector<int> contents;

  for (auto e : *fileTree) {
    if (e->isFile()) {
      auto suffix = e->suffix().toLower();
      if (m_Enabled[CONTENT_PLUGIN] &&
          (suffix == u"esp"_s || suffix == u"esm"_s || suffix == u"esl"_s)) {
        contents.push_back(CONTENT_PLUGIN);
      } else if (m_Enabled[CONTENT_BSA] && (suffix == u"bsa"_s || suffix == u"ba2"_s)) {
        contents.push_back(CONTENT_BSA);
      } else if (m_Enabled[CONTENT_INI] && suffix == u"ini"_s &&
                 e->compare(u"meta.ini"_s) != 0) {
        contents.push_back(CONTENT_INI);
      } else if (m_Enabled[CONTENT_MODGROUP] && suffix == u"modgroups"_s) {
        contents.push_back(CONTENT_MODGROUP);
      }
    } else {
      if (m_Enabled[CONTENT_TEXTURE] &&
          (e->compare(u"textures"_s) == 0 || e->compare(u"icons"_s) == 0 ||
           e->compare(u"bookart"_s) == 0)) {
        contents.push_back(CONTENT_TEXTURE);
      } else if (m_Enabled[CONTENT_MESH] && e->compare(u"meshes"_s) == 0) {
        contents.push_back(CONTENT_MESH);
      } else if (m_Enabled[CONTENT_INTERFACE] &&
                 (e->compare(u"interface"_s) == 0 || e->compare(u"menus"_s) == 0)) {
        contents.push_back(CONTENT_INTERFACE);
      } else if (m_Enabled[CONTENT_SOUND] &&
                 (e->compare(u"music"_s) == 0 || e->compare(u"sound"_s) == 0)) {
        contents.push_back(CONTENT_SOUND);
      } else if (m_Enabled[CONTENT_SCRIPT] && e->compare(u"scripts"_s) == 0) {
        contents.push_back(CONTENT_SCRIPT);
      } else if (m_Enabled[CONTENT_SKYPROC] && e->compare(u"SkyProc Patchers"_s) == 0) {
        contents.push_back(CONTENT_SKYPROC);
      } else if (m_Enabled[CONTENT_MCM] && e->compare(u"MCM"_s) == 0) {
        contents.push_back(CONTENT_MCM);
      } else if (m_Enabled[CONTENT_OPTIONAL] && e->compare(u"Optional"_s) == 0 &&
                 e->astree()->size() > 0) {
        contents.push_back(CONTENT_OPTIONAL);
      }
    }
  }

  if (m_Enabled[CONTENT_FACEGEN]) {
    auto e1 = fileTree->findDirectory(u"meshes/actors/character/facegendata"_s);
    if (e1) {
      contents.push_back(CONTENT_FACEGEN);
    } else {
      auto e2 = fileTree->findDirectory(u"textures/actors/character/facegendata"_s);
      if (e2) {
        contents.push_back(CONTENT_FACEGEN);
      }
    }
  }

  auto extender = m_GameFeatures->gameFeature<MOBase::ScriptExtender>();
  if (extender != nullptr) {
    auto e = fileTree->findDirectory(extender->PluginPath());
    if (e) {
      if (m_Enabled[CONTENT_SKSE_FILES]) {
        contents.push_back(CONTENT_SKSE_FILES);
      }
      if (m_Enabled[CONTENT_SKSE]) {
        for (auto f : *e) {
          if (f->hasSuffix(u"dll"_s)) {
            contents.push_back(CONTENT_SKSE);
            break;
          }
        }
      }
    }
  }

  return contents;
}
