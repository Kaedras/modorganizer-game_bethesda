#ifndef FO4LONDON_MODDATACONTENT_H
#define FO4LONDON_MODDATACONTENT_H

#include <gamebryomoddatacontent.h>
#include <ifiletree.h>

using Qt::StringLiterals::operator ""_s;

class Fallout4LondonModDataContent : public GamebryoModDataContent
{
protected:
  enum Fallout4LondonContent
  {
    CONTENT_MATERIAL = CONTENT_NEXT_VALUE
  };

public:
  Fallout4LondonModDataContent(const MOBase::IGameFeatures* gameFeatures)
      : GamebryoModDataContent(gameFeatures)
  {
    m_Enabled[CONTENT_SKYPROC] = false;
  }

  std::vector<Content> getAllContents() const override
  {
    auto contents = GamebryoModDataContent::getAllContents();
    contents.push_back(
        Content(CONTENT_MATERIAL, u"Materials"_s, u":/MO/gui/content/material"_s));
    return contents;
  }

  std::vector<int>
  getContentsFor(std::shared_ptr<const MOBase::IFileTree> fileTree) const override
  {
    auto contents = GamebryoModDataContent::getContentsFor(fileTree);
    for (auto e : *fileTree) {
      if (e->compare(u"materials"_s) == 0) {
        contents.push_back(CONTENT_MATERIAL);
        break;  // Early break if you have nothing else to check.
      }
    }
    return contents;
  }
};

#endif  // FO4LONDON_MODDATACONTENT_H
