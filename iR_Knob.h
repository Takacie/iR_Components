#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {

class KnobLooksAndFeel : public LookAndFeel_V4
{
public:
  KnobLooksAndFeel(const juce::Colour& main_colour) :
    main_colour(main_colour)
  {}

  void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional,
                        float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
  {
    // reset to custom angle
    rotaryStartAngle = MathConstants<float>::pi * 225 / 180;
    rotaryEndAngle = MathConstants<float>::pi * 495 / 180;

    // colours
    const auto colour01 = Colour(64, 64, 64);
    const auto colour02 = Colour(218, 218, 218);

    // configure
    const auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);
    const auto centre_x = bounds.getCentreX();
    const auto centre_y = bounds.getCentreY();
    const auto radius = bounds.getHeight() / 2.0f;
    const auto diameter = radius * 2;
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
    back_arc_path.addCentredArc(centre_x, centre_y, arc_radius, arc_radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);

    g.setColour(colour01);
    g.strokePath(back_arc_path, PathStrokeType(line_width, PathStrokeType::curved, PathStrokeType::butt));

    if (slider.isEnabled())
    {
      // draw value arc
      Path value_arc_path;
      value_arc_path.addCentredArc(centre_x, centre_y, arc_radius, arc_radius, 0.0f, rotaryStartAngle, toAngle, true);
      ColourGradient arc_gradient(ColourGradient::horizontal(main_colour.darker(), centre_x - radius, 
                                                             main_colour, centre_x + radius));
      g.setGradientFill(arc_gradient);
      g.strokePath(value_arc_path, PathStrokeType(line_width, PathStrokeType::curved, PathStrokeType::butt));

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

  void drawLabel(Graphics& g, Label& label) override
  {
    juce::Colour a = juce::Colour(0, 0, 0);
    g.setColour(a.withAlpha(0.5f));
    g.fillRect(label.getLocalBounds());

    if (!label.isBeingEdited())
    {
      auto alpha = label.isEnabled() ? 1.0f : 0.5f;
      const Font font(getLabelFont(label));

      g.setColour(label.findColour(Label::textColourId).withMultipliedAlpha(alpha));
      g.setFont(font);

      auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

      g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
        jmax(1, (int)((float)textArea.getHeight() / font.getHeight())),
        label.getMinimumHorizontalScale());
    }
  }

  Slider::SliderLayout getSliderLayout(Slider& slider)
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

private:
  juce::Colour main_colour;
};

using KnobAttachment = AudioProcessorValueTreeState::SliderAttachment;
class Knob : public Slider
{
public:
  Knob(AudioProcessorValueTreeState& apvts, String parameterID, LookAndFeel& lf, double midPointValue = 0.5) :
    apvts(&apvts),
    parameter_id(parameterID),
    knob_attachment(std::make_unique<KnobAttachment>(apvts, parameterID, *this))
  {
    setRange(0.0, 1.0);
    setSkewFactorFromMidPoint(midPointValue);
    setLookAndFeel(&lf);
    setSliderStyle(SliderStyle::RotaryVerticalDrag);
    setTextBoxStyle(TextBoxBelow, false, 0, 0);

    String text = apvts.getParameter(parameterID)->getName(16);
    title_label->setFont(Font(16, Font::plain));
    title_label->setText(text, dontSendNotification);
    title_label->setJustificationType(Justification::centred);

    setShowValue(false);
  }

  void addAndMakeVisibleMyself(AudioProcessorEditor& editor)
  {
    editor.addAndMakeVisible(this);
    editor.addAndMakeVisible(title_label);
  }

  void mouseDoubleClick(const MouseEvent& event) override
  {
    if (modifier_flag == modifier_flag & ModifierKeys::Flags::ctrlModifier) {
      Slider::mouseDoubleClick(event);
      return;
    }
    setShowValue(true);
    showTextBox();
  }

  void mouseDown(const MouseEvent& event) override
  {
    Slider::mouseDown(event);
  }

  void modifierKeysChanged(const ModifierKeys& modifiers) override
  {
    Slider::modifierKeysChanged(modifiers);
    modifier_flag = modifiers.getRawFlags();
  }

  void mouseDrag(const MouseEvent& event) override
  {
    Slider::mouseDrag(event);
    setShowValue(true);
  }

  void mouseUp(const MouseEvent& event) override
  {
    Slider::mouseUp(event);
    setShowValue(false);
  }

  void setPosition(int x, int y, float size_ratio)
  {
    setBounds(x, y, 100 * size_ratio, 100 * size_ratio);
    title_label->setBounds(x, y + 90 * size_ratio, 100 * size_ratio, 20 * size_ratio);
    title_label->setFont(Font(16 * size_ratio, Font::plain));
  }

  void setLabelFont(const Font& newFont)
  {
    title_label->setFont(newFont);
  }

  void setShowValue(bool value)
  { 
    show_value = value;
    if (value) {
      setTextBoxStyle(TextBoxBelow, false, 0, 0);
    }
    else {
      setTextBoxStyle(NoTextBox, false, 0, 0);
    }
  }

private:
  AudioProcessorValueTreeState* apvts;
  juce::String parameter_id;
  std::unique_ptr<KnobAttachment> knob_attachment;
  std::unique_ptr<Label> title_label_ptr = std::make_unique<Label>();
  Label* title_label = title_label_ptr.get();

  int modifier_flag;
  bool show_value = false;
};

} // namespace GUI
} // namespace iNVOXRecords