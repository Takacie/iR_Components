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

#include "ButtonLookAndFeel.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// Button class
//----------------------------------------------------------------------------------------------------------------------
using APVTS = AudioProcessorValueTreeState;
using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;
class Button : public juce::ToggleButton
{
public:
  // constructor
  Button(APVTS& apvts, const String& parameterID);

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
  static std::unique_ptr<ButtonLookAndFeel> lookandfeel;
  static void setButtonColor(const Colour& colour) { if (lookandfeel) lookandfeel->setMainColour(colour); }

private:
  APVTS* apvts;
  String parameter_id;
  ButtonAttachment button_attachment;
};

} // namespace iNVOXRecords::gui