#include "iR_KnobLookAndFeel.h"
#include "iR_Knob.h"

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_KnobLookAndFeel implementation
//----------------------------------------------------------------------------------------------------------------------
iR_KnobLookAndFeel::iR_KnobLookAndFeel(const Colour& mainColour) :
  main_colour(mainColour)
{}

void iR_KnobLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional,
  float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
  // set to custom angle
  rotaryStartAngle = degreesToRadians<float>(225);
  rotaryEndAngle = degreesToRadians<float>(495);

  // colours
  const auto colour01 = Colour(64, 64, 64);
  const auto colour02 = Colour(218, 218, 218);

  // configure
  const auto b = Rectangle<float>(x, y, width, height).reduced(10);
  const auto w = b.getWidth();
  const auto h = b.getHeight();
  const auto r = w / 2.0f;
  const auto centre = b.getCentreX();
  const auto to_angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
  const auto line_width = w * 0.04f;
  const auto arc_r = r - line_width * 0.5f;

  // draw center ellipse
  const auto ellipse_diameter = w * 0.7f;
  Rectangle<float> ellipse_rect(centre, centre, ellipse_diameter, ellipse_diameter);
  ellipse_rect.setCentre(centre, centre);
  g.setColour(colour01.darker());
  g.fillEllipse(ellipse_rect);
  ellipse_rect.setSize(ellipse_diameter * 0.9f, ellipse_diameter * 0.9f);
  ellipse_rect.setCentre(centre, centre);
  g.setColour(colour01);
  g.fillEllipse(ellipse_rect);

  // draw background arc
  Path back_arc_path;
  back_arc_path.addCentredArc(centre, centre, arc_r, arc_r,
    0.0f, rotaryStartAngle, rotaryEndAngle, true);

  g.setColour(colour01);
  g.strokePath(back_arc_path, PathStrokeType(line_width, PathStrokeType::curved, PathStrokeType::rounded));

  if (slider.isEnabled())
  {
    // draw value arc
    KnobStartPos start_pos = dynamic_cast<iR_Knob*>(&slider)->getStartPosition();
    Path value_arc_path;
    if (start_pos == KnobStartPos::StartLeft) {
      value_arc_path.addCentredArc(centre, centre, arc_r, arc_r, 0.0f, rotaryStartAngle, to_angle, true);
    }
    else if (start_pos == KnobStartPos::StartRight) {
      value_arc_path.addCentredArc(centre, centre, arc_r, arc_r, 0.0f, rotaryEndAngle, to_angle, true);
    }
    else if (start_pos == KnobStartPos::StartCenter) {
      if (sliderPosProportional > 0.5f) {
        value_arc_path.addCentredArc(centre, centre, arc_r, arc_r,
          0.0f, MathConstants<float>::twoPi, to_angle, true);
      }
      else {
        value_arc_path.addCentredArc(centre, centre, arc_r, arc_r,
          0.0f, MathConstants<float>::twoPi, to_angle, true);
      }
    }

    g.setColour(main_colour);
    g.strokePath(value_arc_path, PathStrokeType(line_width, PathStrokeType::curved, PathStrokeType::rounded));

    // draw ellipse guide
    Path guide_path;
    const auto ellipse_r = ellipse_diameter * 0.5f;
    const auto guide_x = centre + ellipse_r * std::sin(to_angle) * 0.5f;
    const auto guide_y = centre - ellipse_r * std::cos(to_angle) * 0.5f;
    guide_path.startNewSubPath(guide_x, guide_y);
    guide_path.lineTo(value_arc_path.getCurrentPosition());

    g.setColour(colour02);
    g.setColour(colour02);
    g.strokePath(guide_path, PathStrokeType(line_width, PathStrokeType::curved, PathStrokeType::butt));

    // draw handle
    Path handle_path;
    const auto handle_width = line_width * 2.5f;
    Rectangle handle(-handle_width / 2, -handle_width / 2, handle_width, handle_width);
    AffineTransform transform = AffineTransform::rotation(to_angle).translated(value_arc_path.getCurrentPosition());
    handle_path.addRectangle(handle);

    g.setColour(colour02);
    g.fillPath(handle_path, transform);
  }
}

void iR_KnobLookAndFeel::drawLabel(Graphics& g, Label& label)
{
  auto a = Colour(0, 0, 0);
  g.setColour(a.withAlpha(0.75f));
  g.fillRect(label.getLocalBounds());

  if (!label.isBeingEdited())
  {
    auto alpha = label.isEnabled() ? 1.0f : 0.5f;
    const Font font(label.getHeight() * 0.7f, Font::plain);

    g.setColour(label.findColour(Label::textColourId).withMultipliedAlpha(alpha));
    g.setFont(font);

    auto text_rect = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

    g.drawFittedText(label.getText(), text_rect, label.getJustificationType(),
      jmax(1, (int)((float)text_rect.getHeight() / font.getHeight())),
      label.getMinimumHorizontalScale());
  }
}

void iR_KnobLookAndFeel::fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor)
{
  ignoreUnused(width, height);

  const Font font(textEditor.getHeight() * 0.7f, Font::plain);
  setColour(CaretComponent::caretColourId, Colour(255, 255, 255));
  textEditor.applyFontToAllText(font);
  textEditor.setJustification(Justification::centred);
}

Slider::SliderLayout iR_KnobLookAndFeel::getSliderLayout(Slider& slider)
{
  auto localBounds = slider.getLocalBounds();

  auto textBoxWidth = slider.getWidth() * 0.8;
  auto textBoxHeight = slider.getHeight() * 0.2;

  Slider::SliderLayout layout;

  layout.textBoxBounds.setWidth(textBoxWidth);
  layout.textBoxBounds.setHeight(textBoxHeight);

  layout.textBoxBounds.setX((localBounds.getWidth() - textBoxWidth) / 2);
  layout.textBoxBounds.setY(localBounds.getHeight() * 0.4 - 5);

  layout.sliderBounds = localBounds;

  return layout;
}

} // namespace GUI
} // namespace iNVOXRecords