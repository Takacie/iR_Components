#include "iR_Button.h"

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_Button implementation
//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<iR_ButtonLookAndFeel> iR_Button::lookandfeel = std::make_unique<iR_ButtonLookAndFeel>(Colour(132, 106, 192));

iR_Button::iR_Button(APVTS& apvts, const String& parameterID) :
  apvts(&apvts),
  parameter_id(parameterID),
  button_attachment(ButtonAttachment(apvts, parameterID, *this))
{
  setLookAndFeel(lookandfeel.get());
  auto text = apvts.getParameter(parameterID)->getName(16);
  setButtonText(text);
}

void iR_Button::setPosition(int x, int y, int width, float size_ratio)
{
  setBounds(x * size_ratio, y * size_ratio, width * size_ratio, 20 * size_ratio);
}

} // namespace GUI
} // namespace iNVOXRecords