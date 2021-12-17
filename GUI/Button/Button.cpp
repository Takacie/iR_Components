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

#include "Button.h"

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// Button implementation
//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<ButtonLookAndFeel> Button::lookandfeel = std::make_unique<ButtonLookAndFeel>(Colour(132, 106, 192));

Button::Button(APVTS& apvts, const String& parameterID) :
  apvts(&apvts),
  parameter_id(parameterID),
  button_attachment(ButtonAttachment(apvts, parameterID, *this))
{
  setLookAndFeel(lookandfeel.get());
  auto text = apvts.getParameter(parameterID)->getName(16);
  setButtonText(text);
}

void Button::setPosition(int x, int y, int width, float size_ratio)
{
  setBounds(x * size_ratio, y * size_ratio, width * size_ratio, 20 * size_ratio);
}

} // namespace iNVOXRecords::gui