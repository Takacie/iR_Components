#pragma once

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
  iR_Button(APVTS& apvts, const String& parameterID, LookAndFeel* look_and_feel, double midPointValue = 0.5);

  // common method
  void addAndMakeVisibleMyself(AudioProcessorEditor& editor);

  // getter

  // setter
  void setPosition(int x, int y, int width, float size_ratio);

private:
  APVTS* apvts;
  String parameter_id;
  std::unique_ptr<ButtonAttachment> button_attachment;
};

} // namespace GUI
} // namespace iNVOXRecords