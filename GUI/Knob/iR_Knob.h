#pragma once

#include "iR_KnobLookAndFeel.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {

using APVTS = AudioProcessorValueTreeState;
using KnobAttachment = AudioProcessorValueTreeState::SliderAttachment;

//----------------------------------------------------------------------------------------------------------------------
// Knob class
//----------------------------------------------------------------------------------------------------------------------
class iR_Knob : public Slider
{
public:
  // enums
  enum KnobStartPos { StartLeft, StartCenter, StartRight, StartPosNone };

  // constructor
  iR_Knob(APVTS& apvts, const String& parameterID);
  
  // override
  void paint(Graphics& g) override;
  void mouseDoubleClick(const MouseEvent& event) override;
  void mouseDrag(const MouseEvent& event) override;
  void mouseUp(const MouseEvent& event) override;

  // getter
  KnobStartPos getStartPosition() { return start_pos; }
  Label* getTitleLabel() { return &title_label; }

  // setter
  void setPosition(int x, int y, float size_ratio);
  void setShowValue(bool value) { if (value_label) value_label->setAlpha(value); }
  void setStartPosition(KnobStartPos value) { start_pos = value; }
  double test(String a) { return 1.0f; };

  // static
  static std::unique_ptr<iR_KnobLookAndFeel> lookandfeel;
  static void setKnobColor(const Colour& colour) { if (lookandfeel) lookandfeel->setMainColour(colour); }
  static bool alwaysShowValue;

private:
  APVTS* apvts;
  String parameter_id;
  KnobAttachment knob_attachment;
  Label title_label;
  Label* value_label;
  KnobStartPos start_pos = StartLeft;
};

} // namespace GUI
} // namespace iNVOXRecords