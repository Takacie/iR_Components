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

#include "ButtonLookAndFeel.h"

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// ButtonLookAndFeel implementation
//----------------------------------------------------------------------------------------------------------------------
ButtonLookAndFeel::ButtonLookAndFeel(const juce::Colour& main_colour) :
  main_colour(main_colour)
{}

void ButtonLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
  bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
  const auto size = button.getHeight();
  const Font font(size, Font::plain);

  drawTickBox(g, button, size * 0.2f, 0,
    size, size,
    button.getToggleState(),
    button.isEnabled(),
    shouldDrawButtonAsHighlighted,
    shouldDrawButtonAsDown);

  g.setColour(button.findColour(ToggleButton::textColourId));
  g.setFont(font);

  g.drawFittedText(button.getButtonText(),
    button.getLocalBounds().withTrimmedLeft(roundToInt(size * 1.3f))
    .withTrimmedRight(2),
    Justification::centredLeft, 1);
}

void ButtonLookAndFeel::drawTickBox(Graphics& g, Component& component,
  float x, float y, float w, float h,
  const bool ticked,
  const bool isEnabled,
  const bool shouldDrawButtonAsHighlighted,
  const bool shouldDrawButtonAsDown)
{
  ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

  Rectangle<float> tickBounds(x, y, w, h);
  tickBounds.reduce(h * 0.1f, h * 0.1f);

  if (ticked) {
    g.setColour(main_colour);
    g.fillRoundedRectangle(tickBounds, h * 0.3f);
  }

  g.setColour(Colours::white);
  g.drawRoundedRectangle(tickBounds, h * 0.3f, h * 0.075f);
}

} // namespace iNVOXRecords::gui