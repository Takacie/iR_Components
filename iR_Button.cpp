#include "iR_Button.h"

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_Button implementation
//----------------------------------------------------------------------------------------------------------------------
iR_Button::iR_Button(APVTS& apvts, const String& parameterID, LookAndFeel* look_and_feel, double midPointValue) :
  apvts(&apvts),
  parameter_id(parameterID),
  button_attachment(std::make_unique<ButtonAttachment>(apvts, parameterID, *this))
{
  setLookAndFeel(look_and_feel);
  String text = apvts.getParameter(parameterID)->getName(16);
  setButtonText(text);
}

void iR_Button::addAndMakeVisibleMyself(AudioProcessorEditor& editor)
{
  editor.addAndMakeVisible(this);
}

void iR_Button::setPosition(int x, int y, int width, float size_ratio)
{
  setBounds(x * size_ratio, y * size_ratio, width * size_ratio, 20 * size_ratio);
}

} // namespace GUI
} // namespace iNVOXRecords