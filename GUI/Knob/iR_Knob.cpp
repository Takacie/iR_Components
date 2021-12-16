#include "iR_Knob.h"

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_Knob implementation
//----------------------------------------------------------------------------------------------------------------------
bool iR_Knob::alwaysShowValue { false };
Colour iR_Knob::staticColour { 132, 106, 192 };

iR_Knob::iR_Knob(APVTS& apvts, const String& parameterID, const float& scale) :
  apvts(apvts),
  parameter(*apvts.getParameter(parameterID)),
  scale(scale)
{
  auto minValue = range.start;
  auto maxValue = range.end;
  setSkewFactorFromMidPoint(minValue + (maxValue - minValue) / 2);

  const String text = parameter.getName(16);
  titleLabel.setFont(Font(16, Font::plain));
  titleLabel.setText(text, dontSendNotification);
  titleLabel.setJustificationType(Justification::centred);
  addAndMakeVisible(titleLabel);

  if (!alwaysShowValue)
    setShowValue(false);

  attachment.sendInitialUpdate();
}

void iR_Knob::paint(Graphics& g)
{
  setAlpha(isEnabled() ? 1.0f : 0.5f);

  if (!isMouseOverOrDragging() && !valueLabel.isBeingEdited() && !alwaysShowValue) {
    setShowValue(false);
  }

  // colours
  const auto colour01 = Colour(64, 64, 64);
  const auto colour02 = Colour(218, 218, 218);

  // configure
  constexpr float startAngle = degreesToRadians<float>(225);
  constexpr float endAngle = degreesToRadians<float>(495);
  const auto b = getLocalBounds().reduced(10);
  const auto w = b.getWidth();
  const auto h = b.getHeight();
  const auto r = w / 2.0f;
  const auto centre = b.getCentreX();
  const auto toAngle = startAngle + value * (endAngle - startAngle);
  const auto lineW = w * 0.04f;
  const auto arcR = r - lineW * 0.5f;

  // draw center ellipse
  const auto ellipseDiameter = w * 0.7f;
  Rectangle<float> ellipseRect(centre, centre, ellipseDiameter, ellipseDiameter);

  ellipseRect.setCentre(centre, centre);
  g.setColour(colour01.darker());
  g.fillEllipse(ellipseRect);
  ellipseRect.setSize(ellipseDiameter * 0.9f, ellipseDiameter * 0.9f);
  ellipseRect.setCentre(centre, centre);
  g.setColour(colour01);
  g.fillEllipse(ellipseRect);

  // draw background arc
  Path backArcPath;

  backArcPath.addCentredArc(centre, centre, arcR, arcR, 0.0f, startAngle, endAngle, true);
  g.setColour(colour01);
  g.strokePath(backArcPath, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

  // draw value arc
  Path valueArcPath;

  switch (startPos)
  {
  case iR_Knob::KnobStartPos::StartLeft:
    valueArcPath.addCentredArc(centre, centre, arcR, arcR, 0.0f, startAngle, toAngle, true);
    break;
  case iR_Knob::KnobStartPos::StartRight:
    valueArcPath.addCentredArc(centre, centre, arcR, arcR, 0.0f, endAngle, toAngle, true);
    break;
  case iR_Knob::KnobStartPos::StartCenter:
    valueArcPath.addCentredArc(centre, centre, arcR, arcR, 0.0f, MathConstants<float>::twoPi, toAngle, true);
    break;
  default:
    break;
  }

  g.setColour(useIndividualColour ? mainColour : staticColour);
  g.strokePath(valueArcPath, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

  // draw ellipse guide
  Path guidePath;
  const auto ellipseR = ellipseDiameter * 0.5f;
  const auto guide_x = centre + ellipseR * std::sin(toAngle) * 0.5f;
  const auto guide_y = centre - ellipseR * std::cos(toAngle) * 0.5f;

  guidePath.startNewSubPath(guide_x, guide_y);
  guidePath.lineTo(valueArcPath.getCurrentPosition());
  g.setColour(colour02);
  g.strokePath(guidePath, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::butt));

  // draw handle
  Path handle_path;
  const auto handle_width = lineW * 2.5f;
  const Rectangle<float> handle(-handle_width / 2, -handle_width / 2, handle_width, handle_width);
  const AffineTransform transform = AffineTransform::rotation(toAngle).translated(valueArcPath.getCurrentPosition());

  handle_path.addRectangle(handle);
  g.setColour(colour02);
  g.fillPath(handle_path, transform);
}

void iR_Knob::mouseDoubleClick(const MouseEvent& event)
{
  if (ModifierKeys::getCurrentModifiers().isCtrlDown()) {
    const float denormalized = parameter.getNormalisableRange().convertFrom0to1(parameter.getDefaultValue());
    attachment.setValueAsPartOfGesture(denormalized);
    return;
  }

  setShowValue(true);
  //showTextBox();
}

void iR_Knob::mouseDown(const MouseEvent& event)
{
  isEdit = true;
  editStartValue = value;
  editStartY = event.getMouseDownY();
  setShowValue(true);
}

void iR_Knob::mouseDrag(const MouseEvent& event)
{
  const int distance = editStartY - event.getPosition().getY();
  const float factor = 0.0025f;
  const float clamped = std::clamp(distance * factor + editStartValue, 0.0f, 1.0f);
  const float denormalize = range.convertFrom0to1(clamped);

  attachment.setValueAsPartOfGesture(denormalize);
}

void iR_Knob::mouseUp(const MouseEvent& event)
{
  isEdit = false;

  if (!alwaysShowValue)
    setShowValue(false);
}

void iR_Knob::parentSizeChanged()
{
  // set knob bounds
  const int x = initPosition.getX() * scale;
  const int y = initPosition.getY() * scale;
  const int w = 100 * scale;
  const int h = 110 * scale;

  setBounds(x, y, w, h);

  // set value label bounds
  const int vlW = w * 0.8f;
  const int vlH = h * 0.2f;
  const int vlX = (getWidth() - vlW) / 2;
  const int vlY = getHeight() * 0.35f;

  valueLabel.setBounds(vlX, vlY, vlW, vlH);

  // set title label bounds
  titleLabel.setBounds(0, 87.5f * scale, 100 * scale, 20 * scale);
  titleLabel.setFont(16 * scale);
}

void iR_Knob::setInitPosition(const Point<int>& point) noexcept
{
  setBounds(point.getX(), point.getY(), 100 * scale, 110 * scale);
  initPosition = point;
  parentSizeChanged();
}

void iR_Knob::setUseIndividualColour(bool state, const Colour& colour) noexcept
{
  useIndividualColour = state;
  mainColour = colour;
}

} // namespace iNVOXRecords::gui