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
class iR_Knob : public Slider
{
public:
  // enums
  enum class KnobStartPos { StartLeft, StartCenter, StartRight, StartPosNone };

  // constructor
  iR_Knob(APVTS& apvts, const String& parameterID, const float& scale);
  
  // override
  void paint(Graphics& g) override;
  void mouseDoubleClick(const MouseEvent& event) override;
  void mouseDrag(const MouseEvent& event) override;
  void mouseUp(const MouseEvent& event) override;
  void parentSizeChanged() override;

  // getter
  KnobStartPos getStartPosition() { return startPos; }
  Label* getTitleLabel() { return &titleLabel; }

  // setter
  void setShowValue(bool value) { valueLabel->setAlpha(value); }
  void setStartPosition(KnobStartPos value) { startPos = value; }

  void setInitPosition(const Point<int>& point) noexcept
  {
	setBounds(point.getX(), point.getY(), 100 * scale, 110 * scale);
	initPosition = point;
	parentSizeChanged();
  }

  void setUseIndividualColour(bool state) { useIndividualColour = state; }

  // static
  static std::unique_ptr<iR_KnobLookAndFeel> lookandfeel;
  static bool alwaysShowValue;
  static Colour staticColour;
  static void setKnobColor(const Colour& colour) { staticColour = colour; }

private:
  APVTS* apvts;
  String parameterId;

  const float& scale;
  Point<int> initPosition;

  KnobAttachment attachment;
  Label titleLabel;
  Label* valueLabel;
  KnobStartPos startPos { KnobStartPos::StartLeft };

  bool useIndividualColour { false };
  Colour mainColour { 132, 106, 192 };
};

} // namespace iNVOXRecords::gui