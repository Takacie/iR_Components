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

#include "KnobLookAndFeel.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {

using APVTS = AudioProcessorValueTreeState;
using KnobAttachment = AudioProcessorValueTreeState::SliderAttachment;

//----------------------------------------------------------------------------------------------------------------------
// Knob class
//----------------------------------------------------------------------------------------------------------------------
class Knob : public juce::Component
{
public:
  // enums
  enum class KnobStartPos { StartLeft, StartCenter, StartRight };

  // constructor
  Knob(APVTS& apvts, const String& parameterID, const float& scale);
  
  // override
  void paint(Graphics& g) override;
  void mouseDoubleClick(const MouseEvent& event) override;
  void mouseDown(const MouseEvent& event) override;
  void mouseDrag(const MouseEvent& event) override;
  void mouseUp(const MouseEvent& event) override;
  void parentSizeChanged() override;

  // setter
  void setShowValue(bool value) noexcept { valueLabel.setAlpha(value); }
  void setStartPosition(KnobStartPos value) noexcept { startPos = value; }
  void setInitPosition(const Point<int>& point) noexcept;
  void setUseIndividualColour(bool state, const Colour& colour) noexcept;
  void setSkewFactorFromMidPoint(float midPoint) { range.setSkewForCentre(midPoint); }

  // static
  static bool alwaysShowValue;
  static Colour staticColour;
  static void setKnobColor(const Colour& colour) noexcept { staticColour = colour; }

private:
  APVTS& apvts;
  RangedAudioParameter& parameter;

  NormalisableRange<float> range { parameter.getNormalisableRange() };
  int editStartY { 0 };
  float editStartValue { 0.0f };
  float value { 0.0f };
  ParameterAttachment attachment{ parameter, [&](float newValue) { value = range.convertTo0to1(newValue); repaint(); }, nullptr };

  const float& scale;
  Point<int> initPosition;

  Label titleLabel;
  Label valueLabel;
  KnobStartPos startPos { KnobStartPos::StartLeft };

  bool useIndividualColour { false };
  Colour mainColour { 132, 106, 192 };

  bool isEdit { false };
};

} // namespace iNVOXRecords::gui