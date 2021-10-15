#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_LooksAndFeel class
//----------------------------------------------------------------------------------------------------------------------
class iR_LooksAndFeel : public LookAndFeel_V4
{
public:
  // constructor
  iR_LooksAndFeel(const juce::Colour& main_colour);

  // override
  void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional,
    float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
  void drawLabel(Graphics& g, Label& label) override;
  void fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor) override;
  void drawTextEditorOutline(Graphics&, int width, int height, TextEditor&) override {}
  Slider::SliderLayout getSliderLayout(Slider& slider) override;
  void drawToggleButton(Graphics& g, ToggleButton& button,
                        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
  void drawTickBox(Graphics& g, Component& component, float x, float y, float w, float h, 
                   const bool ticked, const bool isEnabled, const bool shouldDrawButtonAsHighlighted,
                   const bool shouldDrawButtonAsDown) override;

private:
  Colour main_colour;
};

} // namespace GUI
} // namespace iNVOXRecords