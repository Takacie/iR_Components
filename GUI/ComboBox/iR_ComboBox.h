#pragma once

#include "iR_ComboBoxLookAndFeel.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_ComboBox class
//----------------------------------------------------------------------------------------------------------------------
using APVTS = AudioProcessorValueTreeState;
class iR_ComboBox : public ComboBox
{
public:
  // constructor
  iR_ComboBox(APVTS& apvts, const String& parameterID, const StringArray& items, int default_index = 0) :
    apvts(&apvts),
    parameter_id(parameterID),
    attachment(*apvts.getParameter(parameterID), *this)
  {
    setLookAndFeel(lookandfeel.get());
    getRootMenu()->setLookAndFeel(lookandfeel.get());
    addItemList(items, 1);
    setSelectedItemIndex(default_index);

    String text = apvts.getParameter(parameterID)->getName(16);
    title_label.setFont(Font(16, Font::plain));
    title_label.setText(text, dontSendNotification);
    title_label.setJustificationType(Justification::centred);
    addAndMakeVisible(title_label);
  }

  // override
  void paint(Graphics& g) override
  {
    ComboBox::paint(g);
    setAlpha(isEnabled() ? 1.0f : 0.5f);
  }

  void showPopup() override
  {
    //ComboBox::showPopup(); <- discard isPopupActive()
    getRootMenu()->showMenuAsync(PopupMenu::Options()
      .withPreferredPopupDirection(PopupMenu::Options::PopupDirection::downwards)
      .withTargetComponent(this)
      .withMinimumWidth(getWidth())
      .withStandardItemHeight(getHeight() * 0.25f),
      [&](int choice) {
        hidePopup();
        if (choice != 0) setSelectedId(choice);
    });
  }

  // getter

  // setter
  void setPosition(int x, int y, float scale)
  {
    setBounds(x * scale, y * scale, 100 * scale, 110 * scale);
    title_label.setBounds(0, 87.5f * scale, 100 * scale, 20 * scale);
    title_label.setFont(Font(16 * scale, Font::plain));
  }

  // static
  static std::unique_ptr<iR_ComboBoxLookAndFeel> lookandfeel;
  static void setComboBoxColor(const Colour& colour) { if (lookandfeel) lookandfeel->setMainColour(colour); }

private:
  APVTS* apvts;
  String parameter_id;
  Label title_label;
  ComboBoxParameterAttachment attachment;
};

} // namespace iNVOXRecords::gui