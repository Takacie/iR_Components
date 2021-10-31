#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_FileBrowserLaF class
//----------------------------------------------------------------------------------------------------------------------
class iR_PresetSelectorLaF : public LookAndFeel_V4
{
public:
  // override
  void drawFileBrowserRow(Graphics& g, int width, int height, const File& file, const String& filename, Image* icon,
    const String& fileSizeDescription, const String& fileTimeDescription, bool isDirectory,
    bool isItemSelected, int itemIndex, DirectoryContentsDisplayComponent& dcc) override
  {
    if (isItemSelected) {
      g.fillAll(Colour(70, 70, 75));
    }

    const auto scale_basis = height;
    const auto x = scale_basis * 0.25f;
    g.setColour(Colours::white);
    g.setFont(scale_basis * 0.7f);

    const auto text = filename.replace(".irps", "");
    g.drawFittedText(text, x, 0, width - x, height, Justification::centredLeft, 1);
  }

  void drawPopupMenuItem(Graphics& g, const Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted,
    bool isTicked, bool hasSubMenu, const String& text, const String& shortcutKeyText, const Drawable* icon,
    const Colour* textColourToUse) override
  {
    auto textColour = Colours::white;

    auto r = area.reduced(1);

    if (isHighlighted && isActive)
    {
      g.setColour(Colour(70, 70, 75));
      g.fillRect(r);
    }

    g.setColour(Colours::white);
    g.setFont(r.getHeight() * 0.8f);

    r.removeFromLeft(5);
    g.drawFittedText(text, r, Justification::centredLeft, 1);
  }

  void drawPopupMenuBackground(Graphics& g, int width, int height) override
  {
    g.fillAll(Colour(50, 50, 55));
  }
};

}
}