#pragma once

#include "iR_ButtonLookAndFeel.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {
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

  // common method
  void addAndMakeVisibleMyself(AudioProcessorEditor& editor);

  // getter

  // setter
  void setPosition(int x, int y, int width, float size_ratio);

  // static
  static std::unique_ptr<iR_ButtonLookAndFeel> lookandfeel;
  static void setButtonColor(const Colour& colour) { if (lookandfeel) lookandfeel->setMainColour(colour); }

private:

  APVTS* apvts;
  String parameter_id;
  std::unique_ptr<ButtonAttachment> button_attachment;
};

} // namespace GUI
} // namespace iNVOXRecords