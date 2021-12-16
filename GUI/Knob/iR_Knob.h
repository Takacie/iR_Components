#pragma once

#include "iR_KnobLookAndFeel.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {

using APVTS = AudioProcessorValueTreeState;
using KnobAttachment = AudioProcessorValueTreeState::SliderAttachment;

//----------------------------------------------------------------------------------------------------------------------
// Knob class
//----------------------------------------------------------------------------------------------------------------------
class iR_Knob : public Component
{
public:
  // enums
  enum class KnobStartPos { StartLeft, StartCenter, StartRight };

  // constructor
  iR_Knob(APVTS& apvts, const String& parameterID, const float& scale);
  
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