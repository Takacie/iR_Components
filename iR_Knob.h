#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {
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
using APVTS = AudioProcessorValueTreeState;
using KnobAttachment = AudioProcessorValueTreeState::SliderAttachment;
class Knob : public Slider
{
public:
  Knob(APVTS& apvts, String parameterID, double midPointValue = 0.5);

  void addAndMakeVisibleMyself(AudioProcessorEditor& editor);
  
  // override
  void mouseDoubleClick(const MouseEvent& event) override;
  void mouseDrag(const MouseEvent& event) override;
  void mouseUp(const MouseEvent& event) override;

  // getter
  KnobStartPos getStartPosition() { return start_pos; }

  // setter
  void setPosition(int x, int y, float size_ratio);
  void setLabelFont(const Font& newFont) { title_label->setFont(newFont); }
  void setShowValue(bool value) { value_label->setAlpha(value); }
  void setStartPosition(KnobStartPos value) { start_pos = value; }

private:
  APVTS* apvts;
  juce::String parameter_id;
  std::unique_ptr<KnobAttachment> knob_attachment;
  std::unique_ptr<Label> title_label_ptr = std::make_unique<Label>();
  Label* title_label = title_label_ptr.get();
  Label* value_label;
  KnobStartPos start_pos = KnobStartPos::StartLeft;
};

//----------------------------------------------------------------------------------------------------------------------
// KnobLooksAndFeel class
//----------------------------------------------------------------------------------------------------------------------
class KnobLooksAndFeel : public LookAndFeel_V4
{
public:
  KnobLooksAndFeel(const juce::Colour& main_colour);

  void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional,
    float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

  void drawLabel(Graphics& g, Label& label) override;

  Slider::SliderLayout getSliderLayout(Slider& slider) override;

  void setMainColour(const Colour& colour) { main_colour = colour; }

private:
  juce::Colour main_colour;
};

//----------------------------------------------------------------------------------------------------------------------
// knob global configures
//----------------------------------------------------------------------------------------------------------------------
static std::unique_ptr<KnobLooksAndFeel> knob_lf = std::make_unique<KnobLooksAndFeel>(Colour(132, 192, 106));

static void setKnobColor(const Colour& colour) { knob_lf.get()->setMainColour(colour); }

} // namespace GUI
} // namespace iNVOXRecords