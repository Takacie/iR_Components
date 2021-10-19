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
// KnobStartPos enum class
//----------------------------------------------------------------------------------------------------------------------
enum class KnobStartPos
{
  StartLeft, StartCenter, StartRight
};

//----------------------------------------------------------------------------------------------------------------------
// Knob class
//----------------------------------------------------------------------------------------------------------------------
class iR_Knob : public Slider
{
public:
  // constructor
  iR_Knob(APVTS& apvts, const String& parameterID, double midPointValue = 0.5);

  // common method
  void addAndMakeVisibleMyself(AudioProcessorEditor& editor);
  
  // override
  void mouseDoubleClick(const MouseEvent& event) override;
  void mouseDrag(const MouseEvent& event) override;
  void mouseUp(const MouseEvent& event) override;

  // getter
  KnobStartPos getStartPosition() { return start_pos; }

  // setter
  void setPosition(int x, int y, float size_ratio);
  void setShowValue(bool value) { value_label->setAlpha(value); }
  void setStartPosition(KnobStartPos value) { start_pos = value; }

  // static
  static std::unique_ptr<iR_KnobLookAndFeel> lookandfeel;
  static void setKnobColor(const Colour& colour) { if (lookandfeel) lookandfeel->setMainColour(colour); }

private:
  APVTS* apvts;
  String parameter_id;
  std::unique_ptr<KnobAttachment> knob_attachment;
  std::unique_ptr<Label> title_label_ptr = std::make_unique<Label>();
  Label* title_label = title_label_ptr.get();
  Label* value_label;
  KnobStartPos start_pos = KnobStartPos::StartLeft;
};

} // namespace GUI
} // namespace iNVOXRecords