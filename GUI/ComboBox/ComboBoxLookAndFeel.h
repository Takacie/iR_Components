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
// ButtonLookAndFeel class
//----------------------------------------------------------------------------------------------------------------------
class ComboBoxLookAndFeel : public juce::LookAndFeel_V4
{
public:
  // constructor
  ComboBoxLookAndFeel(const juce::Colour& main_colour);

  // override
  void drawComboBox(Graphics& g, int width, int height, bool,
    int, int, int, int, juce::ComboBox& box) override
  {
    const auto round = width * 0.075f;
    const auto frameW = width * 0.03f;
    const auto w = width * 0.9f;
    const auto h = height * 0.6f;
    const auto x = static_cast<float>(width - w) / 2;
    const auto y = height * 0.15f;
    const auto arrowW = width * 0.2f;
    const auto arrowH = height * 0.1f;
    const auto arrowX = static_cast<float>(width - arrowW) / 2;
    const auto arrowY = height * 0.6f;
    const auto offsetX = arrowW * 0.15f;
    const auto offsetY = arrowW * 0.1f;
    const auto arrow_lineW = offsetY;
    Rectangle<int> boxBounds(x, y, w, h);

    const auto colour = Colour(64, 64, 64);
    g.setColour(colour);
    g.fillRoundedRectangle(boxBounds.toFloat(), round);

    g.setColour(colour.darker());
    g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), round, frameW);

    Rectangle<int> arrowZone(arrowX, arrowY, arrowW, arrowH);
    const auto centreY = static_cast<float>(arrowZone.getCentreY());
    Path path;
    path.startNewSubPath(arrowX + offsetX, centreY - offsetY);
    path.lineTo(arrowX + arrowW / 2, centreY + offsetX);
    path.lineTo(arrowX + arrowW - offsetX, centreY - offsetY);

    g.setColour(mainColour);
    g.strokePath(path, PathStrokeType(arrow_lineW));
  }

  void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override
  {
    const auto w = box.getWidth();
    const auto h = box.getHeight();
    const auto fontH = h * 0.18f;
    label.setBounds(0, h * 0.325f, w, fontH);
    label.setJustificationType(Justification::centred);

    label.setFont(fontH);
  }

  void drawPopupMenuItem(Graphics& g, const Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted,
    bool isTicked, bool hasSubMenu, const String& text, const String& shortcutKeyText, const Drawable* icon,
    const Colour* textColourToUse) override
  {
    auto textColour = Colours::white;

    auto r = area.reduced(1);

    if (isHighlighted && isActive) {
      g.setColour(Colour(70, 70, 75));
      g.fillRect(r);
    }

    g.setColour(Colours::white);
    g.setFont(r.getHeight() * 0.7f);

    r.removeFromLeft(5);
    g.drawFittedText(text, r, Justification::centredLeft, 1);
  }

  void drawPopupMenuBackground(Graphics& g, int width, int height) override
  {
    g.fillAll(Colour(50, 50, 55));
    Rectangle<int> menuRect { 0, 0, width, height };
    g.setColour(Colour(200, 200, 205));
    g.drawRect(menuRect);
  }

  // setter
  void setMainColour(const Colour& colour) { mainColour = colour; }

private:
  Colour mainColour;
};

} // namespace iNVOXRecords::gui