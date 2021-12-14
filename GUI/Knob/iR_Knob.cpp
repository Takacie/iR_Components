#include "iR_Knob.h"

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_Knob implementation
//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<iR_KnobLookAndFeel> iR_Knob::lookandfeel = std::make_unique<iR_KnobLookAndFeel>(Colour(132, 106, 192));
bool iR_Knob::alwaysShowValue { false };
Colour iR_Knob::staticColour { 132, 106, 192 };

iR_Knob::iR_Knob(APVTS& apvts, const String& parameterID, const float& scale) :
  Slider(SliderStyle::RotaryVerticalDrag, TextBoxBelow),
  apvts(&apvts),
  parameterId(parameterID),
  scale(scale),
  attachment(KnobAttachment(apvts, parameterID, *this))
{
  setLookAndFeel(lookandfeel.get());
  auto range = apvts.getParameter(parameterID)->getNormalisableRange().getRange();
  auto min_value = range.getStart();
  auto max_value = range.getEnd();
  setRange(min_value, max_value);
  setSkewFactorFromMidPoint(min_value + (max_value - min_value) / 2);

  String text = apvts.getParameter(parameterID)->getName(16);
  titleLabel.setLookAndFeel(&titleLabel.getLookAndFeel()); // apply default to titleLabel
  titleLabel.setFont(Font(16, Font::plain));
  titleLabel.setText(text, dontSendNotification);
  titleLabel.setJustificationType(Justification::centred);
  addAndMakeVisible(titleLabel);

  valueLabel = dynamic_cast<Label*>(getChildComponent(0));
  valueLabel->setInterceptsMouseClicks(false, true);
  valueLabel->setFont(Font(16, Font::plain));
  if (!alwaysShowValue) setShowValue(false);
}

void iR_Knob::paint(Graphics& g)
{
  setAlpha(isEnabled() ? 1.0f : 0.5f);
  if (!isMouseOverOrDragging() && !valueLabel->isBeingEdited()) {
    if (!alwaysShowValue) setShowValue(false);
  }

  // draw
  constexpr float rotaryStartAngle = degreesToRadians<float>(225);
  constexpr float rotaryEndAngle = degreesToRadians<float>(495);
  const auto value = (getMinimum() + getValue()) / (getMaximum() - getMinimum());

  // colours
  const auto colour01 = Colour(64, 64, 64);
  const auto colour02 = Colour(218, 218, 218);

  // configure
  const auto b = getLocalBounds().reduced(10);
  const auto w = b.getWidth();
  const auto h = b.getHeight();
  const auto r = w / 2.0f;
  const auto centre = b.getCentreX();
  const auto to_angle = rotaryStartAngle + value * (rotaryEndAngle - rotaryStartAngle);
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

  // draw value arc
  iR_Knob::KnobStartPos start_pos = getStartPosition();
  Path value_arc_path;
  if (start_pos != iR_Knob::KnobStartPos::StartPosNone) {
    if (start_pos == iR_Knob::KnobStartPos::StartLeft) {
      value_arc_path.addCentredArc(centre, centre, arc_r, arc_r, 0.0f, rotaryStartAngle, to_angle, true);
    }
    else if (start_pos == iR_Knob::KnobStartPos::StartRight) {
      value_arc_path.addCentredArc(centre, centre, arc_r, arc_r, 0.0f, rotaryEndAngle, to_angle, true);
    }
    else if (start_pos == iR_Knob::KnobStartPos::StartCenter) {
      if (value > 0.5f) {
        value_arc_path.addCentredArc(centre, centre, arc_r, arc_r,
          0.0f, MathConstants<float>::twoPi, to_angle, true);
      }
      else {
        value_arc_path.addCentredArc(centre, centre, arc_r, arc_r,
          0.0f, MathConstants<float>::twoPi, to_angle, true);
      }
    }
  }

  g.setColour(useIndividualColour ? mainColour : staticColour);
  g.strokePath(value_arc_path, PathStrokeType(line_width, PathStrokeType::curved, PathStrokeType::rounded));

  // draw ellipse guide
  Path guide_path;
  const auto ellipse_r = ellipse_diameter * 0.5f;
  const auto guide_x = centre + ellipse_r * std::sin(to_angle) * 0.5f;
  const auto guide_y = centre - ellipse_r * std::cos(to_angle) * 0.5f;
  guide_path.startNewSubPath(guide_x, guide_y);
  guide_path.lineTo(value_arc_path.getCurrentPosition());

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
  if (!alwaysShowValue) setShowValue(false);
}

void iR_Knob::parentSizeChanged()
{
  const int x = initPosition.getX() * scale;
  const int y = initPosition.getY() * scale;
  const int w = 100 * scale;
  const int h = 110 * scale;

  setBounds(x, y, w, h);
  titleLabel.setBounds(0, 87.5f * scale, 100 * scale, 20 * scale);
  titleLabel.setFont(16 * scale);
}

} // namespace iNVOXRecords::gui