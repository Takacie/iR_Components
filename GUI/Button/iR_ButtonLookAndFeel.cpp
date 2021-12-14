#include "iR_ButtonLookAndFeel.h"

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_ButtonLookAndFeel implementation
//----------------------------------------------------------------------------------------------------------------------
iR_ButtonLookAndFeel::iR_ButtonLookAndFeel(const juce::Colour& main_colour) :
  main_colour(main_colour)
{}

void iR_ButtonLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
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

void iR_ButtonLookAndFeel::drawTickBox(Graphics& g, Component& component,
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