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

#include "HeaderFont.h"
#include "../PresetSelector/PresetSelector.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// Header class
//----------------------------------------------------------------------------------------------------------------------
class Header : public juce::Component
{
public:
  // constructor
  Header(juce::AudioProcessorEditor& parent, APVTS& apvts, UserProperties* userProperties) :
    apvts(apvts),
    userProperties(userProperties)
  {
    selector.setWidth(selectorWidth);
    addAndMakeVisible(selector);
    parent.addAndMakeVisible(this);
    parent.addChildComponent(selector.getFileListComponent());
  }

  // override
  void paint(juce::Graphics& g) override
  {
    auto b = getBounds();
    const auto w = getWidth();
    const auto h = getHeight();

    g.fillAll(Colour(5, 5, 10));
    g.setColour(mainColour);

    auto lineW = h * 0.025f;
    g.drawRect(b.withLeft(-lineW).withTop(-lineW).withRight(w + lineW), lineW);

    g.setColour(mainColour);
    g.setFont(getCustomFont().withHeight(h * 0.85f));

    auto textRect = b.reduced(w * 0.02f, 0).withTop(-h * 0.05f);
    g.drawText(title, textRect, Justification::centredLeft);
  }

  // setter
  void setPosition(int init_width, float scale)
  {
    setBounds(0, 0, getParentComponent()->getWidth(), 60 * scale);
    selector.setWidth(selectorWidth * scale);
    selector.setPosition(init_width - selectorWidth - 15, 15, scale);
  }

  void setTitle(const String& newTitle) { title = newTitle; }

  void setMainColour(Colour newMainColour) { mainColour = newMainColour; }

  void setSelectorWidth(int newWidth) { selectorWidth = newWidth; }

  // static
  static const Font getCustomFont()
  {
    static auto typeface = Typeface::createSystemTypefaceFor(HeaderFont::ComfortaaRegular_ttf,
      HeaderFont::ComfortaaRegular_ttfSize);
    return Font(typeface);
  }

private:
  APVTS& apvts;
  UserProperties* userProperties;

  String title = "Header";
  Colour mainColour = Colour(132, 192, 106);
  int selectorWidth = 220;

  PresetSelector selector { apvts, userProperties };
};

} // namespace iNVOXRecords::gui