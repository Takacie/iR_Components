#pragma once

#include "HeaderFont.h"
#include "../PresetSelector/iR_PresetSelector.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
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
    selector.setWidth(selector_width);
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
    g.setColour(main_colour);
    
    auto line_w = h * 0.025f;
    g.drawRect(b.withLeft(-line_w).withTop(-line_w).withRight(w + line_w), line_w);

    g.setColour(main_colour);
    g.setFont(getCustomFont().withHeight(h * 0.85f));

    auto text_rect = b.reduced(w * 0.02f, 0).withTop(-h * 0.05f);
    g.drawText(title, text_rect, Justification::centredLeft);
  }

  // setter
  void setPosition(int init_width, float scale)
  {
    setBounds(0, 0, getParentComponent()->getWidth(), 60 * scale);
    selector.setWidth(selector_width * scale);
    selector.setPosition(init_width - selector_width - 15, 15, scale);
  }

  void setTitle(const String& newTitle) { title = newTitle; }

  void setMainColour(Colour newMainColour) { main_colour = newMainColour; }

  void setSelectorWidth(int newWidth) { selector_width = newWidth; }

  // static
  static const Font getCustomFont()
  {
    static auto typeface = Typeface::createSystemTypefaceFor(HeaderFont::ComfortaaRegular_ttf, 
                                                             HeaderFont::ComfortaaRegular_ttfSize);
    return Font(typeface);
  }

private:
  APVTS* apvts;
  iR_UserProperties* user_properties;

  String title = "Header";
  Colour main_colour = Colour(132, 192, 106);
  int selector_width = 220;

  iR_PresetSelector selector = iR_PresetSelector(apvts, user_properties);
};

} // namespace iNVOXRecords::gui