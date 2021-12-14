#pragma once

#include "iR_ButtonLookAndFeel.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// Button class
//----------------------------------------------------------------------------------------------------------------------
using APVTS = AudioProcessorValueTreeState;
using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;
class iR_Button : public ToggleButton
{
public:
  // constructor
  iR_Button(APVTS& apvts, const String& parameterID);

  // override
  void paint(Graphics& g) override
  {
    Button::paint(g);
    setAlpha(isEnabled() ? 1.0f : 0.5f);
  }

  // getter

  // setter
  void setPosition(int x, int y, int width, float size_ratio);

  // static
  static std::unique_ptr<iR_ButtonLookAndFeel> lookandfeel;
  static void setButtonColor(const Colour& colour) { if (lookandfeel) lookandfeel->setMainColour(colour); }

private:
  APVTS* apvts;
  String parameter_id;
  ButtonAttachment button_attachment;
};

} // namespace iNVOXRecords::gui