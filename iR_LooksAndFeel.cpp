#include "iR_LooksAndFeel.h"
#include "iR_Knob.h"

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_LooksAndFeel implementation
//----------------------------------------------------------------------------------------------------------------------
iR_LooksAndFeel::iR_LooksAndFeel(const juce::Colour& main_colour) :
  main_colour(main_colour)
{}

void iR_LooksAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional,
  float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
  // reset to custom angle
  rotaryStartAngle = degreesToRadians<float>(225);
  rotaryEndAngle = degreesToRadians<float>(495);

  // colours
  const auto colour01 = Colour(64, 64, 64);
  const auto colour02 = Colour(218, 218, 218);

  // configure
  const auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);
  const auto centre_x = bounds.getCentreX();
  const auto centre_y = bounds.getCentreY();
  const auto diameter = bounds.getHeight();
  const auto radius = diameter / 2.0f;
  const auto toAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
  const auto line_width = width * 0.04f;
  const auto arc_radius = radius - line_width * 0.5f;

  // draw center ellipse
  const auto ellipse_diameter = diameter * 0.7f;
  Rectangle<float> ellipse_rect(centre_x, centre_y, ellipse_diameter, ellipse_diameter);
  ellipse_rect.setCentre(centre_x, centre_y);
  g.setColour(colour01.darker());
  g.fillEllipse(ellipse_rect);
  ellipse_rect.setSize(ellipse_diameter * 0.9f, ellipse_diameter * 0.9f);
  ellipse_rect.setCentre(centre_x, centre_y);
  g.setColour(colour01);
  g.fillEllipse(ellipse_rect);

  // draw background arc
  Path back_arc_path;
  back_arc_path.addCentredArc(centre_x, centre_y, arc_radius, arc_radius,
    0.0f, rotaryStartAngle, rotaryEndAngle, true);

  g.setColour(colour01);
  g.strokePath(back_arc_path, PathStrokeType(line_width, PathStrokeType::curved, PathStrokeType::rounded));

  if (slider.isEnabled())
  {
    // draw value arc
    KnobStartPos start_pos = dynamic_cast<iR_Knob*>(&slider)->getStartPosition();
    Path value_arc_path;
    if (start_pos == KnobStartPos::StartLeft) {
      value_arc_path.addCentredArc(centre_x, centre_y, arc_radius, arc_radius, 0.0f, rotaryStartAngle, toAngle, true);
    }
    else if (start_pos == KnobStartPos::StartRight) {
      value_arc_path.addCentredArc(centre_x, centre_y, arc_radius, arc_radius, 0.0f, rotaryEndAngle, toAngle, true);
    }
    else if (start_pos == KnobStartPos::StartCenter) {
      if (sliderPosProportional > 0.5f) {
        value_arc_path.addCentredArc(centre_x, centre_y, arc_radius, arc_radius,
          0.0f, MathConstants<float>::twoPi, toAngle, true);
      }
      else {
        value_arc_path.addCentredArc(centre_x, centre_y, arc_radius, arc_radius,
          0.0f, MathConstants<float>::twoPi, toAngle, true);
      }
    }
    ColourGradient arc_gradient(ColourGradient::horizontal(main_colour.darker(), centre_x - radius,
      main_colour.brighter(), centre_x + radius));
    g.setGradientFill(arc_gradient);
    g.strokePath(value_arc_path, PathStrokeType(line_width, PathStrokeType::curved, PathStrokeType::rounded));

    // draw ellipse guide
    Path guide_path;
    const auto ellipse_radius = ellipse_diameter / 2;
    const auto guide_x = centre_x + ellipse_radius * std::sin(toAngle) * 0.5f;
    const auto guide_y = centre_y - ellipse_radius * std::cos(toAngle) * 0.5f;
    guide_path.startNewSubPath(guide_x, guide_y);
    guide_path.lineTo(value_arc_path.getCurrentPosition());

    g.setColour(colour02);
    g.setColour(colour02);
    g.strokePath(guide_path, PathStrokeType(line_width, PathStrokeType::curved, PathStrokeType::butt));

    // draw handle
    Path handle_path;
    const auto handle_width = line_width * 2.5f;
    Rectangle handle(-handle_width / 2, -handle_width / 2, handle_width, handle_width);
    AffineTransform transform = AffineTransform::rotation(toAngle).translated(value_arc_path.getCurrentPosition());
    handle_path.addRectangle(handle);

    g.setColour(colour02);
    g.fillPath(handle_path, transform);
  }
}

void iR_LooksAndFeel::drawLabel(Graphics& g, Label& label)
{
  juce::Colour a = juce::Colour(0, 0, 0);
  g.setColour(a.withAlpha(0.75f));
  g.fillRect(label.getLocalBounds());

  if (!label.isBeingEdited())
  {
    auto alpha = label.isEnabled() ? 1.0f : 0.5f;
    const Font font(label.getHeight() * 0.7f, Font::plain);

    g.setColour(label.findColour(Label::textColourId).withMultipliedAlpha(alpha));
    g.setFont(font);

    auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

    g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
      jmax(1, (int)((float)textArea.getHeight() / font.getHeight())),
      label.getMinimumHorizontalScale());
  }
}

void iR_LooksAndFeel::fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor)
{
  const Font font(textEditor.getHeight() * 0.7f, Font::plain);
  setColour(CaretComponent::caretColourId, Colour(255, 255, 255));
  textEditor.applyFontToAllText(font);
  textEditor.setJustification(Justification::centred);
}

Slider::SliderLayout iR_LooksAndFeel::getSliderLayout(Slider& slider)
{
  auto localBounds = slider.getLocalBounds();

  auto textBoxWidth = slider.getWidth() * 0.8;
  auto textBoxHeight = slider.getHeight() * 0.2;

  Slider::SliderLayout layout;

  if (slider.getTextBoxPosition() != Slider::NoTextBox) {
    layout.textBoxBounds.setWidth(textBoxWidth);
    layout.textBoxBounds.setHeight(textBoxHeight);

    layout.textBoxBounds.setX((localBounds.getWidth() - textBoxWidth) / 2);
    layout.textBoxBounds.setY(localBounds.getHeight() * 0.4);
  }

  layout.sliderBounds = localBounds;

  return layout;
}

void iR_LooksAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
  bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
  const auto size = button.getHeight();
  const Font font(size, Font::plain);

  g.drawRect(button.getBounds().reduced(2, 2), 5);

  drawTickBox(g, button, size * 0.2f, 0,
    size, size,
    button.getToggleState(),
    button.isEnabled(),
    shouldDrawButtonAsHighlighted,
    shouldDrawButtonAsDown);

  g.setColour(button.findColour(ToggleButton::textColourId));
  g.setFont(font);

  if (!button.isEnabled())
    g.setOpacity(0.5f);

  g.drawFittedText(button.getButtonText(),
    button.getLocalBounds().withTrimmedLeft(roundToInt(size * 1.3f))
    .withTrimmedRight(2),
    Justification::centredLeft, 10);
}

void iR_LooksAndFeel::drawTickBox(Graphics& g, Component& component,
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

} // namespace GUI
} // namespace iNVOXRecords