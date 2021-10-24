#include "iR_Knob.h"

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_Knob implementation
//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<iR_KnobLookAndFeel> iR_Knob::lookandfeel = std::make_unique<iR_KnobLookAndFeel>(Colour(132, 106, 192));

iR_Knob::iR_Knob(APVTS& apvts, const String& parameterID, double midPointValue) :
  apvts(&apvts),
  parameter_id(parameterID),
  knob_attachment(KnobAttachment(apvts, parameterID, *this))
{
  String text = apvts.getParameter(parameterID)->getName(16);
  title_label.setLookAndFeel(&getLookAndFeel()); // apply default to title_label
  title_label.setFont(Font(16, Font::plain));
  title_label.setText(text, dontSendNotification);
  title_label.setJustificationType(Justification::centred);
  addAndMakeVisible(title_label);

  setLookAndFeel(lookandfeel.get());
  setRange(0.0, 1.0);
  setSkewFactorFromMidPoint(midPointValue);
  setSliderStyle(SliderStyle::RotaryVerticalDrag);
}

void iR_Knob::mouseDoubleClick(const MouseEvent& event)
{
  if (ModifierKeys::getCurrentModifiers().isCtrlDown()) {
    Slider::mouseDoubleClick(event);
    return;
  }
  setShowValue(true);
  showTextBox();
}

void iR_Knob::mouseDrag(const MouseEvent& event)
{
  Slider::mouseDrag(event);
  setShowValue(true);
}

void iR_Knob::mouseUp(const MouseEvent& event)
{
  Slider::mouseUp(event);
  setShowValue(false);
}

void iR_Knob::setPosition(int x, int y, float size_ratio)
{
  setBounds(x * size_ratio, y * size_ratio, 100 * size_ratio, 100 * size_ratio);
  title_label.setBounds(0, 80 * size_ratio, 100 * size_ratio, 20 * size_ratio);
  title_label.setFont(Font(16 * size_ratio, Font::plain));
}

} // namespace GUI
} // namespace iNVOXRecords