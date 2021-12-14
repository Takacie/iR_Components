#include "iR_KnobLookAndFeel.h"
#include "iR_Knob.h"

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_KnobLookAndFeel implementation
//----------------------------------------------------------------------------------------------------------------------
iR_KnobLookAndFeel::iR_KnobLookAndFeel(const Colour& mainColour) :
  main_colour(mainColour)
{}

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

} // namespace iNVOXRecords::gui