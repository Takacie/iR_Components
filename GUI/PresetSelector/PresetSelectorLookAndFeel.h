/*///-------------------------------------------------------------------------------------------------------------------
Copyright(c) 2021 iNVOXRecords

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
/*///-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// FileBrowserLaF class
//----------------------------------------------------------------------------------------------------------------------
class PresetSelectorLaF : public juce::LookAndFeel_V4
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
    Rectangle<float> menu_rect(0, 0, width, height);
    g.setColour(Colour(200, 200, 205));
    g.drawRect(menu_rect);
  }
};

} // namespace iNVOXRecords::gui