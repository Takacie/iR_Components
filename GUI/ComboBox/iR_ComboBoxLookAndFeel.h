#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_ButtonLookAndFeel class
//----------------------------------------------------------------------------------------------------------------------
class iR_ComboBoxLookAndFeel : public LookAndFeel_V4
{
public:
  // constructor
  iR_ComboBoxLookAndFeel(const juce::Colour& main_colour);

  // override
  void drawComboBox(Graphics& g, int width, int height, bool,
    int, int, int, int, ComboBox& box) override
  {
    const auto round = width * 0.075f;
    const auto frame_w = width * 0.03f;
    const auto w = width * 0.9f;
    const auto h = height * 0.6f;
    const auto x = static_cast<float>(width - w) / 2;
    const auto y = height * 0.15f;
    const auto arrow_w = width * 0.2f;
    const auto arrow_h = height * 0.1f;
    const auto arrow_x = static_cast<float>(width - arrow_w) / 2;
    const auto arrow_y = height * 0.6f;
    const auto offset_x = arrow_w * 0.15f;
    const auto offset_y = arrow_w * 0.1f;
    const auto arrow_line_w = offset_y;
    Rectangle<int> boxBounds(x, y, w, h);

    const auto colour = Colour(64, 64, 64);
    g.setColour(colour);
    g.fillRoundedRectangle(boxBounds.toFloat(), round);

    g.setColour(colour.darker());
    g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), round, frame_w);

    Rectangle<int> arrowZone(arrow_x, arrow_y, arrow_w, arrow_h);
    const auto centre_y = static_cast<float>(arrowZone.getCentreY());
    Path path;
    path.startNewSubPath(arrow_x + offset_x, centre_y - offset_y);
    path.lineTo(arrow_x + arrow_w / 2, centre_y + offset_x);
    path.lineTo(arrow_x + arrow_w - offset_x, centre_y - offset_y);

    g.setColour(main_colour);
    g.strokePath(path, PathStrokeType(arrow_line_w));
  }

  void positionComboBoxText(ComboBox& box, Label& label) override
  {
    const auto w = box.getWidth();
    const auto h = box.getHeight();
    const auto font_h = h * 0.18f;
    label.setBounds(0, h * 0.325f, w, font_h);
    label.setJustificationType(Justification::centred);

    label.setFont(font_h);
  }

  void drawPopupMenuItem(Graphics& g, const Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted,
    bool isTicked, bool hasSubMenu, const String& text, const String& shortcutKeyText, const Drawable* icon,
    const Colour* textColourToUse) override
  {
    auto textColour = Colours::white;

    auto r = area.reduced(1);

    if (isHighlighted && isActive)
    {
      g.setColour(Colour(70, 70, 75));
      g.fillRect(r);
    }

    g.setColour(Colours::white);
    g.setFont(r.getHeight() * 0.7f);

    r.removeFromLeft(5);
    g.drawFittedText(text, r, Justification::centredLeft, 1);
  }

  void drawPopupMenuBackground(Graphics& g, int width, int height) override
  {
    g.fillAll(Colour(50, 50, 55));
    Rectangle<float> menu_rect(0, 0, width, height);
    g.setColour(Colour(200, 200, 205));
    g.drawRect(menu_rect);
  }

  // setter
  void setMainColour(const Colour& mainColour) { main_colour = mainColour; }

private:
  Colour main_colour;
};

} // namespace iNVOXRecords::gui