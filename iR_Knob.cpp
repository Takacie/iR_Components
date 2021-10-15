#include "iR_Knob.h"

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_Knob implementation
//----------------------------------------------------------------------------------------------------------------------
iR_Knob::iR_Knob(APVTS& apvts, const String& parameterID, LookAndFeel* look_and_feel, double midPointValue) :
  apvts(&apvts),
  parameter_id(parameterID),
  knob_attachment(std::make_unique<KnobAttachment>(apvts, parameterID, *this))
{
  setRange(0.0, 1.0);
  setSkewFactorFromMidPoint(midPointValue);
  setLookAndFeel(look_and_feel);
  setSliderStyle(SliderStyle::RotaryVerticalDrag);

  String text = apvts.getParameter(parameterID)->getName(16);
  title_label->setFont(Font(16, Font::plain));
  title_label->setText(text, dontSendNotification);
  title_label->setJustificationType(Justification::centred);

  value_label = dynamic_cast<Label*>(getChildComponent(0));
  value_label->setInterceptsMouseClicks(false, true);
  value_label->onEditorHide = [this] { setShowValue(false); };
  setShowValue(false);
}

void iR_Knob::addAndMakeVisibleMyself(AudioProcessorEditor& editor)
{
  editor.addAndMakeVisible(this);
  editor.addAndMakeVisible(title_label);
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
  setBounds(x * size_ratio, y * size_ratio, 100 * size_ratio, 95 * size_ratio);
  title_label->setBounds(x * size_ratio, (y + 80) * size_ratio, 100 * size_ratio, 20 * size_ratio);
  title_label->setFont(Font(16 * size_ratio, Font::plain));
}

} // namespace GUI
} // namespace iNVOXRecords