/*///-------------------------------------------------------------------------------------------------------------------
Copyright(c) 2021 iNVOXRecords

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
/*///-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_KnobLookAndFeel class
//----------------------------------------------------------------------------------------------------------------------
class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
  // constructor
  KnobLookAndFeel(const Colour& mainColour = Colour(132, 106, 192));

  // override
  void drawLabel(Graphics& g, juce::Label& label) override;
  void fillTextEditorBackground(Graphics& g, int width, int height, juce::TextEditor& textEditor) override;
  void drawTextEditorOutline(Graphics&, int width, int height, juce::TextEditor&) override {}

  // setter
  void setMainColour(const Colour& colour) { mainColour = colour; }

private:
  Colour mainColour;
};

} // namespace iNVOXRecords::gui