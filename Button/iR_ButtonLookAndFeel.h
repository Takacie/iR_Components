#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_ButtonLookAndFeel class
//----------------------------------------------------------------------------------------------------------------------
class iR_ButtonLookAndFeel : public LookAndFeel_V4
{
public:
  // constructor
  iR_ButtonLookAndFeel(const juce::Colour& main_colour);

  // override
  void drawToggleButton(Graphics& g, ToggleButton& button,
                        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
  void drawTickBox(Graphics& g, Component& component, float x, float y, float w, float h, 
                   const bool ticked, const bool isEnabled, const bool shouldDrawButtonAsHighlighted,
                   const bool shouldDrawButtonAsDown) override;

  // setter
  void setMainColour(const Colour& mainColour) { main_colour = mainColour; }

private:
  Colour main_colour;
};

} // namespace GUI
} // namespace iNVOXRecords