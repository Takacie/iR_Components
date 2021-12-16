#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_KnobLookAndFeel class
//----------------------------------------------------------------------------------------------------------------------
class iR_KnobLookAndFeel : public LookAndFeel_V4
{
public:
  // constructor
  iR_KnobLookAndFeel(const Colour& mainColour = Colour(132, 106, 192));

  // override
  void drawLabel(Graphics& g, Label& label) override;
  void fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor) override;
  void drawTextEditorOutline(Graphics&, int width, int height, TextEditor&) override {}

  // setter
  void setMainColour(const Colour& mainColour) { main_colour = mainColour; }

private:
  Colour main_colour;
};

} // namespace iNVOXRecords::gui