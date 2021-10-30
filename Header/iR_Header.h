#pragma once

#include "HeaderFont.h"
#include "../PresetSelector/iR_PresetSelector.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_Header class
//----------------------------------------------------------------------------------------------------------------------
class iR_Header : public Component
{
public:
  // constructor
  iR_Header(juce::AudioProcessorEditor& parent, APVTS* apvts, iR_UserProperties* userProperties) :
    apvts(apvts),
    user_properties(userProperties)
  {
    addAndMakeVisible(selector);
    parent.addAndMakeVisible(this);
    parent.addChildComponent(selector.getFileListComponent());
  }

  // override
  void paint(juce::Graphics& g) override
  {
    const int w = getWidth();
    const int h = getHeight();
    Rectangle<int> b = getBounds();
    Rectangle<int> text_rect = b.reduced(w * 0.025f, 0);

    g.fillAll(Colour(5, 5, 10));
    g.setColour(main_colour);
    
    int line_w = h * 0.025f;
    g.drawRect(b.withLeft(-line_w).withTop(-line_w).withRight(w + line_w), line_w);

    g.setColour(main_colour);
    g.setFont(getCustomFont().withHeight(h * 0.8f));

    g.drawText("Convolver", text_rect, Justification::centredLeft);
  }

  void resized() override
  {

  }

  // setter
  void setPosition(int init_width, float scale)
  {
    setBounds(0, 0, getParentComponent()->getWidth(), 60 * scale);
    selector.setPosition(init_width - 250 - 15, 15, scale);
  }

  void setMainColour(Colour newMainColour) { main_colour = newMainColour; }

  // static
  static const Font getCustomFont()
  {
    static auto typeface = Typeface::createSystemTypefaceFor(HeaderFont::ComfortaaRegular_ttf, HeaderFont::ComfortaaRegular_ttfSize);
    return Font(typeface);
  }

private:
  APVTS* apvts;
  iR_UserProperties* user_properties;
  Colour main_colour = Colour(132, 192, 106);

  iR_PresetSelector selector = iR_PresetSelector(apvts, user_properties);
};

}
}